#!/usr/bin/env python3
"""Tests for hostfixtures.py, on made-up C functions and made-up fixtures.

Every function and every fixtures file here is made up and written by this file as a dictionary in the documented
format (`ps1/src/slot06_nonmatching/difftest.py`'s header); nothing was recorded, there is no emulator, no disc
image and no PS1 compiler. The programs are built for real with the cross compiler CC (about a second each) and
started directly, as `test_hostreplay.py` starts them. Every expected line and value is written out by hand below,
never computed by a copy of the code under test.

    python3 test_hostfixtures.py --cc CC

The last line says how many tests passed and how long they took.
"""

from __future__ import annotations

import argparse
import contextlib
import io
import json
import sys
import tempfile
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import hostfixtures as hf  # noqa: E402
import hostreplay as hr  # noqa: E402

CONFIG = """[types]
fields = "t.fields"
header = "t.h"

[[unit]]
name = "ua"
source = "a.c"
functions = [
  { name = "stub_a", address = 0x80100000, size = 16 },
  { name = "stub_b", address = 0x80100010, size = 16 },
]
"""
SYMBOLS = "data_word = 0x80190000;\n"

SOURCES = {
    "add": "int add(int a, int b) { return a + b; }\n",
    "loc0": "extern int stub_a(int *p);\nint loc0(void) { int local = 0; stub_a(&local); return 0; }\n",
    "loc256": "extern int stub_a(int *p);\nint loc256(void) { int local = 256; stub_a(&local); return 0; }\n",
    "loc119": "extern int stub_a(int *p);\nint loc119(void) { int local = 119; stub_a(&local); return 0; }\n",
    "loc375": "extern int stub_a(int *p);\nint loc375(void) { int local = 375; stub_a(&local); return 0; }\n",
    "locnone": "extern int stub_a(int *p);\nint locnone(void) { int local; stub_a(&local); return 0; }\n",
    "five": "int five(int a, int b, int c, int d, short e, int f) { return a + b + c + d + e * 16 + f; }\n",
    "u8f": "unsigned char u8f(int a) { return (unsigned char)a; }\n",
    "s16f": "short s16f(int a) { return (short)a; }\n",
    "voidf": "void voidf(int *p) { *p = 1; }\n",
    "st": "void st(unsigned *p, unsigned v) { *p = v; }\n",
    "nostore": "void nostore(unsigned *p) { (void)p; }\n",
    "c1": "extern int stub_a(int a, int b, int c);\n"
          "int c1(int *p) { *p = 5; stub_a(0x12345678, (int)p, 7); *p = 6; return 0; }\n",
    "ret4": "extern int stub_a(void);\n"
            "int ret4(void) { int a = stub_a(); int b = stub_a(); int c = stub_a(); int d = stub_a();\n"
            "  return ((a * 10 + b) * 10 + c) * 10 + d; }\n",
    "ends": "extern int stub_a(void);\nextern int data_word;\n"
            "int ends(void) { stub_a(); stub_a(); data_word = 99; return 1; }\n",
    "stk": "extern int stub_a(int *a, int *b);\n"
           "int stk(int *q) { int local = 0x77; stub_a(&local, q); return local; }\n",
    "fault": "typedef void (*fn)(void);\nextern void __attribute__((stdcall)) ExitProcess(unsigned);\n"
             "int fault(int mode, int addr, int c)\n{\n"
             "  switch (mode) {\n"
             "  case 0: return 42;\n"
             "  case 1: return *(volatile int *)addr;\n"
             "  case 2: *(volatile int *)addr = 1; return 5;\n"
             "  case 3: ((fn)addr)(); return 6;\n"
             "  case 4: ExitProcess(9); return 0;\n"
             "  case 5: for (;;) {}\n"
             "  case 6: return 100 / c;\n"
             "  case 8: *(volatile int *)0x80000900 = 1; return *(volatile int *)0;\n"
             "  }\n  return 0;\n}\n",
    "spin": "int spin(void) { for (;;) {} }\n",
    "bad": "int bad(void) { return 1 }\n",
    "nodef": "extern int nodef(void);\n",
    "nw": "extern int nowhere(void);\nint nw(void) { return nowhere(); }\n",
    "one": "int one(int a) { (void)a; return 1; }\n",
    "two": "int two(int a) { (void)a; return 2; }\n",
}

A5 = 0xA5


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def fx(**kw) -> dict:
    """One fixture in the documented format, with every field; keys given replace the defaults."""
    base = {"args": [], "calls": [], "case": 0, "ends_in_callee": False, "note": "t", "reads": [], "recorders": [], "result": None,
            "same": [], "seed": 1, "watch": [], "writes": []}
    base.update(kw)
    return base


def doc(name: str, fixtures: list, **kw) -> dict:
    out = {"fixtures": fixtures, "format": 1, "function": name, "recorded": {"cases": 1}, "uncovered": {"edges": [], "slots": []}}
    out.update(kw)
    return out


def call(callee="stub_a", args=(), returned="0x00000000", pointees=None, watched=()) -> dict:
    return {"args": list(args), "callee": callee, "pointees": pointees or {}, "returned": returned, "watched": list(watched)}


def recorder(callee="stub_a", at="0x80100000", arguments=0, **kw) -> dict:
    return {"arguments": arguments, "at": at, "callee": callee, **kw}


class Rig:
    def __init__(self, root: Path, cc: str):
        self.root, self.cc = root, cc
        self.src = root / "src"
        self.src.mkdir()
        (self.src / "build.toml").write_text(CONFIG)
        (self.src / "t.fields").write_text("struct S size=0x4\n0x000 u32 a\n")
        (self.src / "symbols.ld").write_text(SYMBOLS)
        (self.src / "a.c").write_text("int x;\n")
        self.count = 0

    def folder(self, sources: dict[str, str], docs: dict[str, object]) -> Path:
        """A new folder with the C of `sources` (by name) and the fixtures files of `docs`; a str value is written as it is."""
        self.count += 1
        folder = self.root / f"case{self.count}" / "t_nonmatching"
        folder.mkdir(parents=True)
        for name, text in sources.items():
            (folder / f"{name}.c").write_text(text)
        for name, value in docs.items():
            (folder / f"{name}.fixtures.json").write_text(value if isinstance(value, str) else json.dumps(value))
        return folder

    def run(self, folders, names=(), jobs=4, overrides=None, timeout=5, build=None):
        """(status, the lines on standard output, the text on standard error) of the command."""
        if overrides is None:
            overrides = self.root / "no_overrides"
            overrides.mkdir(exist_ok=True)
        argv = ["--config", str(self.src / "build.toml"), "--cc", self.cc, "--build", str(build or self.root / "build"), "--jobs", str(jobs),
                "--overrides", str(overrides), "--timeout", str(timeout)]
        for folder in folders:
            argv += ["--folder", str(folder)]
        argv += list(names)
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            status = hf.main(argv)
        return status, out.getvalue().splitlines(), err.getvalue()

    def replay(self, name: str, fixtures: list, source: str | None = None, **kw):
        folder = self.folder({name: SOURCES[name] if source is None else source}, {name: doc(name, fixtures)})
        return self.run([folder], **kw)

    def load(self, fixtures: list, name: str = "f"):
        """The parsed file of a made-up document."""
        self.count += 1
        path = self.root / f"load{self.count}.fixtures.json"
        path.write_text(json.dumps(doc(name, fixtures)))
        return hf.load_fixtures(path, name)


def counts(replayed, passed, failed, built, not_replayed) -> str:
    return f"native replay: functions replayed {replayed}, all passed {passed}, with a failure {failed}, not built {built}, not replayed {not_replayed}"


def fixture_head(number, case=0, note="t") -> str:
    return f"  fixture {number} (case {case}, {note}):"


def raises(fn, exc, *needles):
    try:
        fn()
    except exc as err:
        missing = [n for n in needles if n not in str(err)]
        return None if not missing else f"the error {str(err)!r} does not name {missing}"
    return "no error was raised"


# ---------------------------------------------------------------------------------------------------------------
# The return type.


def return_type_cases():
    def rt(source, name="f"):
        r = hf.return_type(source, name)
        return (r.kind, r.bits) if r.kind == "bits" else r.kind

    yield "type-void", same(rt("void f(int a) {\n}\n"), "void")
    yield "type-int-is-a-word", same(rt("int f(void) { return 1; }"), "word")
    yield "type-u8-is-8-bits", same(rt("u8 f(int a) { return a; }"), ("bits", 8))
    yield "type-s8-is-8-bits", same(rt("s8 f(int a) { return a; }"), ("bits", 8))
    yield "type-char-is-8-bits", same(rt("char f(int a) { return a; }"), ("bits", 8))
    yield "type-unsigned-char-is-8-bits", same(rt("unsigned char f(int a) { return a; }"), ("bits", 8))
    yield "type-signed-char-is-8-bits", same(rt("signed char f(int a) { return a; }"), ("bits", 8))
    yield "type-u16-is-16-bits", same(rt("u16 f(int a) { return a; }"), ("bits", 16))
    yield "type-s16-is-16-bits", same(rt("s16 f(int a) { return a; }"), ("bits", 16))
    yield "type-short-is-16-bits", same(rt("short f(int a) { return a; }"), ("bits", 16))
    yield "type-unsigned-short-is-16-bits", same(rt("unsigned short f(int a) { return a; }"), ("bits", 16))
    yield "type-long-is-a-word", same(rt("long f(int a) { return a; }"), "word")
    yield "type-unsigned-is-a-word", same(rt("unsigned f(int a) { return a; }"), "word")
    yield "type-pointer-to-u8-is-a-word", same(rt("u8 *f(int a) { return 0; }"), "word")
    yield "type-struct-pointer-is-a-word", same(rt("Object *f(int a) { return 0; }"), "word")
    yield "type-after-the-qualifier-static", same(rt("static u8 f(int a) { return a; }"), ("bits", 8))
    yield "type-on-the-line-before-the-name", same(rt("u8\nf(int a)\n{\n return a;\n}\n"), ("bits", 8))
    yield "type-skips-the-prototype-and-the-comments", same(rt('#include "x.h"\n/* s16 f(int a) { */\n// s8 f(int a) {\nu8 f(int);\nint g(void) { return 0; }\nu16 f(int a)\n{ return a; }\n'), ("bits", 16))
    yield "type-skips-a-call-before-the-definition", same(rt("int g(void) { f(1); return 0; }\nu8 f(int a) { return a; }\n"), ("bits", 8))
    yield "type-name-is-a-whole-word", same(rt("u8 xf(int a) { return a; }\ns16 f(int a) { return a; }\n"), ("bits", 16))
    yield "type-text-is-kept-for-the-line", same(hf.return_type("unsigned char f(void) { return 0; }", "f").text, "unsigned char")
    yield "type-no-definition-is-an-error", raises(lambda: hf.return_type("extern int f(void);\n", "f"), ValueError, "no definition of f")


# ---------------------------------------------------------------------------------------------------------------
# Reading a file.


def reader_cases(rig: Rig):
    def bad(name, mutate, wanted, fixtures=None, **kw):
        document = doc("f", fixtures if fixtures is not None else [fx()], **kw)
        mutate(document)
        path = rig.root / f"bad_{name}.fixtures.json"
        path.write_text(json.dumps(document))
        return raises(lambda: hf.load_fixtures(path, "f"), hf.FixtureError, f"bad_{name}.fixtures.json", wanted)

    good = rig.load([fx(args=["0x00000001"], reads=[["0x80000100", "0102"]], writes=[["0x80000200", "ff"]], result="0x00000007")])
    yield "reader-takes-a-file-of-format-1", same((good.format, len(good.fixtures), good.name), (1, 1, "f"))
    f0 = good.fixtures[0]
    yield "reader-turns-runs-into-bytes", same((f0.args, f0.reads, f0.writes, f0.result), ([1], {0x80000100: 1, 0x80000101: 2}, {0x80000200: 0xFF}, 7))
    yield "reader-takes-a-null-result", same(rig.load([fx()]).fixtures[0].result, None)

    path = rig.root / "notjson.fixtures.json"
    path.write_text("{")
    yield "reader-refuses-text-that-is-not-json", raises(lambda: hf.load_fixtures(path, "f"), hf.FixtureError, "notjson.fixtures.json is not JSON")
    yield "reader-refuses-a-file-that-is-not-there", raises(lambda: hf.load_fixtures(rig.root / "absent.fixtures.json", "f"), hf.FixtureError, "cannot read absent.fixtures.json")
    path = rig.root / "list.fixtures.json"
    path.write_text("[]")
    yield "reader-refuses-a-file-that-is-not-an-object", raises(lambda: hf.load_fixtures(path, "f"), hf.FixtureError, "list.fixtures.json: the file is not an object")
    yield "reader-refuses-format-3", bad("fmt", lambda d: d.update(format=3), "format 3, this tool reads format 1")
    yield "reader-refuses-a-missing-format", bad("nofmt", lambda d: d.pop("format"), "format None")
    yield "reader-refuses-another-function", bad("fn", lambda d: d.update(function="g"), "the file is for 'g', not f")
    yield "reader-refuses-no-fixtures", bad("none", lambda d: d.update(fixtures=[]), "no fixtures")
    yield "reader-refuses-a-missing-fixtures-field", bad("nofix", lambda d: d.pop("fixtures"), "no fixtures")
    yield "reader-refuses-a-fixture-without-writes", bad("nowrites", lambda d: d["fixtures"][1].pop("writes"), "fixture 1: it lacks writes", fixtures=[fx(), fx()])
    yield "reader-names-every-missing-field", bad("nomany", lambda d: [d["fixtures"][0].pop("args"), d["fixtures"][0].pop("seed")], "fixture 0: it lacks args, seed")
    yield "reader-refuses-needs-image", bad("img", lambda d: d["fixtures"][0].update(needs_image=True), "needs_image")
    yield "reader-refuses-an-address-that-is-not-a-word", bad("hex", lambda d: d["fixtures"][0].update(args=["80000000"]), "must be a hexadecimal word")
    yield "reader-refuses-odd-hex-bytes", bad("odd", lambda d: d["fixtures"][0].update(reads=[["0x80000100", "012"]]), "whole bytes")
    yield "reader-refuses-a-run-outside-memory", bad("out", lambda d: d["fixtures"][0].update(writes=[["0x801fffff", "0102"]]), "outside RAM and the scratchpad")
    yield "reader-refuses-a-call-to-an-unknown-recorder", bad("rec", lambda d: d["fixtures"][0].update(calls=[call("stub_b")]), "no recorder stands for", fixtures=[fx(recorders=[recorder()])])
    yield "reader-refuses-a-call-marked-original", bad("orig", lambda d: d["fixtures"][0]["calls"][0].update(original=True), "format 2",
                                                       fixtures=[fx(recorders=[recorder()], calls=[call()])])
    yield "reader-refuses-a-call-without-watched", bad("nowatched", lambda d: d["fixtures"][0]["calls"][0].pop("watched"), "a call lacks watched",
                                                       fixtures=[fx(recorders=[recorder()], calls=[call()])])
    yield "reader-refuses-a-pointee-the-recorder-does-not-read", bad("ptr", lambda d: None, "has pointee '1'",
                                                                     fixtures=[fx(recorders=[recorder(arguments=2)], calls=[call(args=["0x80000100", "0x80000100"], pointees={"1": []})])])
    yield "reader-refuses-a-change-beyond-its-block", bad("span", lambda d: None, "does not lie inside the 4 bytes",
                                                           fixtures=[fx(recorders=[recorder(arguments=1, pointees={"0": 1})], calls=[call(args=["0x80000100"], pointees={"0": [[3, "0102"]]})])])
    yield "reader-refuses-watched-blocks-that-do-not-match-watch", bad("nwatch", lambda d: None, "has 0 watched blocks, the fixture watches 1",
                                                                      fixtures=[fx(recorders=[recorder()], watch=[["0x80000100", 1]], calls=[call()])])
    yield "reader-refuses-a-boolean-for-ends-in-callee", bad("bool", lambda d: d["fixtures"][0].update(ends_in_callee=1), "ends_in_callee must be true or false")
    yield "reader-reads-a-format-2-file-without-reading-its-fixtures", same(
        (lambda p: (p.write_text(json.dumps(doc("f", [{"anything": 1}], format=2))), hf.load_fixtures(p, "f").format)[1])(rig.root / "f2.fixtures.json"), 2)
    yield "reader-takes-the-recorder-options", same(
        (lambda r: (r.masks, r.pointees, r.stores, r.counts, r.ends_at))(rig.load([fx(recorders=[recorder(arguments=2, masks={"0": 65535}, pointees={"1": 2},
                                                                                                        stores=[[2, "0x1f800004", "0x00000001"]], counts=["0x801ac310"], ends_run_at=3)])]).fixtures[0].recorders[0]),
        ({0: 65535}, {1: 2}, [(2, 0x1F800004, 1)], [0x801AC310], 3))


# ---------------------------------------------------------------------------------------------------------------
# From a fixture to a case.


def case_cases(rig: Rig):
    f = rig.load([fx(args=["0x80000100", "0x00000005"], reads=[["0x80000100", "0102"]], same=[["0x80000102", "03"], ["0x80000100", "0a"]],
                     writes=[["0x80000101", "11"], ["0x80000104", "2200"], ["0x1f800010", "ff"]], watch=[["0x80000300", 2]],
                     recorders=[recorder(arguments=2, masks={"1": 255}, pointees={"0": 1}, stores=[[1, "0x80000400", "0x00000009"]], counts=["0x80000404"], ends_run_at=2),
                                recorder("stub_b", "0x80100010", 1)],
                     calls=[call(args=["0x1", "0x2"], returned="0x00000003", watched=[[]]), call("stub_b", ["0x4"], "0x00000005", watched=[[]]), call(args=["0x6", "0x7"], returned="0x00000008", watched=[[]])])]).fixtures[0]
    case = hf.build_case(f)
    yield "case-args-are-the-fixtures-words-then-the-stack-words", same(
        case["args"], [0x80000100, 5, 0, 0, 0xA5A5A5A5, 0xA5A5A5A5, 0xA5A5A5A5, 0xA5A5A5A5])
    # The fifth and later arguments: what the fixture states as read at 0x801ff010 and upward. Two bytes stated of
    # the fifth (a halfword argument), the whole sixth, one byte of the eighth (stated by `same`); a read just
    # below, at 0x801ff00c, is no argument.
    stack = hf.build_case(rig.load([fx(args=["0x1", "0x2", "0x3", "0x4"], reads=[["0x801ff010", "3955"], ["0x801ff014", "78563412"]],
                                       same=[["0x801ff01f", "7f"]])]).fixtures[0])
    yield "case-the-fifth-to-eighth-arguments-are-the-words-above-the-home-area", same(
        stack["args"], [1, 2, 3, 4, 0xA5A55539, 0x12345678, 0xA5A5A5A5, 0x7FA5A5A5])
    yield "case-memory-is-reads-then-same-then-flipped-writes", same(
        case["memory"], [("bytes", 0x1F800010, bytes([0x00])), ("bytes", 0x80000100, bytes([0x0A, 0x02, 0x03])), ("bytes", 0x80000104, bytes([0xDD, 0xFF]))])
    yield "case-watch-is-the-fixtures", same(case["watch"], [(0x80000300, 2)])
    yield "case-stub-options-pass-through", same(
        {k: case["stubs"][0][k] for k in ("at", "arguments", "masks", "pointees", "stores", "counts", "ends_at")},
        {"at": 0x80100000, "arguments": 2, "masks": {1: 255}, "pointees": {0: 1}, "stores": [(1, 0x80000400, 9)], "counts": [0x80000404], "ends_at": 2})
    yield "case-returns-are-the-callees-values-in-order", same((case["stubs"][0]["returns"], case["stubs"][1]["returns"]), ([3, 8], [5]))
    yield "case-has-no-zeroed-stack-and-no-zeroed-arena", same([m for m in case["memory"] if m[0] == "fill"], [])
    yield "case-takes-a-recorder-with-no-calls", same(rig.load([fx(recorders=[recorder()])]).fixtures[0].recorders[0].callee, "stub_a")
    yield "case-adjacent-bytes-are-one-run", same(
        hf.build_case(rig.load([fx(reads=[["0x80000100", "01"], ["0x80000101", "02"]], writes=[["0x80000102", "00"]])]).fixtures[0])["memory"],
        [("bytes", 0x80000100, bytes([1, 2, 0xFF]))])
    yield "case-the-harness-takes-the-case", same(hr.normalize_case(case)["args"][:2], [0x80000100, 5])


# ---------------------------------------------------------------------------------------------------------------
# The comparison on results written by hand.


INT = hf.ReturnType("int", "word")


def returned(result=0, calls=(), changes=None) -> dict:
    return {"end": "returned", "result": result, "calls": list(calls), "changes": changes or {}}


def stub_call(at=0x80100000, args=(), stack=(), pointees=None, watched=()) -> dict:
    return {"at": at, "args": list(args), "stack_args": list(stack), "pointees": pointees or {}, "watched": list(watched)}


def compare_cases(rig: Rig):
    def cmp(fixture, result, ret=INT):
        return hf.compare(rig.load([fixture]).fixtures[0], result, ret)

    yield "compare-passes-an-equal-result", same(cmp(fx(result="0x00000007"), returned(7)), ([], 0))
    yield "compare-a-null-result-is-not-compared", same(cmp(fx(), returned(7)), ([], 0))
    yield "compare-a-wrong-result", same(cmp(fx(result="0x00000008"), returned(7))[0],
                                         ["result: the PC build gives 0x00000007, the original gave 0x00000008"])
    yield "compare-the-upper-bits-of-a-word-result", same(cmp(fx(result="0x80000007"), returned(7))[0],
                                                           ["result: the PC build gives 0x00000007, the original gave 0x80000007"])
    u8 = hf.ReturnType("u8", "bits", 8)
    s16 = hf.ReturnType("s16", "bits", 16)
    yield "compare-u8-ignores-bits-8-and-up", same(cmp(fx(result="0xffffff78"), returned(0x78), u8)[0], [])
    yield "compare-u8-the-other-way", same(cmp(fx(result="0x00000078"), returned(0x12345678), u8)[0], [])
    yield "compare-u8-low-8-bits-differ", same(cmp(fx(result="0x00000079"), returned(0x78), u8)[0],
                                                ["result: the PC build gives 0x00000078, the original gave 0x00000079 (compared in the low 8 bits, the type is `u8`)"])
    yield "compare-u8-bit-7-counts", same(cmp(fx(result="0x00000078"), returned(0xF8), u8)[0],
                                          ["result: the PC build gives 0x000000f8, the original gave 0x00000078 (compared in the low 8 bits, the type is `u8`)"])
    yield "compare-s16-ignores-bits-16-and-up", same(cmp(fx(result="0x12340007"), returned(0xFFFF0007), s16)[0], [])
    yield "compare-s16-bit-8-counts", same(cmp(fx(result="0x00000007"), returned(0x0107), s16)[0],
                                           ["result: the PC build gives 0x00000107, the original gave 0x00000007 (compared in the low 16 bits, the type is `s16`)"])
    yield "compare-void-with-a-result", same(cmp(fx(result="0x00000005"), returned(5), hf.ReturnType("void", "void"))[0],
                                              ["the C returns nothing, the original's result was 0x00000005"])
    yield "compare-void-with-no-result", same(cmp(fx(), returned(5), hf.ReturnType("void", "void"))[0], [])
    yield "compare-a-result-of-a-run-that-ends-in-a-callee-is-not-compared", same(
        cmp(fx(result="0x00000007", ends_in_callee=True, recorders=[recorder()]), {"end": "stub", "calls": [], "changes": {}})[0], [])

    yield "compare-fault-read", same(cmp(fx(), {"end": "fault", "fault": {"kind": "read", "address": 0, "pc": 0x401000}, "calls": [], "changes": {}})[0],
                                     ["the PC build faults: read at 0x00000000 from the instruction at 0x00401000"])
    yield "compare-fault-write", same(cmp(fx(), {"end": "fault", "fault": {"kind": "write", "address": 4, "pc": 0x401000}, "calls": [], "changes": {}})[0],
                                      ["the PC build faults: write at 0x00000004 from the instruction at 0x00401000"])
    yield "compare-fault-exec", same(cmp(fx(), {"end": "fault", "fault": {"kind": "exec", "address": 0x80000800, "pc": 0x80000800}, "calls": [], "changes": {}})[0],
                                     ["the PC build faults: it called 0x80000800, where no stand-in stands"])
    yield "compare-fault-other", same(cmp(fx(), {"end": "fault", "fault": {"kind": "other", "address": 0xC0000094, "pc": 0x401000}, "calls": [], "changes": {}})[0],
                                      ["the PC build faults: exception 0xc0000094 at the instruction 0x00401000"])
    yield "compare-fault-calls", same(cmp(fx(), {"end": "fault", "fault": {"kind": "calls", "address": 0, "pc": 0}, "calls": [], "changes": {}})[0],
                                      ["the PC build faults: it called the stand-ins more than 100000 times in one case"])
    yield "compare-a-fault-hides-the-other-differences", same(
        len(cmp(fx(result="0x00000001", writes=[["0x80000100", "01"]]), {"end": "fault", "fault": {"kind": "read", "address": 0, "pc": 1}, "calls": [], "changes": {}})[0]), 1)
    yield "compare-a-wrong-way-to-end-hides-the-other-differences", same(
        len(cmp(fx(result="0x00000001", recorders=[recorder()], calls=[call()], writes=[["0x80000100", "01"]]), {"end": "stub", "calls": [], "changes": {}})[0]), 1)
    yield "compare-died", same(cmp(fx(), {"end": "died", "status": 9, "calls": [], "changes": {}})[0], ["the PC build dies: the process ended with status 9"])
    yield "compare-timeout", same(cmp(fx(), {"end": "timeout", "calls": [], "changes": {}})[0], ["the PC build does not return: no answer within the time allowed"])
    yield "compare-returned-where-the-original-ended-in-a-callee", same(
        cmp(fx(ends_in_callee=True, recorders=[recorder()], calls=[call()]), returned(0, [stub_call()]))[0],
        ["the PC build returned, the original ended inside a callee that does not return"])
    yield "compare-ended-in-a-callee-where-the-original-returned", same(
        cmp(fx(recorders=[recorder()], calls=[call()]), {"end": "stub", "calls": [stub_call()], "changes": {}})[0],
        ["the PC build ended inside a callee that stops the run, the original returned"])
    yield "compare-ended-in-a-callee-as-the-original-did", same(
        cmp(fx(ends_in_callee=True, recorders=[recorder()], calls=[call()]), {"end": "stub", "calls": [stub_call()], "changes": {}}), ([], 0))

    one = dict(recorders=[recorder(arguments=2)], calls=[call(args=["0x00000001", "0x00000002"])])
    yield "compare-equal-calls-pass", same(cmp(fx(**one), returned(0, [stub_call(args=[1, 2])])), ([], 0))
    yield "compare-a-missing-call", same(cmp(fx(**one), returned())[0], ["call 1: missing, the original called stub_a"])
    yield "compare-an-extra-call", same(cmp(fx(recorders=[recorder(arguments=2)]), returned(0, [stub_call(args=[1, 2])]))[0],
                                        ["call 1: extra, the PC build called stub_a"])
    two = dict(recorders=[recorder(arguments=1), recorder("stub_b", "0x80100010", 1)], calls=[call(args=["0x1"]), call("stub_b", ["0x2"])])
    yield "compare-the-second-call-missing", same(cmp(fx(**two), returned(0, [stub_call(args=[1])]))[0], ["call 2: missing, the original called stub_b"])
    yield "compare-a-call-of-another-callee", same(
        cmp(fx(**two), returned(0, [stub_call(args=[1]), stub_call(0x80100000, [2])]))[0], ["call 2: the PC build called stub_a, the original called stub_b"])
    yield "compare-the-callee-is-named-by-the-recorder-at-its-address", same(
        cmp(fx(**two), returned(0, [stub_call(args=[1]), stub_call(0x80100010, [2])])), ([], 0))
    yield "compare-a-wrong-argument", same(cmp(fx(**one), returned(0, [stub_call(args=[1, 3])]))[0],
                                           ["call 1 to stub_a: argument 2 is 0x00000003, the original gave 0x00000002"])
    yield "compare-the-first-wrong-argument-is-the-one-named", same(cmp(fx(**one), returned(0, [stub_call(args=[9, 8])]))[0],
                                                                    ["call 1 to stub_a: argument 1 is 0x00000009, the original gave 0x00000001"])
    yield "compare-calls-then-result-then-writes-in-that-order", same(
        cmp(fx(result="0x00000001", writes=[["0x80000100", "01"]], **one), returned(0, [stub_call(args=[9, 2])]))[0],
        ["call 1 to stub_a: argument 1 is 0x00000009, the original gave 0x00000001", "result: the PC build gives 0x00000000, the original gave 0x00000001",
         "missing write at 0x80000100: the PC build leaves 0xfe, the original left 0x01"])

    def stack(value):
        return cmp(fx(recorders=[recorder(arguments=1)], calls=[call(args=[value])]), returned(0, [stub_call(args=[0], stack=[0])]))

    yield "stack-argument-at-the-bottom-of-the-region-is-equal", same(stack("0x801fe000"), ([], 1))
    yield "stack-argument-in-the-region-is-equal", same(stack("0x801fe800"), ([], 1))
    yield "stack-argument-at-the-last-address-is-equal", same(stack("0x801ff00f"), ([], 1))
    yield "stack-argument-at-the-end-of-the-region-is-a-difference", same(
        stack("0x801ff010"), (["call 1 to stub_a: argument 1 is an address of the PC's stack, the original gave 0x801ff010"], 0))
    yield "stack-argument-below-the-region-is-a-difference", same(
        stack("0x801fdfff"), (["call 1 to stub_a: argument 1 is an address of the PC's stack, the original gave 0x801fdfff"], 0))
    yield "stack-argument-counts-only-those-taken-as-equal", same(
        cmp(fx(recorders=[recorder(arguments=2)], calls=[call(args=["0x801fe800", "0x801fe900"])]), returned(0, [stub_call(args=[0, 0], stack=[0, 1])]))[1], 2)
    yield "an-address-of-the-pc-stack-in-a-plain-argument-is-just-a-value", same(
        cmp(fx(recorders=[recorder(arguments=1)], calls=[call(args=["0x801fe800"])]), returned(0, [stub_call(args=[0x801FE800])])), ([], 0))
    yield "a-plain-argument-that-differs-in-the-stack-region-is-a-difference", same(
        cmp(fx(recorders=[recorder(arguments=1)], calls=[call(args=["0x801fe800"])]), returned(0, [stub_call(args=[0x801FE804])]))[0],
        ["call 1 to stub_a: argument 1 is 0x801fe804, the original gave 0x801fe800"])

    # The words behind a pointer argument and the watched blocks: the case's input memory with the changes over it.
    ptr = dict(reads=[["0x80000700", "01000000"]], recorders=[recorder(arguments=1, pointees={"0": 1})])

    def block(changes, seen, arg="0x80000700"):
        return cmp(fx(calls=[call(args=[arg], pointees={"0": changes})], **ptr), returned(0, [stub_call(args=[int(arg, 16)], pointees={0: seen})]))[0]

    yield "block-equal-passes", same(block([[0, "05"]], bytes([5, 0, 0, 0])), [])
    yield "block-expected-starts-from-the-input-memory", same(block([], bytes([1, 0, 0, 0])), [])
    yield "block-a-wrong-byte-0", same(block([[0, "04"]], bytes([5, 0, 0, 0])),
                                       ["call 1 to stub_a: argument 1 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x04)"])
    yield "block-no-change-listed-means-the-input-value", same(block([], bytes([5, 0, 0, 0])),
                                                               ["call 1 to stub_a: argument 1 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x01)"])
    yield "block-the-change-offset-counts", same(block([[2, "07"]], bytes([1, 0, 0, 0])),
                                                 ["call 1 to stub_a: argument 1 differs from byte 2 (the PC build has 0x00 there at this call, the original had 0x07)"])
    yield "block-unstated-input-is-0xA5", same(
        cmp(fx(recorders=[recorder(arguments=1, pointees={"0": 1})], calls=[call(args=["0x80000800"], pointees={"0": [[1, "00"]]})]),
            returned(0, [stub_call(args=[0x80000800], pointees={0: bytes([0xA5, 0, 0xA5, 0xA5])})]))[0], [])
    # Behind a stack argument lies a local: the original's is zero with the fixture's changes over it, and every
    # byte is compared, the ones the fixture's changes do not name too (they were zero in the original).
    behind = lambda changes, seen: cmp(fx(recorders=[recorder(arguments=1, pointees={"0": 1})], calls=[call(args=["0x801fe800"], pointees={"0": changes})]),
                                       returned(0, [stub_call(args=[0], stack=[0], pointees={0: seen})]))
    yield "block-behind-the-stack-argument-equal-passes", same(behind([[0, "77"]], bytes([0x77, 0, 0, 0])), ([], 1))
    yield "block-behind-the-stack-argument-a-byte-the-changes-do-not-name-is-compared", same(
        behind([[0, "77"]], bytes([0x77, 0x01, 0, 0]))[0],
        ["call 1 to stub_a: argument 1 differs from byte 1 (the PC build has 0x01 there at this call, the original had 0x00)"])
    yield "block-behind-the-stack-argument-no-change-means-zero", same(
        (behind([], bytes([0, 0, 0, 0]))[0], behind([], bytes([0, 1, 0, 0]))[0]),
        ([], ["call 1 to stub_a: argument 1 differs from byte 1 (the PC build has 0x01 there at this call, the original had 0x00)"]))
    yield "block-behind-the-stack-argument-a-changed-byte-is-compared", same(
        cmp(fx(recorders=[recorder(arguments=1, pointees={"0": 1})], calls=[call(args=["0x801fe800"], pointees={"0": [[0, "78"]]})]),
            returned(0, [stub_call(args=[0], stack=[0], pointees={0: bytes([0x77, 0, 0, 0])})]))[0],
        ["call 1 to stub_a: argument 1 differs from byte 0 (the PC build has 0x77 there at this call, the original had 0x78)"])
    w = dict(reads=[["0x80000700", "01000000"]], watch=[["0x80000700", 1]], recorders=[recorder(arguments=0)])
    yield "watched-block-equal-passes", same(cmp(fx(calls=[call(watched=[[[0, "05"]]])], **w), returned(0, [stub_call(watched=[bytes([5, 0, 0, 0])])]))[0], [])
    yield "watched-block-wrong", same(cmp(fx(calls=[call(watched=[[[0, "06"]]])], **w), returned(0, [stub_call(watched=[bytes([5, 0, 0, 0])])]))[0],
                                      ["call 1 to stub_a: watched block 1 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x06)"])
    yield "watched-block-second-call-is-looked-at", same(
        cmp(fx(calls=[call(watched=[[]]), call(watched=[[[3, "09"]]])], **w), returned(0, [stub_call(watched=[bytes([1, 0, 0, 0])]), stub_call(watched=[bytes([1, 0, 0, 0])])]))[0],
        ["call 2 to stub_a: watched block 1 differs from byte 3 (the PC build has 0x00 there at this call, the original had 0x09)"])
    yield "watched-block-not-logged", same(
        cmp(fx(calls=[call(watched=[[]])], **w), returned(0, [stub_call(watched=[])]))[0], ["call 1 to stub_a: the PC build logged nothing for watched block 1"])
    yield "a-call-difference-hides-the-block-check", same(
        cmp(fx(calls=[call(args=["0x1"], watched=[[[0, "06"]]])], **{**w, "recorders": [recorder(arguments=1)]}),
            returned(0, [stub_call(args=[2], watched=[bytes([5, 0, 0, 0])])]))[0],
        ["call 1 to stub_a: argument 1 is 0x00000002, the original gave 0x00000001"])

    # The writes, exactly.
    yield "writes-equal-pass", same(cmp(fx(writes=[["0x80000100", "0102"]]), returned(0, changes={0x80000100: 1, 0x80000101: 2}))[0], [])
    yield "writes-a-missing-write-leaves-the-flipped-start", same(cmp(fx(writes=[["0x80000100", "01"]]), returned())[0],
                                                                  ["missing write at 0x80000100: the PC build leaves 0xfe, the original left 0x01"])
    yield "writes-a-missing-write-of-a-stated-byte-leaves-its-read-value", same(
        cmp(fx(reads=[["0x80000100", "07"]], writes=[["0x80000100", "01"]]), returned())[0],
        ["missing write at 0x80000100: the PC build leaves 0x07, the original left 0x01"])
    yield "writes-a-missing-write-in-the-scratchpad", same(cmp(fx(writes=[["0x1f800010", "ff"]]), returned())[0],
                                                           ["missing write at 0x1f800010: the PC build leaves 0x00, the original left 0xff"])
    yield "writes-a-wrong-value", same(cmp(fx(writes=[["0x80000100", "06"]]), returned(0, changes={0x80000100: 5}))[0],
                                       ["wrong value at 0x80000100: the PC build leaves 0x05, the original left 0x06"])
    yield "writes-an-extra-write", same(cmp(fx(), returned(0, changes={0x80000102: 2}))[0],
                                        ["extra write at 0x80000102: the PC build left 0x02, the original wrote nothing there"])
    yield "writes-lines-are-in-address-order-all-kinds", same(
        cmp(fx(writes=[["0x80000100", "01"], ["0x80000104", "09"]]), returned(0, changes={0x80000104: 8, 0x80000102: 2}))[0],
        ["missing write at 0x80000100: the PC build leaves 0xfe, the original left 0x01", "extra write at 0x80000102: the PC build left 0x02, the original wrote nothing there",
         "wrong value at 0x80000104: the PC build leaves 0x08, the original left 0x09"])
    yield "writes-nothing-written-and-nothing-expected-passes", same(cmp(fx(), returned()), ([], 0))


# ---------------------------------------------------------------------------------------------------------------
# The command, on programs built for real.


def run_cases(rig: Rig):
    # A result.
    status, out, err = rig.replay("add", [fx(args=["0x00000003", "0x00000004"], result="0x00000007"),
                                          fx(args=["0x00000003", "0x00000004"], result="0x00000008", case=3, note="eight"),
                                          fx(args=["0x00000003", "0x00000004"], result="0x80000007", case=4, note="high bit")])
    yield "run-a-wrong-result-and-the-order-of-lines", same(out, [
        "add native replay: fixtures 3, passed 1, failed 2",
        fixture_head(1, 3, "eight"), "    result: the PC build gives 0x00000007, the original gave 0x00000008",
        fixture_head(2, 4, "high bit"), "    result: the PC build gives 0x00000007, the original gave 0x80000007",
        counts(1, 0, 1, 0, 0)])
    yield "run-status-1-for-a-failed-fixture", same((status, err), (1, ""))
    # A local handed to a callee, as real programs: the original's local was 0 (no change against the zeroed stack) or
    # 119 (one changed byte). A C whose local is 256, or 375, differs in a byte that the fixture's changes do not name.
    def local_fixture(changes):
        return fx(recorders=[recorder(arguments=1, pointees={"0": 1})], calls=[call(args=["0x801fefe8"], pointees={"0": changes})], result="0x00000000")

    passes = lambda name, changes: rig.replay(name, [local_fixture(changes)])[1]
    yield "run-a-zero-local-passes-and-256-does-not", same((passes("loc0", []), passes("loc256", [])), (
        ["loc0 native replay: fixtures 1, passed 1, failed 0, stack arguments not compared 1", counts(1, 1, 0, 0, 0)],
        ["loc256 native replay: fixtures 1, passed 0, failed 1, stack arguments not compared 1", fixture_head(0),
         "    call 1 to stub_a: argument 1 differs from byte 1 (the PC build has 0x01 there at this call, the original had 0x00)",
         counts(1, 0, 1, 0, 0)]))
    yield "run-a-local-of-119-passes-and-375-does-not", same((passes("loc119", [[0, "77"]]), passes("loc375", [[0, "77"]])), (
        ["loc119 native replay: fixtures 1, passed 1, failed 0, stack arguments not compared 1", counts(1, 1, 0, 0, 0)],
        ["loc375 native replay: fixtures 1, passed 0, failed 1, stack arguments not compared 1", fixture_head(0),
         "    call 1 to stub_a: argument 1 differs from byte 1 (the PC build has 0x01 there at this call, the original had 0x00)",
         counts(1, 0, 1, 0, 0)]))
    # A local the function never stores is 0 on the console's zeroed stack, and here: the harness zeroes its stack.
    yield "run-a-local-never-stored-is-zero-as-on-the-console", same(passes("locnone", []), [
        "locnone native replay: fixtures 1, passed 1, failed 0, stack arguments not compared 1", counts(1, 1, 0, 0, 0)])
    # The fifth argument (a halfword here) and the sixth come from the words the fixture states above the home area:
    # 1 + 2 + 3 + 4 + 0x0020 * 16 + 0x100 = 0x30a. With another fifth word in the fixture the same C gives another sum.
    status, out, err = rig.replay("five", [
        fx(args=["0x1", "0x2", "0x3", "0x4"], reads=[["0x801ff010", "2000"], ["0x801ff014", "00010000"]], result="0x0000030a"),
        fx(args=["0x1", "0x2", "0x3", "0x4"], reads=[["0x801ff010", "2100"], ["0x801ff014", "00010000"]], result="0x0000030a", case=1, note="other fifth")])
    yield "run-the-fifth-and-sixth-arguments-reach-the-function", same(out, [
        "five native replay: fixtures 2, passed 1, failed 1",
        fixture_head(1, 1, "other fifth"), "    result: the PC build gives 0x0000031a, the original gave 0x0000030a",
        counts(1, 0, 1, 0, 0)])
    status, out, err = rig.replay("add", [fx(args=["0x00000003", "0x00000004"], result="0x00000007")])
    yield "run-a-pass", same((status, out, err), (0, ["add native replay: fixtures 1, passed 1, failed 0", counts(1, 1, 0, 0, 0)], ""))

    status, out, _ = rig.replay("u8f", [fx(args=["0x00000078"], result="0xffffff78"), fx(args=["0x00000078"], result="0x00000079", case=1)])
    yield "run-u8-result-compared-in-8-bits", same(out[:4], [
        "u8f native replay: fixtures 2, passed 1, failed 1", fixture_head(1, 1),
        "    result: the PC build gives 0x00000078, the original gave 0x00000079 (compared in the low 8 bits, the type is `unsigned char`)", counts(1, 0, 1, 0, 0)])
    status, out, _ = rig.replay("s16f", [fx(args=["0x00000007"], result="0xabcd0007"), fx(args=["0x00000007"], result="0x00010008", case=1)])
    yield "run-short-result-compared-in-16-bits", same(out[:3], [
        "s16f native replay: fixtures 2, passed 1, failed 1", fixture_head(1, 1),
        "    result: the PC build gives 0x00000007, the original gave 0x00010008 (compared in the low 16 bits, the type is `short`)"])
    status, out, _ = rig.replay("voidf", [fx(args=["0x80000100"], result="0x00000005", writes=[["0x80000100", "01000000"]])])
    yield "run-void-with-a-result", same(out[:3], [
        "voidf native replay: fixtures 1, passed 0, failed 1", fixture_head(0), "    the C returns nothing, the original's result was 0x00000005"])
    status, out, _ = rig.replay("voidf", [fx(args=["0x80000100"], writes=[["0x80000100", "01000000"]])])
    yield "run-void-with-no-result-passes", same(status, 0)

    # Writes.
    p = "0x80000100"
    status, out, _ = rig.replay("st", [
        fx(args=[p, "0x00000001"], writes=[[p, "01000000"]]),
        fx(args=[p, "0x00000005"], writes=[[p, "06000000"]], case=1, note="wrong"),
        fx(args=[p, "0x01020304"], writes=[[p, "0403"]], case=2, note="extra"),
        fx(args=[p, "0x00000001"], reads=[[p, "07000000"]], writes=[[p, "01"]], case=3, note="stated"),
        fx(args=[p, "0x04030201"], case=4, note="four extra")])
    yield "run-st-writes-wrong-extra-and-limit-of-three-lines", same(out, [
        "st native replay: fixtures 5, passed 2, failed 3",
        fixture_head(1, 1, "wrong"), "    wrong value at 0x80000100: the PC build leaves 0x05, the original left 0x06",
        fixture_head(2, 2, "extra"), "    extra write at 0x80000102: the PC build left 0x02, the original wrote nothing there",
        "    extra write at 0x80000103: the PC build left 0x01, the original wrote nothing there",
        fixture_head(4, 4, "four extra"),
        "    extra write at 0x80000100: the PC build left 0x01, the original wrote nothing there",
        "    extra write at 0x80000101: the PC build left 0x02, the original wrote nothing there",
        "    extra write at 0x80000102: the PC build left 0x03, the original wrote nothing there",
        counts(1, 0, 1, 0, 0)])
    status, out, _ = rig.replay("nostore", [
        fx(args=[p], writes=[[p, "01"]], note="one"), fx(args=[p], writes=[[p, "ff"]], case=1, note="ff"),
        fx(args=[p], reads=[[p, "07"]], writes=[[p, "01"]], case=2, note="read"), fx(args=["0x1f800010"], writes=[["0x1f800010", "01"]], case=3, note="scratch"),
        fx(args=[p], case=4, note="nothing")])
    yield "run-a-c-that-does-not-store-fails-whatever-the-value", same(out, [
        "nostore native replay: fixtures 5, passed 1, failed 4",
        fixture_head(0, 0, "one"), "    missing write at 0x80000100: the PC build leaves 0xfe, the original left 0x01",
        fixture_head(1, 1, "ff"), "    missing write at 0x80000100: the PC build leaves 0x00, the original left 0xff",
        fixture_head(2, 2, "read"), "    missing write at 0x80000100: the PC build leaves 0x07, the original left 0x01",
        fixture_head(3, 3, "scratch"), "    missing write at 0x1f800010: the PC build leaves 0xfe, the original left 0x01",
        counts(1, 0, 1, 0, 0)])

    # Calls.
    base = dict(args=["0x80000700"], reads=[["0x80000700", "01000000"]], writes=[["0x80000700", "06"]], result="0x00000000")
    first = call(args=["0x12345678", "0x80000700", "0x00000007"])
    rec = recorder(arguments=3)
    status, out, _ = rig.replay("c1", [
        fx(recorders=[rec], calls=[first], **base),
        fx(recorders=[rec], calls=[call(args=["0x12345679", "0x80000700", "0x00000007"])], case=1, note="arg", **base),
        fx(recorders=[rec], calls=[first, first], case=2, note="missing", **base),
        fx(recorders=[rec], calls=[], case=3, note="extra", **base),
        fx(recorders=[rec, recorder("stub_b", "0x80100010", 3)], calls=[call("stub_b", ["0x12345678", "0x80000700", "0x00000007"])], case=4, note="other", **base),
        fx(recorders=[recorder(arguments=3, pointees={"1": 1})], calls=[call(args=first["args"], pointees={"1": [[0, "05"]]})], case=5, note="word ok", **base),
        fx(recorders=[recorder(arguments=3, pointees={"1": 1})], calls=[call(args=first["args"], pointees={"1": [[0, "04"]]})], case=6, note="word wrong", **base),
        fx(recorders=[recorder(arguments=3, pointees={"1": 1})], calls=[call(args=first["args"], pointees={"1": []})], case=7, note="word input", **base),
        fx(recorders=[rec], watch=[["0x80000700", 1]], calls=[call(args=first["args"], watched=[[[0, "05"]]])], case=8, note="watched ok", **base),
        fx(recorders=[rec], watch=[["0x80000700", 1]], calls=[call(args=first["args"], watched=[[[0, "06"]]])], case=9, note="watched wrong", **base),
        fx(recorders=[rec], watch=[["0x80000700", 1]], calls=[call(args=first["args"], watched=[[[0, "05"], [2, "05"]]])], case=10, note="watched byte 2", **base),
        fx(recorders=[rec], calls=[call(args=["0x1", "0x80000700", "0x7"])], case=11, note="two", **{**base, "result": "0x00000001"})])
    yield "run-calls-and-blocks-of-a-c-that-calls", same(out, [
        "c1 native replay: fixtures 12, passed 3, failed 9",
        fixture_head(1, 1, "arg"), "    call 1 to stub_a: argument 1 is 0x12345678, the original gave 0x12345679",
        fixture_head(2, 2, "missing"), "    call 2: missing, the original called stub_a",
        fixture_head(3, 3, "extra"), "    call 1: extra, the PC build called stub_a",
        fixture_head(4, 4, "other"), "    call 1: the PC build called stub_a, the original called stub_b",
        fixture_head(6, 6, "word wrong"), "    call 1 to stub_a: argument 2 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x04)",
        fixture_head(7, 7, "word input"), "    call 1 to stub_a: argument 2 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x01)",
        fixture_head(9, 9, "watched wrong"), "    call 1 to stub_a: watched block 1 differs from byte 0 (the PC build has 0x05 there at this call, the original had 0x06)",
        fixture_head(10, 10, "watched byte 2"), "    call 1 to stub_a: watched block 1 differs from byte 2 (the PC build has 0x00 there at this call, the original had 0x05)",
        fixture_head(11, 11, "two"),
        "    call 1 to stub_a: argument 1 is 0x12345678, the original gave 0x00000001",
        "    result: the PC build gives 0x00000000, the original gave 0x00000001",
        counts(1, 0, 1, 0, 0)])

    # Returned values in order.
    r = recorder(arguments=0)
    status, out, _ = rig.replay("ret4", [
        fx(recorders=[r], calls=[call(returned="0x00000001"), call(returned="0x00000002"), call(returned="0x00000003"), call(returned="0x00000004")], result="0x000004d2"),
        fx(recorders=[r], calls=[call(returned="0x00000002"), call(returned="0x00000001"), call(returned="0x00000003"), call(returned="0x00000004")], result="0x000004d2", case=1, note="order")])
    yield "run-returned-values-come-in-the-order-of-the-calls", same(out[:3], [
        "ret4 native replay: fixtures 2, passed 1, failed 1", fixture_head(1, 1, "order"), "    result: the PC build gives 0x00000856, the original gave 0x000004d2"])

    # A run that ends in a callee.
    ends = recorder(arguments=0, ends_run_at=2)
    status, out, _ = rig.replay("ends", [
        fx(recorders=[ends], calls=[call(), call()], ends_in_callee=True),
        fx(recorders=[ends], calls=[call(), call()], ends_in_callee=False, case=1, note="wrong way"),
        fx(recorders=[r], calls=[call(), call()], ends_in_callee=True, case=2, note="returns anyway"),
        fx(recorders=[ends], calls=[call(), call()], ends_in_callee=True, case=3, note="stored after", writes=[["0x80190000", "63"]])])
    yield "run-ends-in-callee", same(out, [
        "ends native replay: fixtures 4, passed 1, failed 3",
        fixture_head(1, 1, "wrong way"), "    the PC build ended inside a callee that stops the run, the original returned",
        fixture_head(2, 2, "returns anyway"), "    the PC build returned, the original ended inside a callee that does not return",
        fixture_head(3, 3, "stored after"), "    missing write at 0x80190000: the PC build leaves 0x9c, the original left 0x63",
        counts(1, 0, 1, 0, 0)])

    # Stack arguments.
    sr = recorder(arguments=2)
    mk = lambda value, case, note: fx(args=["0x80000700"], recorders=[sr], calls=[call(args=[value, "0x80000700"])], result="0x00000077", case=case, note=note)
    status, out, _ = rig.replay("stk", [mk("0x801fe800", 0, "in"), mk("0x801fe000", 1, "low edge"), mk("0x801ff00f", 2, "high edge"),
                                        mk("0x801ff010", 3, "above"), mk("0x801fdfff", 4, "below"),
                                        fx(args=["0x80000700"], recorders=[recorder(arguments=2, pointees={"0": 1})], calls=[call(args=["0x801fe800", "0x80000700"], pointees={"0": [[0, "77"]]})], result="0x00000077", case=5, note="local"),
                                        fx(args=["0x80000700"], recorders=[recorder(arguments=2, pointees={"0": 1})], calls=[call(args=["0x801fe800", "0x80000700"], pointees={"0": [[0, "78"]]})], result="0x00000077", case=6, note="local wrong")])
    yield "run-stack-arguments", same(out, [
        "stk native replay: fixtures 7, passed 4, failed 3, stack arguments not compared 5",
        fixture_head(3, 3, "above"), "    call 1 to stub_a: argument 1 is an address of the PC's stack, the original gave 0x801ff010",
        fixture_head(4, 4, "below"), "    call 1 to stub_a: argument 1 is an address of the PC's stack, the original gave 0x801fdfff",
        fixture_head(6, 6, "local wrong"), "    call 1 to stub_a: argument 1 differs from byte 0 (the PC build has 0x77 there at this call, the original had 0x78)",
        counts(1, 0, 1, 0, 0)])

    # Faults and the rest of the ways a run ends.
    folder = rig.folder({"fault": SOURCES["fault"]}, {"fault": doc("fault", [
        fx(args=["0x00000001", "0x00000000", "0x00000000"], result="0x0000002a", case=1, note="read 0"),
        fx(args=["0x00000002", "0x00000000", "0x00000000"], case=2, note="write 0"),
        fx(args=["0x00000003", "0x80000800", "0x00000000"], case=3, note="call"),
        fx(args=["0x00000004", "0x00000000", "0x00000000"], case=4, note="exit"),
        fx(args=["0x00000005", "0x00000000", "0x00000000"], case=5, note="spin"),
        fx(args=["0x00000006", "0x00000000", "0x00000000"], case=6, note="divide"),
        fx(args=["0x00000008", "0x00000000", "0x00000000"], case=8, note="write then read 0", writes=[["0x80000900", "01000000"]]),
        fx(args=["0x00000000", "0x00000000", "0x00000000"], result="0x0000002a", case=0, note="fine")])})
    status, out, _ = rig.run([folder], timeout=2, build=rig.root / "faultbuild")
    exe = rig.root / "faultbuild" / "fault" / "fault.exe"
    pc = {m: hr.run_cases(exe, [{"args": [m, 0, 0]}], 5)[0]["fault"]["pc"] for m in (1, 2, 6)}
    yield "run-faults-and-deaths", same(out, [
        "fault native replay: fixtures 8, passed 1, failed 7",
        fixture_head(0, 1, "read 0"), f"    the PC build faults: read at 0x00000000 from the instruction at {pc[1]:#010x}",
        fixture_head(1, 2, "write 0"), f"    the PC build faults: write at 0x00000000 from the instruction at {pc[2]:#010x}",
        fixture_head(2, 3, "call"), "    the PC build faults: it called 0x80000800, where no stand-in stands",
        fixture_head(3, 4, "exit"), "    the PC build dies: the process ended with status 9",
        fixture_head(4, 5, "spin"), "    the PC build does not return: no answer within the time allowed",
        fixture_head(5, 6, "divide"), f"    the PC build faults: exception 0xc0000094 at the instruction {pc[6]:#010x}",
        fixture_head(6, 8, "write then read 0"), "    the PC build faults: read at 0x00000000 from the instruction at " + f"{hr.run_cases(exe, [{'args': [8, 0, 0]}], 5)[0]['fault']['pc']:#010x}",
        counts(1, 0, 1, 0, 0)])

    # A note of more than 70 characters is cut to 67 and three dots.
    long_note = "n" * 71
    status, out, _ = rig.replay("add", [fx(args=["0x00000001", "0x00000001"], result="0x00000009", note=long_note), fx(args=["0x00000001", "0x00000001"], result="0x00000009", note="m" * 70, case=1)])
    yield "run-a-long-note-is-cut", same(out[1:4], [fixture_head(0, 0, "n" * 67 + "..."), "    result: the PC build gives 0x00000002, the original gave 0x00000009", fixture_head(1, 1, "m" * 70)])


def not_built_cases(rig: Rig):
    status, out, err = rig.replay("bad", [fx(result="0x00000001")])
    yield "not-built-a-c-that-does-not-compile", same((len(out), out[0].startswith("bad native replay: not built: "), "bad.c" in out[0], out[1], status),
                                                      (2, True, True, counts(0, 0, 0, 1, 0), 1))
    status, out, _ = rig.replay("nodef", [fx()])
    yield "not-built-no-definition-says-the-return-type-cannot-be-read", same(
        (out[0], status), ("nodef native replay: not built: nodef.c: no definition of nodef with a return type in its file; its return type cannot be read", 1))
    status, out, _ = rig.replay("nw", [fx(result="0x00000001")])
    yield "not-built-a-name-with-no-address", same((out[0].startswith("nw native replay: not built: link: no PS1 address, and nothing in the link provides: nowhere"), len(out), status), (True, 2, 1))
    folder = rig.folder({}, {"nofile": doc("nofile", [fx()])})
    status, out, _ = rig.run([folder])
    yield "not-built-a-missing-c-file", same((out[0].startswith("nofile native replay: not built: cannot read nofile.c:"), out[1], status), (True, counts(0, 0, 0, 1, 0), 1))


def evidence_cases(rig: Rig):
    folder = rig.folder({"add": SOURCES["add"]}, {"add": doc("add", [fx(args=["0x1", "0x2"], result="0x00000003"), fx(args=["0x1", "0x2"], result="0x00000003")])})
    real = hr.run_cases
    hr.run_cases = lambda exe, cases, timeout, prefix=(): real(exe, cases, timeout, prefix)[:1]
    try:
        status, out, _ = rig.run([folder], jobs=1)
    finally:
        hr.run_cases = real
    yield "a-missing-result-is-not-a-pass", same((status, out), (1, ["add native replay: not built: the harness gave 1 results for 2 cases", counts(0, 0, 0, 1, 0)]))
    argv = ["--config", str(rig.src / "build.toml"), "--cc", str(rig.root / "no" / "gcc"), "--build", str(rig.root / "ccbuild2"), "--folder", str(folder),
            "--overrides", str(rig.root / "no_overrides")]
    o = io.StringIO()
    with contextlib.redirect_stdout(o):
        status = hf.main(argv)
    lines = o.getvalue().splitlines()
    yield "a-compiler-that-is-not-there-is-not-built", same((status, lines[0].startswith("add native replay: not built: cannot run compiler "), lines[1]), (1, True, counts(0, 0, 0, 1, 0)))


def refused_cases(rig: Rig):
    status, out, _ = rig.replay("add", [fx(recorders=[recorder(arguments=17)], result="0x00000000")])
    yield "not-built-cases-the-harness-refuses", same((status, out), (1, [
        "add native replay: not built: the harness refuses the cases: the program refused the cases: more than 16 arguments in: stub 80100000 11 0", counts(0, 0, 0, 1, 0)]))


def format_and_input_cases(rig: Rig):
    f2 = doc("fmt2", [fx(), fx()], format=2)
    folder = rig.folder({"fmt2": SOURCES["add"], "add": SOURCES["add"]}, {"fmt2": f2, "add": doc("add", [fx(args=["0x1", "0x2"], result="0x00000003")])})
    status, out, err = rig.run([folder])
    yield "format-2-is-reported-and-counted-apart", same(out, [
        "add native replay: fixtures 1, passed 1, failed 0",
        "fmt2 native replay: not replayed: its fixtures hold calls of original callees (format 2)", counts(1, 1, 0, 0, 1)])
    yield "format-2-alone-is-status-0", same(rig.run([rig.folder({"fmt2": SOURCES["add"]}, {"fmt2": f2})])[0], 0)
    yield "format-2-does-not-build-the-c", same(rig.run([rig.folder({}, {"fmt2": f2})])[1], [
        "fmt2 native replay: not replayed: its fixtures hold calls of original callees (format 2)", counts(0, 0, 0, 0, 1)])

    ok = doc("ok", [fx(args=["0x1", "0x2"], result="0x00000003")])
    bad_docs = {
        "notjson": ("{", "INPUT ERROR: notjson.fixtures.json is not JSON:"),
        "fmt3": (doc("fmt3", [fx()], format=3), "INPUT ERROR: fmt3.fixtures.json: format 3, this tool reads format 1\n"),
        "other": (doc("zzz", [fx()]), "INPUT ERROR: other.fixtures.json: the file is for 'zzz', not other\n"),
        "nowrites": (doc("nowrites", [fx(), {k: v for k, v in fx().items() if k != "writes"}]), "INPUT ERROR: nowrites.fixtures.json: fixture 1: it lacks writes\n"),
        "empty": (doc("empty", []), "INPUT ERROR: empty.fixtures.json: no fixtures\n"),
    }
    for name, (value, wanted) in bad_docs.items():
        folder = rig.folder({"ok": SOURCES["add"], name: SOURCES["add"]}, {"ok": ok, name: value})
        status, out, err = rig.run([folder])
        yield f"input-{name}-is-status-2-and-nothing-is-run", same((status, out, err.startswith(wanted)), (2, [], True))
    status, out, err = rig.run([rig.folder({"add": SOURCES["add"]}, {"add": doc("add", [fx()])})], names=["nosuch"])
    yield "input-a-name-with-no-file", same((status, out, err.startswith("INPUT ERROR: nosuch: no fixtures file nosuch.fixtures.json in t_nonmatching")), (2, [], True))
    status, out, err = rig.run([rig.folder({}, {})])
    yield "input-no-fixtures-file-at-all", same((status, out, err.startswith("INPUT ERROR: no fixtures file in t_nonmatching")), (2, [], True))
    status, out, err = rig.run([rig.folder({"add": SOURCES["add"]}, {"add": doc("add", [fx(args=["0x1", "0x2"], result="0x00000003")])}),
                                rig.folder({"add": SOURCES["add"]}, {"add": doc("add", [fx()])})])
    yield "input-the-same-function-in-two-folders", same((status, out, err.startswith("INPUT ERROR: add: fixtures files in two folders")), (2, [], True))
    out, err = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
        status = hf.main(["--config", str(rig.root / "none.toml"), "--folder", str(rig.root), "--cc", rig.cc])
    yield "input-a-missing-config", same((status, out.getvalue(), err.getvalue().startswith("INPUT ERROR: ")), (2, "", True))
    yield "named-functions-select-from-the-folder", same(
        rig.run([rig.folder({"add": SOURCES["add"], "st": SOURCES["st"]}, {"add": doc("add", [fx(args=["0x1", "0x2"], result="0x00000003")]), "st": doc("st", [fx()])})], names=["add"])[1],
        ["add native replay: fixtures 1, passed 1, failed 0", counts(1, 1, 0, 0, 0)])
    yield "status-0-when-nothing-failed-and-all-built", same(rig.replay("add", [fx(args=["0x1", "0x2"], result="0x00000003")])[0], 0)
    yield "status-1-for-not-built-alone", same(rig.replay("bad", [fx()])[0], 1)


def override_cases(rig: Rig):
    ov = rig.root / "ovr"
    ov.mkdir()
    (ov / "one.c").write_text(SOURCES["two"].replace("two", "one"))
    folder = rig.folder({"one": SOURCES["one"], "two": SOURCES["two"]},
                        {"one": doc("one", [fx(args=["0x1"], result="0x00000002")]), "two": doc("two", [fx(args=["0x1"], result="0x00000002")])})
    status, out, _ = rig.run([folder], overrides=ov)
    yield "override-replaces-the-c-and-is-said", same(out, [
        "one native replay: the PC program runs the override one.c of ovr; it is replayed in place of the nonmatching C",
        "one native replay: fixtures 1, passed 1, failed 0", "two native replay: fixtures 1, passed 1, failed 0", counts(2, 2, 0, 0, 0)])
    status, out, _ = rig.run([folder], overrides=rig.root / "no_overrides")
    yield "without-the-override-the-nonmatching-c-fails", same(out[:3], [
        "one native replay: fixtures 1, passed 0, failed 1", fixture_head(0), "    result: the PC build gives 0x00000001, the original gave 0x00000002"])
    # A function of the overrides folder itself is replayed from its own file, with no note.
    ofolder = rig.root / "ovr2"
    ofolder.mkdir()
    (ofolder / "one.c").write_text(SOURCES["two"].replace("two", "one"))
    (ofolder / "one.fixtures.json").write_text(json.dumps(doc("one", [fx(args=["0x1"], result="0x00000002")])))
    status, out, _ = rig.run([ofolder], overrides=ofolder)
    yield "a-function-of-the-overrides-folder-is-replayed-without-a-note", same(out, ["one native replay: fixtures 1, passed 1, failed 0", counts(1, 1, 0, 0, 0)])
    yield "override-of-a-named-function-only", same(rig.run([folder], overrides=ov, names=["two"])[1], ["two native replay: fixtures 1, passed 1, failed 0", counts(1, 1, 0, 0, 0)])


def order_cases(rig: Rig):
    good = lambda n: doc(n, [fx(args=["0x1", "0x2"], result="0x00000003")])
    spin = doc("a_slow", [fx(result="0x00000001")])
    folder = rig.folder({"a_slow": SOURCES["spin"].replace("spin", "a_slow"), "b_add": SOURCES["add"].replace("add", "b_add"), "c_add": SOURCES["add"].replace("add", "c_add")},
                        {"a_slow": spin, "b_add": good("b_add"), "c_add": good("c_add")})
    wanted = ["a_slow native replay: fixtures 1, passed 0, failed 1", fixture_head(0), "    the PC build does not return: no answer within the time allowed",
              "b_add native replay: fixtures 1, passed 1, failed 0", "c_add native replay: fixtures 1, passed 1, failed 0", counts(3, 2, 1, 0, 0)]
    for jobs in (1, 4):
        status, out, _ = rig.run([folder], jobs=jobs, timeout=2)
        yield f"order-lines-come-in-the-order-of-the-names-with-{jobs}-jobs", same((status, out), (1, wanted))


def folder_cases(rig: Rig):
    cfg = rig.root / "cfgdir" / "build.toml"
    for name in ("slot_nonmatching", "library_nonmatching", "a_nonmatching", "plain"):
        (cfg.parent / name).mkdir(parents=True, exist_ok=True)
    (cfg.parent / "afile_nonmatching").write_text("")
    yield "default-folders-are-the-nonmatching-ones-but-the-library-and-the-overrides", same(
        hf.default_folders(cfg), [cfg.parent / "a_nonmatching", cfg.parent / "slot_nonmatching", hf.REPO / "port" / "overrides"])


def groups(rig: Rig):
    yield return_type_cases()
    yield reader_cases(rig)
    yield case_cases(rig)
    yield compare_cases(rig)
    yield folder_cases(rig)
    yield run_cases(rig)
    yield not_built_cases(rig)
    yield evidence_cases(rig)
    yield refused_cases(rig)
    yield format_and_input_cases(rig)
    yield override_cases(rig)
    yield order_cases(rig)


def start_check(cc: str, work: Path) -> str | None:
    import subprocess
    probe = work / "probe.c"
    probe.write_text("int main(void) { return 7; }\n")
    out = work / "probe.exe"
    proc = subprocess.run([cc, "-o", str(out), str(probe)], capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        return "the compiler cannot build a program: " + (proc.stderr.strip().splitlines() or ["?"])[0]
    try:
        ran = subprocess.run([str(out)], capture_output=True, timeout=60)
    except (OSError, subprocess.TimeoutExpired) as err:
        return f"cannot start a Windows program: {err}"
    return None if ran.returncode == 7 else f"cannot start a Windows program (status {ran.returncode})"


def main() -> int:
    parser = argparse.ArgumentParser(description="Tests for hostfixtures.py.")
    parser.add_argument("--cc", required=True, help="the cross compiler")
    args = parser.parse_args()
    started = time.monotonic()
    passed = failed = 0
    with tempfile.TemporaryDirectory(prefix="hostfixtures-test-") as tmp:
        work = Path(tmp)
        problem = start_check(args.cc, work)
        if problem:
            print(problem)
            return 2
        rig = Rig(work, args.cc)
        for produced in groups(rig):
            while True:
                try:
                    name, detail = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a test must report, not crash
                    print(f"FAIL the test itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                if detail is None:
                    print(f"ok   {name}")
                    passed += 1
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    seconds = time.monotonic() - started
    print(f"{failed} of {passed + failed} tests FAILED in {seconds:.1f} seconds" if failed else f"all {passed} tests passed in {seconds:.1f} seconds")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
