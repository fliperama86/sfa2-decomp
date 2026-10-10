#!/usr/bin/env python3
"""Tests for hostreplay.py and the harness port/replay/harness.c, on made-up C functions.

Every function here is made up and written by this file into a temporary tree with a made-up configuration and
symbol file; nothing of the game, no disc image and no PS1 compiler is needed. Each program is built for real
with the cross compiler CC (a compile and link of one function takes about a second) and started directly, as
the other port tests start theirs. Every expected value is written out by hand below, never computed by a copy
of the code under test.

    python3 test_hostreplay.py --cc CC [--run PREFIX]

PREFIX is a command prefix that starts a Windows program, as in test_hostbuild.py; without it the program is
started directly. The last line says how many tests passed and how long they took.
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
import tempfile
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import hostreplay as hr  # noqa: E402

A5 = 0xA5A5A5A5

CONFIG = """[types]
fields = "t.fields"
header = "t.h"

[[unit]]
name = "ua"
source = "a.c"
functions = [
  { name = "stub_a", address = 0x80100000, size = 16 },
  { name = "stub_b", address = 0x80100010, size = 16 },
  { name = "stub_edge", address = 0x80000ffe, size = 16 },
]
"""
SYMBOLS = "data_word = 0x80190000;\n/* a comment */\nscratch_word = 0x1f800010;\n"

SOURCES = {
    "calc": "int calc(int a, int b, int c, int d, int e, int f, int g, int h)\n"
            "{ return (h << 28) | (g << 24) | (f << 20) | (e << 16) | (d << 12) | (c << 8) | (b << 4) | a; }\n",
    "mem": "unsigned mem(unsigned *p, unsigned v) { unsigned old = *p; *p = v; return old; }\n",
    "call1": "extern int stub_a(int a, int b, int c);\n"
             "int call1(int *p) { *p = 5; stub_a(0x12345678, (int)p, 7); *p = 6; return 0; }\n",
    "ret4": "extern int stub_a(void);\n"
            "int ret4(void) { int a = stub_a(); int b = stub_a(); int c = stub_a(); int d = stub_a();\n"
            "  return ((a * 10 + b) * 10 + c) * 10 + d; }\n",
    "store3": "extern int stub_a(void);\nextern int data_word;\n"
              "int store3(int *out) { int i; for (i = 0; i < 3; i++) { stub_a(); out[i] = data_word; } return 0; }\n",
    "ends": "extern int stub_a(void);\nextern int data_word;\n"
            "int ends(void) { stub_a(); stub_a(); data_word = 99; return 1; }\n",
    "rd": "extern int data_word;\nextern int scratch_word;\n"
          "int rd(int which) { return which ? scratch_word : data_word; }\n",
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
             "  case 7: { volatile char big[4096]; big[0] = 1; return fault(7, addr, c) + big[0]; }\n"
             "  case 8: *(volatile int *)0x80000900 = 1; return *(volatile int *)0;\n"
             "  }\n  return 0;\n}\n",
    "stk": "extern int stub_a(int *a, int *b);\n"
           "int stk(int *q) { int local = 0x77; stub_a(&local, q); return local; }\n",
    "al": "int table_c[2] = { 5, 6 };\nint al(int i) { return table_c[i]; }\n",
    "edge": "extern int stub_edge(int a);\nint edge(void) { return stub_edge(3) + 1; }\n",
    "spin": "extern int stub_a(void);\nint spin(void) { for (;;) stub_a(); }\n",
    "withhdr": '#include "val.h"\nint withhdr(void) { return VAL; }\n',
    "other": "int other(void) { return 1; }\n",
    "bad": "int bad(void) { return 1 }\n",
    "nw": "extern int nowhere(void);\nint nw(void) { return nowhere(); }\n",
}


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def raises(fn, *needles):
    """None when fn raises a ReplayError whose text has every needle."""
    try:
        fn()
    except hr.ReplayError as err:
        missing = [n for n in needles if n not in str(err)]
        return None if not missing else f"the error {str(err)!r} does not name {missing}"
    return "no error was raised"


class Rig:
    def __init__(self, root: Path, cc: str, prefix: list[str]):
        self.root, self.cc, self.prefix = root, cc, prefix
        self.src = root / "src"
        self.src.mkdir()
        (self.src / "build.toml").write_text(CONFIG)
        (self.src / "t.fields").write_text("struct S size=0x4\n0x000 u32 a\n")
        (self.src / "symbols.ld").write_text(SYMBOLS)
        (self.src / "a.c").write_text("int x;\n")
        self.build_dir = root / "build"

    def source(self, name: str, text: str | None = None) -> Path:
        path = self.src / f"{name}.c"
        wanted = SOURCES[name] if text is None else text
        if not path.is_file() or path.read_text() != wanted:   # a file is touched only when it changes
            path.write_text(wanted)
        return path

    def program(self, name: str, text: str | None = None) -> Path:
        return hr.build_program(self.source(name, text), name, self.src / "build.toml", self.build_dir, self.cc, 120)

    def run(self, exe: Path, cases: list[dict], timeout: float = 20):
        return hr.run_cases(exe, cases, timeout, self.prefix)

    def one(self, name: str, case: dict, timeout: float = 20) -> dict:
        return self.run(self.program(name), [case], timeout)[0]

    def impl_address(self, exe: Path, name: str) -> int:
        out = subprocess.run([hr.hb.default_nm(self.cc), str(exe)], capture_output=True, text=True, timeout=60).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 3 and parts[2] in (f"_impl_{name}", f"impl_{name}"):
                return int(parts[0], 16)
        raise AssertionError(f"impl_{name} is not in the program")


def small(result: dict) -> dict:
    """The result without the calls and the changes."""
    return {k: v for k, v in result.items() if k not in ("calls", "changes")}


def le(address: int, value: int, length: int = 4) -> dict[int, int]:
    """Not a copy of the code under test: the bytes of a little-endian word, written to a dictionary."""
    return {address + i: (value >> (8 * i)) & 0xFF for i in range(length)}


# ---------------------------------------------------------------------------------------------------------------


def argument_and_memory_cases(rig: Rig):
    exe = rig.program("calc")
    got = rig.run(exe, [{"args": [1, 2, 3, 4, 5, 6, 7, 8]}, {"args": [1, 2, 3]}, {"args": [0xFFFFFFFF, 0]}, {"args": [-1]}])
    yield "eight-words-reach-the-function", same([r["result"] for r in got], [0x87654321, 0x00000321, 0xFFFFFFFF, 0xFFFFFFFF])
    yield "the-end-of-a-plain-run-is-returned-with-no-calls-and-no-changes", same([(r["end"], r["calls"], r["changes"]) for r in got], [("returned", [], {})] * 4)

    exe = rig.program("mem")
    got = rig.run(exe, [
        {"args": [0x80000300, 7]},
        {"args": [0x80000300, 9]},
        {"memory": [("bytes", 0x80000320, bytes([1, 2, 3, 4]))], "args": [0x80000320, 0]},
        {"memory": [("fill", 0x80000400, 8, 0x11)], "args": [0x80000404, 0]},
        {"memory": [("fill", 0x80000500, 8, 0x22), ("bytes", 0x80000502, bytes([0xFF, 0xEE]))], "args": [0x80000500, 0]},
        {"memory": [("bytes", 0x80000600, bytes([1, 0, 0, 0]))], "args": [0x80000600, 1]},
        {"args": [0x80000700, A5]},
        {"args": [0x1F800008, 0xA5]},
        {"args": [0x1F800FFC, 0x01020304]},
        {"args": [0x801FFFFC, 0x0A0B0C0D]},
    ])
    yield "unstated-memory-reads-0xA5", same(got[0]["result"], A5)
    yield "memory-is-fresh-in-each-case", same((got[0]["result"], got[1]["result"]), (A5, A5))
    yield "a-store-shows-as-the-four-changed-bytes", same(got[0]["changes"], {0x80000300: 7, 0x80000301: 0, 0x80000302: 0, 0x80000303: 0})
    yield "bytes-are-put-in-the-order-given", same(got[2]["result"], 0x04030201)
    yield "fill-puts-the-byte-over-the-length", same(got[3]["result"], 0x11111111)
    yield "memory-entries-apply-in-order", same(got[4]["result"], 0xEEFF2222)
    yield "a-store-of-the-value-already-there-is-no-change", same((got[5]["changes"], got[6]["changes"]), ({}, {}))
    yield "only-the-bytes-that-differ-show-in-the-scratchpad", same(got[7]["changes"], {0x1F800009: 0, 0x1F80000A: 0, 0x1F80000B: 0})
    yield "the-last-word-of-the-scratchpad-page-is-memory", same(got[8]["changes"], {0x1F800FFC: 4, 0x1F800FFD: 3, 0x1F800FFE: 2, 0x1F800FFF: 1})
    yield "the-last-word-of-ram-is-memory", same(got[9]["changes"], {0x801FFFFC: 0x0D, 0x801FFFFD: 0x0C, 0x801FFFFE: 0x0B, 0x801FFFFF: 0x0A})
    yield "a-store-of-a-word-shows-as-four-bytes-in-little-endian-order", same(
        rig.run(exe, [{"args": [0x80000010, 0x11223344]}])[0]["changes"], {0x80000010: 0x44, 0x80000011: 0x33, 0x80000012: 0x22, 0x80000013: 0x11})

    exe = rig.program("rd")
    got = rig.run(exe, [
        {"memory": [("bytes", 0x80190000, bytes([0x34, 0x12, 0, 0]))], "args": [0]},
        {"memory": [("bytes", 0x1F800010, bytes([0x78, 0x56, 0, 0]))], "args": [1]},
        {"args": [0]},
    ])
    yield "a-data-name-of-the-symbol-file-reads-ram-at-its-address", same(got[0]["result"], 0x1234)
    yield "a-data-name-in-the-scratchpad-reads-the-scratchpad", same(got[1]["result"], 0x5678)
    yield "a-data-name-reads-0xA5-in-a-fresh-case", same(got[2]["result"], A5)

    exe = rig.program("al")
    got = rig.run(exe, [{"args": [1]}, {"args": [0]}])
    yield "data-defined-in-the-c-is-the-hosts-copy", same([r["result"] for r in got], [6, 5])


def stub_cases(rig: Rig):
    exe = rig.program("call1")
    p = 0x80000700
    got = rig.run(exe, [{
        "args": [p], "memory": [("bytes", p + 4, bytes([0xEF, 0xBE, 0, 0]))],
        "stubs": [{"at": 0x80100000, "arguments": 3, "masks": {0: 0xFFFF}, "pointees": {1: 2}}],
        "watch": [(p, 1)],
    }])[0]
    yield "a-stub-logs-the-address-and-the-masked-arguments", same(
        [(c["at"], c["args"], c["stack_args"]) for c in got["calls"]], [(0x80100000, [0x5678, p, 7], [])])
    yield "a-stub-logs-the-words-behind-a-pointer-argument", same([c["pointees"] for c in got["calls"]], [{1: bytes([5, 0, 0, 0, 0xEF, 0xBE, 0, 0])}])
    yield "a-watched-block-shows-a-store-made-before-the-call-and-not-one-made-after", same(
        [c["watched"] for c in got["calls"]], [[bytes([5, 0, 0, 0])]])
    yield "the-memory-after-the-call-shows-the-store-after-it-and-no-stub-bytes", same(
        (got["end"], got["result"], got["changes"]), ("returned", 0, {p: 6, p + 1: 0, p + 2: 0, p + 3: 0}))

    exe = rig.program("ret4")
    got = rig.run(exe, [
        {"stubs": [{"at": 0x80100000, "arguments": 0, "returns": [1, 2]}]},
        {"stubs": [{"at": 0x80100000, "arguments": 0}]},
        {"stubs": [{"at": 0x80100000, "arguments": 0, "returns": [7]}]},
        {"stubs": [{"at": 0x80100000, "arguments": 0, "returns": [1, 2, 3, 4, 5]}]},
    ])
    yield "returns-come-in-order-and-the-last-repeats", same(got[0]["result"], 1222)
    yield "a-stub-with-no-returns-returns-0", same(got[1]["result"], 0)
    yield "one-return-is-repeated", same(got[2]["result"], 7777)
    yield "returns-left-over-are-not-used", same(got[3]["result"], 1234)
    yield "every-call-of-a-stub-is-logged-in-order", same([len(r["calls"]) for r in got], [4, 4, 4, 4])

    exe = rig.program("store3")
    out = 0x80000800
    got = rig.run(exe, [{
        "args": [out], "memory": [("bytes", 0x80190100, bytes([0x0A, 0, 0, 0]))],
        "stubs": [{"at": 0x80100000, "arguments": 0, "stores": [(2, 0x80190000, 0x55)], "counts": [0x80190100]}],
        "watch": [(0x80190000, 1)],
    }])[0]
    want = {**le(out + 4, 0x55), **le(out + 8, 0x55), **le(0x80190000, 0x55), 0x80190100: 0x0D}
    yield "a-store-happens-at-its-call-and-a-count-counts-every-call", same(got["changes"], want)
    yield "a-store-and-a-count-happen-after-the-call-is-logged", same(
        [c["watched"] for c in got["calls"]], [[bytes([0xA5] * 4)], [bytes([0xA5] * 4)], [bytes([0x55, 0, 0, 0])]])

    exe = rig.program("ends")
    got = rig.run(exe, [
        {"stubs": [{"at": 0x80100000, "arguments": 0, "ends_at": 2}]},
        {"stubs": [{"at": 0x80100000, "arguments": 0, "ends_at": 3}]},
        {"stubs": [{"at": 0x80100000, "arguments": 0}]},
    ])
    yield "ends-at-ends-the-run-at-that-call", same((got[0]["end"], len(got[0]["calls"]), "result" in got[0]), ("stub", 2, False))
    yield "code-after-the-ending-call-does-not-run", same(got[0]["changes"], {})
    yield "ends-at-beyond-the-calls-made-lets-the-function-return", same((got[1]["end"], got[1]["result"], got[1]["changes"]), ("returned", 1, le(0x80190000, 99)))
    yield "no-ends-at-lets-the-function-return", same((got[2]["end"], got[2]["result"]), ("returned", 1))

    exe = rig.program("stk")
    q = 0x80000A00
    got = rig.run(exe, [
        {"args": [q], "memory": [("bytes", q, bytes([0x11, 0, 0, 0]))], "stubs": [{"at": 0x80100000, "arguments": 2, "pointees": {0: 1, 1: 1}}]},
        {"args": [q], "stubs": [{"at": 0x80100000, "arguments": 2, "masks": {0: 0xFF}}]},
        {"args": [0], "stubs": [{"at": 0x80100000, "arguments": 2, "pointees": {1: 1}}]},
    ])
    yield "the-address-of-a-local-is-logged-as-0-with-a-mark", same(
        [(c["args"], c["stack_args"]) for c in got[0]["calls"]], [([0, q], [0])])
    yield "a-pointee-behind-an-address-of-a-local-is-still-read", same(
        [c["pointees"] for c in got[0]["calls"]], [{0: bytes([0x77, 0, 0, 0]), 1: bytes([0x11, 0, 0, 0])}])
    yield "the-function-returns-after-a-stub-that-got-a-local", same(got[0]["result"], 0x77)
    yield "a-mask-does-not-change-the-mark-of-an-address-of-a-local", same([(c["args"], c["stack_args"]) for c in got[1]["calls"]], [([0, q], [0])])
    yield "a-pointee-that-cannot-be-read-is-a-fault-of-the-case-with-no-half-entry", same(
        (got[2]["end"], got[2]["fault"]["kind"], got[2]["fault"]["address"], got[2]["calls"]), ("fault", "read", 0, []))

    exe = rig.program("edge")
    got = rig.run(exe, [{"stubs": [{"at": 0x80000FFE, "arguments": 1, "returns": [41]}]}])[0]
    yield "a-stub-across-a-page-boundary-works", same((got["end"], got["result"], [(c["at"], c["args"]) for c in got["calls"]]), ("returned", 42, [(0x80000FFE, [3])]))
    yield "the-stubs-bytes-across-two-pages-are-not-changes", same(got["changes"], {})

    exe = rig.program("spin")
    got = rig.run(exe, [{"stubs": [{"at": 0x80100000, "arguments": 0}]}])[0]
    yield "a-function-that-calls-a-stub-for-ever-ends-as-a-fault-of-calls", same(
        (got["end"], got["fault"]["kind"], len(got["calls"])), ("fault", "calls", 100000))


def fault_cases(rig: Rig):
    exe = rig.program("fault")
    code = rig.impl_address(exe, "fault")
    stub_here = {"at": 0x80001000, "arguments": 0, "returns": [5]}
    got = rig.run(exe, [
        {"args": [1, 0]},                                       # 0 read of address 0
        {"args": [0]},                                          # 1 the next case runs
        {"args": [2, 0x90000000]},                              # 2 write to an unmapped address
        {"args": [3, 0x80001000], "stubs": [stub_here]},        # 3 a stub at an address in RAM is called
        {"args": [3, 0x80001000]},                              # 4 the same address without a stub
        {"args": [3, 0x90000000]},                              # 5 outside RAM
        {"args": [3, 0x1F800100]},                              # 6 the scratchpad has no code
        {"args": [1, 0x80200000]},                              # 7 the first address past RAM
        {"args": [8]},                                          # 8 a write, then a read of 0
        {"args": [1, 0x80000900]},                              # 9 memory is fresh after a fault
        {"args": [6, 0, 0]},                                    # 10 division by zero
        {"args": [7, 0]},                                       # 11 stack overflow
        {"args": [7, 0]},                                       # 12 and again
        {"args": [0]},                                          # 13 still runs
    ])
    fk = lambda r: (r["end"], r["fault"]["kind"], r["fault"]["address"]) if r["end"] == "fault" else (r["end"], None, None)  # noqa: E731
    yield "a-read-of-address-0-is-a-fault-of-kind-read", same(fk(got[0]), ("fault", "read", 0))
    yield "the-pc-of-a-read-fault-is-inside-the-function", same(code <= got[0]["fault"]["pc"] < code + 0x100, True)
    yield "the-next-case-of-the-process-runs-after-a-fault", same((got[1]["end"], got[1]["result"]), ("returned", 42))
    yield "a-write-to-an-unmapped-address-is-a-fault-of-kind-write", same(fk(got[2]), ("fault", "write", 0x90000000))
    yield "the-pc-of-a-write-fault-is-inside-the-function", same(code <= got[2]["fault"]["pc"] < code + 0x100, True)
    yield "a-stub-is-called-and-the-function-returns", same((got[3]["end"], got[3]["result"], len(got[3]["calls"])), ("returned", 6, 1))
    yield "a-call-of-an-address-in-ram-without-a-stub-is-a-fault-of-kind-exec", same(fk(got[4]), ("fault", "exec", 0x80001000))
    yield "pages-have-no-execute-permission-again-in-the-next-case", same(got[4]["end"], "fault")
    yield "an-exec-fault-has-the-address-as-the-pc", same(got[4]["fault"]["pc"], 0x80001000)
    yield "a-call-outside-ram-is-a-fault-of-kind-exec", same(fk(got[5]), ("fault", "exec", 0x90000000))
    yield "a-call-into-the-scratchpad-is-a-fault-of-kind-exec", same(fk(got[6]), ("fault", "exec", 0x1F800100))
    yield "a-read-at-the-first-address-past-ram-is-a-fault", same(fk(got[7]), ("fault", "read", 0x80200000))
    yield "a-fault-result-still-lists-the-changes-made-before-it", same((fk(got[8]), got[8]["changes"]), (("fault", "read", 0), le(0x80000900, 1)))
    yield "memory-after-a-fault-is-fresh-in-the-next-case", same(got[9]["result"], A5)
    yield "division-by-zero-is-a-fault-of-kind-other-with-the-code", same(fk(got[10]), ("fault", "other", 0xC0000094))
    yield "a-stack-overflow-is-a-fault-of-kind-other-with-the-code", same(fk(got[11]), ("fault", "other", 0xC00000FD))
    yield "a-second-stack-overflow-in-the-same-process-is-caught-too", same(fk(got[12]), ("fault", "other", 0xC00000FD))
    yield "the-case-after-a-stack-overflow-runs", same((got[13]["end"], got[13]["result"]), ("returned", 42))

    got = rig.run(exe, [{"args": [4]}, {"args": [0]}, {"args": [5]}, {"args": [0]}, {"args": [4]}, {"args": [4]}], timeout=3)
    yield "a-function-that-ends-the-process-is-died-with-its-status", same(small(got[0]), {"end": "died", "status": 9})
    yield "the-case-after-a-death-runs-in-a-new-process", same((got[1]["end"], got[1]["result"]), ("returned", 42))
    yield "a-function-that-never-returns-is-a-timeout", same(small(got[2]), {"end": "timeout"})
    yield "the-case-after-a-timeout-runs-in-a-new-process", same((got[3]["end"], got[3]["result"]), ("returned", 42))
    yield "two-deaths-in-a-row-are-two-results", same([small(got[4]), small(got[5])], [{"end": "died", "status": 9}] * 2)
    yield "a-died-or-timeout-result-has-no-calls-and-no-changes", same([(r["calls"], r["changes"]) for r in (got[0], got[2])], [([], {})] * 2)
    yield "one-result-per-case", same(len(got), 6)


def build_cases(rig: Rig):
    yield "a-compile-error-raises-with-the-first-error-line", raises(lambda: rig.program("bad"), "bad.c", ": error:", "';'")
    yield "a-name-without-an-address-is-an-error-naming-it", raises(lambda: rig.program("nw"), "nowhere", "no PS1 address")
    yield "a-source-that-does-not-define-the-name-is-an-error", raises(
        lambda: hr.build_program(rig.source("other"), "missing", rig.src / "build.toml", rig.build_dir, rig.cc, 120), "does not define missing")
    yield "a-name-that-is-not-a-c-name-is-an-error", raises(
        lambda: hr.build_program(rig.source("other"), "a-b", rig.src / "build.toml", rig.build_dir, rig.cc, 120), "not a C name")
    yield "a-missing-source-is-an-error", raises(
        lambda: hr.build_program(rig.src / "nofile.c", "nofile", rig.src / "build.toml", rig.build_dir, rig.cc, 120), "nofile.c")

    exe = rig.program("other")
    first = exe.stat().st_mtime_ns
    exe2 = rig.program("other")
    yield "a-program-newer-than-its-inputs-is-not-built-again", same((exe2, exe2.stat().st_mtime_ns), (exe, first))

    t = first + 2 * 10**9
    (rig.src / "other.c").write_text("int other(void) { return 3; }\n")
    os.utime(rig.src / "other.c", ns=(t, t))
    exe = hr.build_program(rig.src / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "a-changed-source-is-built-again", same(rig.run(exe, [{}])[0]["result"], 3)

    (rig.src / "val.h").write_text("#define VAL 5\n")
    exe = rig.program("withhdr")
    before = rig.run(exe, [{}])[0]["result"]
    t = exe.stat().st_mtime_ns
    (rig.src / "val.h").write_text("#define VAL 6\n")
    os.utime(rig.src / "val.h", ns=(t + 5 * 10**9, t + 5 * 10**9))
    exe = hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "a-changed-header-is-built-again", same((before, rig.run(exe, [{}])[0]["result"]), (5, 6))

    # The header goes away. The compiler's listing of what the source reads then fails, and a failed listing is not an
    # empty one: the program on disk (which still answers 6) is not handed back; the build is made and names the header.
    (rig.src / "val.h").unlink()
    yield "a-missing-header-is-an-error-and-not-the-program-on-disk", raises(
        lambda: hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120), "val.h")
    (rig.src / "val.h").write_text("#define VAL 7\n")
    exe = hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "the-header-back-with-another-value-is-built", same(rig.run(exe, [{}])[0]["result"], 7)

    # The listing does not end (a made-up compiler run that gives no answer for it): the program is built all the same.
    again = hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    quiet, mtime = again.stat().st_mtime_ns == exe.stat().st_mtime_ns, exe.stat().st_mtime_ns
    real, listings = hr.hb.hostcheck.compile_run, []

    def no_end_for_the_listing(argv, timeout):
        if "-M" in argv:
            listings.append(argv)
            return None
        return real(argv, timeout)

    hr.hb.hostcheck.compile_run = no_end_for_the_listing
    try:
        exe = hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    finally:
        hr.hb.hostcheck.compile_run = real
    yield "a-listing-that-does-not-end-means-a-build-not-the-program-on-disk", same(
        (quiet, len(listings), exe.stat().st_mtime_ns != mtime, rig.run(exe, [{}])[0]["result"]), (True, 1, True, 7))

    # Another source file for the same name, older than the program on disk: the program is what it was made from, not
    # only when; the other file's program answers 9. And without the note of what it was made from it is built again.
    exe = rig.program("other")
    alt = rig.root / "alt"
    alt.mkdir()
    (alt / "other.c").write_text("int other(void) { return 9; }\n")
    old = exe.stat().st_mtime_ns - 60 * 10**9
    os.utime(alt / "other.c", ns=(old, old))
    exe = hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "another-older-source-for-the-same-name-is-built-not-reused", same(rig.run(exe, [{}])[0]["result"], 9)
    mtime = exe.stat().st_mtime_ns
    again = hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    quiet = again.stat().st_mtime_ns == mtime
    (rig.build_dir / "gen" / "other.from").unlink()
    again = hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "a-program-without-the-note-of-its-origin-is-built-again", same((quiet, again.stat().st_mtime_ns != mtime), (True, True))
    yield "the-note-of-origin-names-the-source-the-configuration-and-the-compiler", same(
        (rig.build_dir / "gen" / "other.from").read_text().splitlines(),
        [str((alt / "other.c").resolve()), str((rig.src / "build.toml").resolve()), rig.cc])

    # A build that fails takes the note away: the program still on disk is from an earlier build, and after the source
    # is put back with an old time nothing says any more that it is this source's program, so it is built.
    good = "int other(void) { return 9; }\n"
    exe = hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    mtime = exe.stat().st_mtime_ns
    (alt / "other.c").write_text("int other(void) { return 9 }\n")
    os.utime(alt / "other.c", ns=(mtime + 5 * 10**9, mtime + 5 * 10**9))
    failed = raises(lambda: hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120), "other.c")
    (alt / "other.c").write_text(good)
    os.utime(alt / "other.c", ns=(old, old))
    exe = hr.build_program(alt / "other.c", "other", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "after-a-failed-build-the-program-on-disk-is-not-handed-back", same((failed, exe.stat().st_mtime_ns != mtime), (None, True))

    # An input with the very time of the program is not older than it.
    mtime = exe.stat().st_mtime_ns
    os.utime(rig.src / "val.h", ns=(mtime, mtime))
    exe = hr.build_program(rig.src / "withhdr.c", "withhdr", rig.src / "build.toml", rig.build_dir, rig.cc, 120)
    yield "an-input-of-the-same-time-as-the-program-is-built-again", same(exe.stat().st_mtime_ns != mtime, True)

    exe = rig.program("calc")
    mtime = exe.stat().st_mtime_ns
    exe = rig.program("calc")
    quiet = exe.stat().st_mtime_ns == mtime
    (rig.src / "symbols.ld").write_text(SYMBOLS + "unused_name = 0x80190004;\n")
    os.utime(rig.src / "symbols.ld", ns=(mtime + 5 * 10**9, mtime + 5 * 10**9))
    exe = rig.program("calc")
    yield "a-changed-symbol-file-is-built-again", same((quiet, exe.stat().st_mtime_ns != mtime), (True, True))

    # The configuration is read once for each state of its files, not once for each program. A symbol file that
    # moves a name is read again: the program then reads the name at its new address.
    both = {"memory": [("bytes", 0x80190000, bytes([1, 0, 0, 0])), ("bytes", 0x80190040, bytes([2, 0, 0, 0]))], "args": [0]}
    exe = rig.program("rd")
    before = rig.run(exe, [both])[0]["result"]
    mtime = exe.stat().st_mtime_ns
    (rig.src / "symbols.ld").write_text(SYMBOLS.replace("data_word = 0x80190000;", "data_word = 0x80190040;"))
    os.utime(rig.src / "symbols.ld", ns=(mtime + 9 * 10**9, mtime + 9 * 10**9))
    exe = rig.program("rd")
    yield "a-moved-name-of-the-symbol-file-is-read-at-its-new-address", same((before, rig.run(exe, [both])[0]["result"]), (1, 2))
    # A fields file that changes is read again: the generated header has the new field.
    mtime = exe.stat().st_mtime_ns
    (rig.src / "t.fields").write_text("struct S size=0x8\n0x000 u32 a\n0x004 u32 added_field\n")
    os.utime(rig.src / "t.fields", ns=(mtime + 9 * 10**9, mtime + 9 * 10**9))
    rig.program("rd")
    yield "a-changed-fields-file-is-read-again", same("added_field" in (rig.build_dir / "gen" / "t.h").read_text(), True)
    # A configuration that changes is read again (a function's address here); it is then put back as it was.
    cfg_path = rig.src / "build.toml"
    was = hr._configuration(cfg_path)[0]["stub_a"]
    mtime = cfg_path.stat().st_mtime_ns
    cfg_path.write_text(CONFIG.replace("address = 0x80100000", "address = 0x80100040"))
    os.utime(cfg_path, ns=(mtime + 9 * 10**9, mtime + 9 * 10**9))
    now = hr._configuration(cfg_path)[0]["stub_a"]
    cfg_path.write_text(CONFIG)
    os.utime(cfg_path, ns=(mtime + 18 * 10**9, mtime + 18 * 10**9))
    yield "a-changed-configuration-is-read-again", same((was, now, hr._configuration(cfg_path)[0]["stub_a"]), (0x80100000, 0x80100040, 0x80100000))
    # And nothing is read again while the three files stay as they are.
    reads = []
    real = hr.hb.hostcheck.read_config
    hr.hb.hostcheck.read_config = lambda path: reads.append(path) or real(path)
    try:
        rig.program("rd")
        rig.program("calc")
    finally:
        hr.hb.hostcheck.read_config = real
    yield "the-configuration-is-not-read-again-for-each-program", same(reads, [])


def format_cases_test(rig: Rig | None):
    case = {
        "memory": [("fill", 0x80000010, 8, 0x11), ("bytes", 0x1F800004, bytes([0xDE, 0xAD]))],
        "args": [1, 0x80000020, -1],
        "stubs": [{"at": 0x80100000, "arguments": 3, "masks": {0: 0xFF, 2: 0xFFFF}, "pointees": {1: 2}, "returns": [9, 8], "stores": [(2, 0x80000040, 0x55)],
                   "counts": [0x80000044], "ends_at": 3}, {"at": 0x80100010, "arguments": 0}],
        "watch": [(0x80000050, 4)],
    }
    text = (
        "case\nfill 80000010 8 11\nbytes 1f800004 dead\nargs 1 80000020 ffffffff\nstub 80100000 3 3\nmask 0 ff\nmask 2 ffff\npointee 1 2\n"
        "return 9\nreturn 8\nstore 2 80000040 55\ncount 80000044\nstub 80100010 0 0\nwatch 80000050 4\ngo\n"
    )
    yield "the-case-text-is-as-documented", same(hr.format_cases([case]), text)
    want = {
        "memory": [("fill", 0x80000010, 8, 0x11), ("bytes", 0x1F800004, bytes([0xDE, 0xAD]))], "args": [1, 0x80000020, 0xFFFFFFFF],
        "stubs": [{"at": 0x80100000, "arguments": 3, "masks": {0: 0xFF, 2: 0xFFFF}, "pointees": {1: 2}, "returns": [9, 8], "stores": [(2, 0x80000040, 0x55)],
                   "counts": [0x80000044], "ends_at": 3},
                  {"at": 0x80100010, "arguments": 0, "masks": {}, "pointees": {}, "returns": [], "stores": [], "counts": [], "ends_at": 0}],
        "watch": [(0x80000050, 4)],
    }
    yield "the-case-text-round-trips-to-the-normalized-case", same(hr.parse_cases(hr.format_cases([case, {}])), [want, {"memory": [], "args": [], "stubs": [], "watch": []}])
    yield "the-case-text-of-the-parse-is-the-same-text", same(hr.format_cases(hr.parse_cases(text)), text)
    yield "more-than-eight-arguments-are-refused", raises_value(lambda: hr.format_cases([{"args": list(range(9))}]))
    yield "a-fill-byte-above-ff-is-refused", raises_value(lambda: hr.format_cases([{"memory": [("fill", 0, 1, 256)]}]))
    yield "a-mask-for-an-argument-the-stub-does-not-take-is-refused", raises_value(lambda: hr.format_cases([{"stubs": [{"at": 0x80100000, "arguments": 1, "masks": {1: 1}}]}]))
    yield "an-unknown-field-is-refused", raises_value(lambda: hr.format_cases([{"argz": []}]))

    result_text = (
        "begin\nend returned\nresult 15\ncall 80100000 3 5678 80000700 7\nstack 1\npointee 1 0500000000ef\nwatched 05000000\n"
        "call 80100010 0\nchange 80000700 4 06000000\nchange 1f800009 2 0000\ndone\n"
        "begin\nend fault\nfault read 0 200123\nchange 80000900 1 01\ndone\n"
        "begin\nend stub\ndone\n"
    )
    parsed, error = hr.parse_results(result_text)
    want = [
        {"end": "returned", "result": 0x15, "calls": [
            {"at": 0x80100000, "args": [0x5678, 0x80000700, 7], "stack_args": [1], "pointees": {1: bytes([5, 0, 0, 0, 0, 0xEF])}, "watched": [bytes([5, 0, 0, 0])]},
            {"at": 0x80100010, "args": [], "stack_args": [], "pointees": {}, "watched": []}],
         "changes": {0x80000700: 6, 0x80000701: 0, 0x80000702: 0, 0x80000703: 0, 0x1F800009: 0, 0x1F80000A: 0}},
        {"end": "fault", "fault": {"kind": "read", "address": 0, "pc": 0x200123}, "calls": [], "changes": {0x80000900: 1}},
        {"end": "stub", "calls": [], "changes": {}},
    ]
    yield "the-result-text-is-read-as-documented", same((parsed, error), (want, ""))
    yield "the-result-text-round-trips", same("".join(hr.format_result(r) for r in parsed), result_text)
    yield "a-result-without-done-is-not-returned", same(hr.parse_results("begin\nend returned\nresult 1\n"), ([], ""))
    yield "an-error-line-is-returned-as-the-error", same(hr.parse_results("error bad line\n"), ([], "bad line"))


def raises_value(fn):
    try:
        fn()
    except ValueError:
        return None
    return "no ValueError was raised"


def harness_text_cases(rig: Rig):
    exe = rig.program("calc")
    run = lambda text: subprocess.run([*rig.prefix, str(exe)], input=text.encode(), capture_output=True, timeout=30)  # noqa: E731
    got = run("case\nargs 3 4\ngo\n")
    yield "the-harness-reads-text-and-writes-text", same((got.returncode, got.stdout.decode()), (0, "begin\nend returned\nresult 43\ndone\n"))
    yield "the-harness-reads-more-than-one-case", same(run("case\nargs 1\ngo\ncase\nargs 2\ngo\n").stdout.decode(), "begin\nend returned\nresult 1\ndone\nbegin\nend returned\nresult 2\ndone\n")
    got = run("case\nbogus 1\ngo\n")
    yield "an-unknown-line-ends-the-harness-with-an-error-line-and-status-2", same((got.returncode, got.stdout.decode()), (2, "error unknown line: bogus 1\n"))
    got = run("case\nfill 1000 4 1\ngo\n")
    yield "memory-outside-the-two-regions-is-an-error", same((got.returncode, "not in memory" in got.stdout.decode()), (2, True))
    got = run("case\nstub 80000000 0 0\nstub 80000002 0 0\ngo\n")
    yield "two-stubs-that-overlap-are-an-error", same((got.returncode, "overlap" in got.stdout.decode()), (2, True))
    yield "run-cases-raises-with-the-harness-error", raises(lambda: hr.run_cases(exe, [{"memory": [("fill", 0x1000, 4, 1)]}], 10, rig.prefix), "not in memory")


def groups(rig: Rig):
    yield format_cases_test(None)
    yield argument_and_memory_cases(rig)
    yield stub_cases(rig)
    yield fault_cases(rig)
    yield build_cases(rig)
    yield harness_text_cases(rig)


def start_check(cc: str, prefix: list[str], work: Path) -> str | None:
    probe = work / "probe.c"
    probe.write_text("int main(void) { return 7; }\n")
    out = work / "probe.exe"
    proc = subprocess.run([cc, "-o", str(out), str(probe)], capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        return "the compiler cannot build a program: " + (proc.stderr.strip().splitlines() or ["?"])[0]
    try:
        ran = subprocess.run([*prefix, str(out)], capture_output=True, timeout=60)
    except (OSError, subprocess.TimeoutExpired) as err:
        return f"cannot start a Windows program: {err}"
    return None if ran.returncode == 7 else f"cannot start a Windows program (status {ran.returncode})"


def main() -> int:
    parser = argparse.ArgumentParser(description="Tests for hostreplay.py and the replay harness.")
    parser.add_argument("--cc", required=True, help="the cross compiler")
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    started = time.monotonic()
    passed = failed = 0
    with tempfile.TemporaryDirectory(prefix="hostreplay-test-") as tmp:
        work = Path(tmp)
        problem = start_check(args.cc, args.run.split(), work)
        if problem:
            print(problem)
            return 2
        rig = Rig(work, args.cc, args.run.split())
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
