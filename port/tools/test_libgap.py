#!/usr/bin/env python3
"""Controls for libgap.py using synthetic inputs.

Builds a small fake PsyZ tree, a fake inventory and a fake symbol file in a
temporary directory, and a fake `nm` for the archive check. Neither the
submodule nor a compiler is needed. The expected values are worked out here
from each layout, not read from the tool.
"""

from __future__ import annotations

import os
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "libgap.py"
BASE = 0x80100000
GOOD = "as required"


def run(args, env=None) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(SCRIPT), *map(str, args)], capture_output=True, text=True, env=env)


def verdict(ok: bool, detail: str) -> subprocess.CompletedProcess:
    return subprocess.CompletedProcess([], 0 if ok else 1, GOOD if ok else "", "" if ok else detail)


class Fixture:
    """A fake PsyZ tree, inventory and symbol file under root/tag."""

    def __init__(self, root: Path, tag: str, built=None, target=None, other=None, rows=(), symbols="", cmake_head="", headers=None):
        self.base = root / tag
        self.tree = self.base / "tree"
        self.psyz = self.tree
        built = {"src/a.c": ""} if built is None else built
        target = target or {}
        cmake = cmake_head + "set(PSYZ_SOURCES\n" + "".join(f"    {p}\n" for p in built) + "    # src/commented.c\n)\n"
        cmake += "if(WIN32)\n" + "".join(f"    list(APPEND PSYZ_SOURCES {p})\n" for p in target) + "endif()\n"
        self.write(self.tree / "psyz" / "CMakeLists.txt", cmake)
        for rel, text in {**built, **target}.items():
            self.write(self.tree / "psyz" / rel, text)
        for rel, text in (other or {}).items():
            self.write(self.tree / rel, text)
        for rel, text in (headers or {}).items():
            self.write(self.tree / "psyz" / "include" / rel, text)
        lines = []
        for i, row in enumerate(rows):
            name, callers, *rest = row
            family = rest[0] if rest else "fam"
            lines.append(f"{BASE + 16 * i:08x}\t16\t{name}\t{family}\t{callers}\trule")
        self.inventory = self.base / "library.tsv"
        self.write(self.inventory, "\n".join(lines) + "\n")
        self.symbols = self.base / "symbols.ld"
        self.write(self.symbols, symbols)

    @staticmethod
    def write(path: Path, text: str) -> None:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)

    def args(self, *more):
        return ["--inventory", self.inventory, "--symbols", self.symbols, "--psyz", self.psyz, *more]

    def address(self, index: int) -> str:
        return f"{BASE + 16 * index:08x}"


def read_out(path: Path) -> dict[str, list[str]]:
    table = {}
    try:
        text = path.read_text()
    except OSError:
        return table
    for line in text.splitlines():
        cols = line.split("\t")
        if len(cols) == 7:
            table[cols[1] if cols[1] != "-" else cols[0]] = cols
    return table


def read_text(path: Path) -> str:
    try:
        return path.read_text()
    except OSError:
        return ""


def column(table: dict, name: str, index: int) -> str:
    return table[name][index] if name in table else "(missing)"


def status_case(root: Path, tag: str, source: str, wanted: dict[str, str], **kwargs):
    """Scan `source` as the one built file and compare each library name's status."""
    names = list(wanted)
    kwargs.setdefault("built", {"src/a.c": source})
    fix = Fixture(root, tag, rows=[(n, 1) for n in names], **kwargs)
    out = fix.base / "out.tsv"
    proc = run(fix.args("--all", "--out", out))
    if proc.returncode != 0:
        return tag, proc, 0, GOOD
    table = read_out(out)
    got = {n: column(table, n, 5) for n in names}
    return tag, verdict(got == wanted, f"wanted {wanted}, got {got}"), 0, GOOD


SCANNER = [
    ("line-comment", "// int f_x(void) { return 1; }\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("block-comment", "/* int f_x(void) {\n return 1; } */\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("string-literal", 'const char *s = "int f_x(void) { return 1; }";\nint f_y(void) { return 1; }\n', {"f_x": "absent", "f_y": "built"}),
    ("prototype", "int f_x(void);\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("static", "static int f_x(void) { return 1; }\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("static-after-struct", "struct S { int a; };\nstatic int\nf_x(void) { return 1; }\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("three-line-signature", "int f_x(int a,\n         int b,\n         int c)\n{\n    return a;\n}\n", {"f_x": "built"}),
    ("return-type-line-before", "unsigned long\nf_x(void) {\n    return 1;\n}\n", {"f_x": "built"}),
    ("psyz-real-else-stub", "#ifdef __psyz\nint f_x(void) { return 1; }\n#else\nint f_x(void) { NOT_IMPLEMENTED; }\n#endif\n", {"f_x": "built"}),
    ("ifndef-psyz-only", "#ifndef __psyz\nint f_x(void) { return 1; }\n#endif\n", {"f_x": "absent"}),
    ("if-0", "#if 0\nint f_x(void) { return 1; }\n#endif\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    ("if-1-else", "#if 1\nint f_x(void) { return 1; }\n#else\nint f_y(void) { return 1; }\n#endif\n", {"f_x": "built", "f_y": "absent"}),
    ("if-defined-psyz", "#if defined(__psyz)\nint f_x(void) { return 1; }\n#else\nint f_x(void) { NOT_IMPLEMENTED; }\n#endif\n", {"f_x": "built"}),
    ("if-not-defined-psyz", "#if !defined(__psyz)\nint f_x(void) { return 1; }\n#else\nint f_x(void) { NOT_IMPLEMENTED; }\n#endif\n", {"f_x": "stub"}),
    (
        "nested-in-branch-not-taken",
        "#ifndef __psyz\n#ifdef FOO\nint f_x(void) { return 1; }\n#else\nint f_y(void) { return 1; }\n#endif\n#endif\n",
        {"f_x": "absent", "f_y": "absent"},
    ),
    ("nested-unknown-in-branch-taken", "#ifdef __psyz\n#ifdef FOO\nint f_x(void) { return 1; }\n#endif\n#endif\n", {"f_x": "some-targets"}),
    (
        "elif-is-unknown",
        "#if 0\nint f_x(void) { return 1; }\n#elif FOO\nint f_y(void) { return 1; }\n#else\nint f_z(void) { return 1; }\n#endif\n",
        {"f_x": "some-targets", "f_y": "some-targets", "f_z": "some-targets"},
    ),
    (
        "nested-braces-and-brace-in-string",
        'int f_x(void) { if (1) { int a[2] = {1, 2}; } const char *s = "}"; NOT_IMPLEMENTED; }\nint f_y(void) { return 1; }\n',
        {"f_x": "stub", "f_y": "built"},
    ),
    ("stub-mark-in-line-comment", "int f_x(void) {\n    // NOT_IMPLEMENTED\n    return 1;\n}\n", {"f_x": "built"}),
    ("stub-mark-in-block-comment", "int f_x(void) {\n    /* NOT_IMPLEMENTED */\n    return 1;\n}\n", {"f_x": "built"}),
    ("stub-mark-longer-name", "int f_x(void) { NOT_IMPLEMENTED_YET(); return 1; }\n", {"f_x": "built"}),
    (
        "function-like-macro-uses",
        "GTE_REGS(GTE_REG)\nSTATIC_ASSERT(sizeof(int) == 4);\nint f_x(void) { return 1; }\nGTE_REGS(GTE_REG)\nint f_y(void) { return 1; }\n",
        {"f_x": "built", "f_y": "built"},
    ),
    ("call-inside-body", "int f_x(void) { f_y(1); return 0; }\n", {"f_x": "built", "f_y": "absent"}),
    ("define-body", "#define MAKE int f_x(void) { return 1; }\nint f_y(void) { return 1; }\n", {"f_x": "absent", "f_y": "built"}),
    (
        "backslash-continuation",
        "#define MAKE(a) \\\n    int f_x(void) { return a; }\nint f_y(void) { return 1 +\\\n 2; }\n",
        {"f_x": "absent", "f_y": "built"},
    ),
    ("include-asm-in-braces", 'void g(void) {\n    INCLUDE_ASM("asm/x", f_x);\n}\n', {"f_x": "absent", "g": "built"}),
    ("include-asm", 'INCLUDE_ASM("asm/x", f_x);\n', {"f_x": "assembly"}),
    ("weak-include-asm", 'WEAK_INCLUDE_ASM("asm/x", f_x);\n', {"f_x": "assembly"}),
    ("include-asm-then-definition", 'INCLUDE_ASM("asm/x", f_x);\nint f_x(void) { return 1; }\n', {"f_x": "built"}),
    (
        "non-definitions-do-not-break-scan",
        'extern "C" {\nint f_x(void) { return 1; }\n}\nstruct X { int a; };\nenum { A, B };\ntypedef struct { int a; } T;\n'
        "int tbl[] = { 1, 2 };\nunion U { int a; float b; };\nint f_y(void) { return 1; }\n",
        {"f_x": "built", "f_y": "built"},
    ),
    (
        "function-pointer-parameter",
        "int f_x(int (*cb)(int), int b) { return cb(b); }\nint f_y(void) { return 1; }\n",
        {"f_x": "built", "f_y": "built", "cb": "absent"},
    ),
    ("attribute-before-return-type", "__attribute__((noinline)) int f_x(void) { return 1; }\n", {"f_x": "built", "__attribute__": "absent"}),
    ("keywords-are-not-names", "if (1) { }\nwhile (1) { }\nint f_x(void) { return 1; }\n", {"if": "absent", "while": "absent", "f_x": "built"}),
    ("function-returning-function-pointer", "void (*f_x(void (*cb)())) { return 0; }\nint f_y(void) { return 1; }\n", {"f_x": "built", "void": "absent", "f_y": "built"}),
    ("if-psyz-value", "#if __psyz\nint f_x(void) { return 1; }\n#else\nint f_y(void) { return 1; }\n#endif\n", {"f_x": "built", "f_y": "absent"}),
    ("if-not-psyz-value", "#if !__psyz\nint f_x(void) { return 1; }\n#else\nint f_y(void) { return 1; }\n#endif\n", {"f_x": "absent", "f_y": "built"}),
    (
        "psx-mark-is-undefined",
        "#ifndef __psx__\nint f_x(void) { return 1; }\n#endif\n#ifdef __psx__\nint f_y(void) { return 1; }\n#endif\n",
        {"f_x": "built", "f_y": "absent"},
    ),
    (
        "unknown-branches-each-open-a-brace",
        "int f_x(int a) {\n#if FOO\n    if (0) {\n#else\n    if (a) {\n#endif\n        a++;\n    }\n    return a;\n}\nint f_y(void) { return 1; }\n",
        {"f_x": "built", "f_y": "built"},
    ),
    (
        "unknown-ifdef-opens-brace-closed-by-later-one",
        "#ifdef A\nint f_x(void) {\n#endif\n    return 1;\n#ifdef A\n}\n#endif\nint f_y(void) { return 1; }\n",
        {"f_x": "some-targets", "f_y": "built"},
    ),
    ("mark-only-under-unknown-condition", "int f_x(void) {\n#ifdef FOO\n    NOT_IMPLEMENTED;\n#endif\n    return 1;\n}\n", {"f_x": "some-targets"}),
    ("mark-under-psyz", "int f_x(void) {\n#ifdef __psyz\n    NOT_IMPLEMENTED;\n#endif\n    return 1;\n}\n", {"f_x": "stub"}),
    (
        "guard-taken-else-not-scanned",
        "#ifndef G\n#define G\nint f_x(void) { return 1; }\n#else\nint f_y(void) { return 1; }\n#endif\n",
        {"f_x": "built", "f_y": "absent"},
    ),
    ("ifndef-then-other-directive-is-unknown", "#ifndef G\n#include <a.h>\nint f_x(void) { return 1; }\n#endif\n", {"f_x": "some-targets"}),
    (
        "later-branch-holds-whole-definition",
        "#ifdef A\nvoid first(void) {\n#else\nvoid whole(void) { }\nvoid first(void) {\n#endif\n}\nvoid after(void) { }\n",
        {"whole": "some-targets", "first": "some-targets", "after": "built"},
    ),
    (
        "branches-end-at-different-depths",
        "void outer(void) {\n#ifdef A\n    if (x) {\n#else\n#endif\n        work();\n#ifdef A\n    }\n#endif\n}\nvoid after(void) { }\n",
        {"outer": "built", "after": "built"},
    ),
    (
        "later-branch-definition-not-ended-is-not-found",
        "#ifdef A\nvoid a(void) {\n#else\nvoid b(void) {\n#endif\n}\n",
        {"a": "some-targets", "b": "absent"},
    ),
    ("ifndef-then-other-define-is-unknown", "#ifndef G\n#define H\nint f_x(void) { return 1; }\n#endif\n", {"f_x": "some-targets"}),
]


def broken_cases(root: Path):
    for tag, source, want in (
        ("open-brace-at-end", "int f_x(void) {\n    return 1;\n", "a.c"),
        ("one-brace-too-many", "int f_x(void) { return 1; }\n}\n", "a.c:2"),
    ):
        fix = Fixture(root, "broken-" + tag, built={"src/a.c": source}, rows=[("f_x", 1)])
        yield "refuse-" + tag, run(fix.args()), 2, want
    fix = Fixture(root, "broken-header", built={"src/a.c": ""}, headers={"h.h": "void f(void) {\n"}, rows=[("f_x", 1)])
    yield "refuse-header-open-brace", run(fix.args()), 2, "h.h"


HEADER = [
    "#define r_built f_built",
    "#define r_stub f_stub",
    "#define r_tgt f_tgt",
    "#define r_abs f_nothing",
    "#ifndef __psx__",
    "#define r_psx f_built",
    "#endif",
    "#ifdef __psx__",
    "#define r_notaken f_built",
    "#endif",
    "#ifdef FOO",
    "#define r_unk f_built",
    "#endif",
    "#define chain1 chain2",
    "#define chain2 f_built",
    "#define r_num 5",
    "#define r_expr (f_built)",
    "#define fm(x) (x)",
    "#define sx sx",
]


def rename_cases(root: Path):
    names = ["r_built", "r_stub", "r_tgt", "r_abs", "r_psx", "r_notaken", "r_unk", "chain1", "r_num", "r_expr", "fm", "sx", "f2t", "r_two"]
    header = "\n".join(HEADER) + "\n#define r_two f2t\n"
    source = "int f_built(void) { return 1; }\nint f_stub(void) { NOT_IMPLEMENTED; }\nint fm(void) { return 1; }\nint sx(void) { return 1; }\n"
    target = {"src/t1.c": "int f_tgt(void) { return 1; }\nint f2t(void) { return 1; }\n", "src/t2.c": "int f2t(void) { return 2; }\n"}
    fix = Fixture(root, "renames", built={"src/a.c": source}, target=target, headers={"h.h": header}, rows=[(n, 1) for n in names])
    out = fix.base / "out.tsv"
    proc = run(fix.args("--out", out))
    want = {
        "r_built": "built", "r_stub": "stub", "r_tgt": "some-targets", "r_abs": "absent", "r_psx": "built", "r_notaken": "absent",
        "r_unk": "some-targets", "chain1": "absent", "r_num": "absent", "r_expr": "absent", "fm": "built", "sx": "built",
        "f2t": "some-targets", "r_two": "some-targets",
    }
    table = read_out(out) if proc.returncode == 0 else {}
    got = {k: v[5] for k, v in table.items()}
    yield "rename-statuses", verdict(got == want, f"wanted {want}, got {got} {proc.stderr}"), 0, GOOD
    h = "psyz/include/h.h"
    at = lambda text: HEADER.index(text) + 1
    yield "rename-line", proc, 0, f"renamed by a header: r_abs to f_nothing ({h}:{at('#define r_abs f_nothing')})\n"
    yield "rename-line-followed-under-psx-undefined", proc, 0, f"renamed by a header: r_psx to f_built ({h}:{at('#define r_psx f_built')})\n"
    yield "rename-line-unknown", proc, 0, f"renamed by a header: r_unk to f_built ({h}:{at('#define r_unk f_built')})\n"
    yield "rename-line-chain-one-step", proc, 0, f"renamed by a header: chain1 to chain2 ({h}:{at('#define chain1 chain2')})\n"
    yield "macro-line", proc, 0, f"macro in a header: fm ({h}:{at('#define fm(x) (x)')})\n"
    text = proc.stdout
    yield "rename-not-taken-no-line", verdict("r_notaken to" not in text and "r_num to" not in text and "r_expr to" not in text, text), 0, GOOD
    yield "self-rename-ignored", verdict("renamed by a header: sx " not in text, text), 0, GOOD
    yield "rename-lines-before-macro-line", verdict(0 <= text.find("renamed by a header") < text.find("macro in a header"), text), 0, GOOD
    place = {k: v[6] for k, v in table.items()}
    wanted = {
        "r_built": f"{h}:1,psyz/src/a.c:1",
        "r_two": f"{h}:{len(HEADER) + 1},psyz/src/t1.c:2,psyz/src/t2.c:1",
        "f2t": "psyz/src/t1.c:2,psyz/src/t2.c:1",
        "r_abs": f"{h}:4",
        "r_unk": f"{h}:12",
    }
    yield "out-places-renamed-and-two-targets", verdict(all(place.get(k) == v for k, v in wanted.items()), str(place)), 0, GOOD
    # archive looks up the other name
    rows = [("r_built", 1)]
    fix = Fixture(root, "rename-archive", built={"src/a.c": source}, headers={"h.h": "#define r_built f_built\n"}, rows=rows)
    lib = fix.base / "libfake.a"
    lib.write_text("")
    yield "archive-renamed-looks-up-other", run(fix.args("--archive", lib), env=fake_nm(root, "nm-ren-a", "0 T f_built\n")), 0, "archive: 1 names checked, 0 disagree\n"
    yield "archive-renamed-own-name-not-looked-up", run(fix.args("--archive", lib), env=fake_nm(root, "nm-ren-b", "0 T r_built\n")), 1, "archive: r_built is built and is not defined\n"


def guard_header_cases(root: Path):
    built = {"src/a.c": "int f_built(void) { return 1; }\n"}
    for tag, header, want in (
        ("guard", "#ifndef H_H\n#define H_H\n#define g_ren f_built\n#endif\n", "built"),
        ("other-directive", "#ifndef H_H\n#include <x.h>\n#define g_ren f_built\n#endif\n", "some-targets"),
        ("other-name", "#ifndef H_H\n#define OTHER_H\n#define g_ren f_built\n#endif\n", "some-targets"),
        ("guard-else-not-scanned", "#ifndef H_H\n#define H_H\n#else\n#define g_ren f_built\n#endif\n", "absent"),
    ):
        fix = Fixture(root, "hdr-" + tag, built=built, headers={"g.h": header}, rows=[("g_ren", 1)])
        out = fix.base / "out.tsv"
        proc = run(fix.args("--out", out))
        got = column(read_out(out), "g_ren", 5) if proc.returncode == 0 else proc.stderr
        yield "header-" + tag, verdict(got == want, f"wanted {want}, got {got}"), 0, GOOD
    fix = Fixture(root, "hdr-conflict", built=built, headers={"a.h": "#define cx f_built\n", "b.h": "#define cx f_other\n"}, rows=[("cx", 1)])
    proc = run(fix.args())
    yield "refuse-conflicting-renames-first", proc, 2, "f_built (psyz/include/a.h:1)"
    yield "refuse-conflicting-renames-second", proc, 2, "f_other (psyz/include/b.h:1)"


def statuses_case(root: Path):
    rows = [
        ("lib_built", 1), ("lib_stub", 1), ("lib_some", 1), ("lib_cond", 1), ("lib_asm", 1), ("lib_nb_def", 1),
        ("lib_nb_asm", 1), ("lib_absent", 1), ("-", 1), ("func_80100099", 1),
    ]
    built = {
        "src/a.c": "int lib_built(void) { return 1; }\nint lib_stub(void) { NOT_IMPLEMENTED; }\n"
        '#ifdef _WIN32\nint lib_cond(void) { return 1; }\n#else\nint lib_cond(void) { return 2; }\n#endif\nINCLUDE_ASM("asm/x", lib_asm);\n'
    }
    fix = Fixture(
        root, "statuses", built=built, target={"src/t.c": "int lib_some(void) { return 1; }\n"},
        other={"decomp/src/o.c": 'int lib_nb_def(void) { return 1; }\nINCLUDE_ASM("asm/y", lib_nb_asm);\n'}, rows=rows,
    )
    out = fix.base / "out.tsv"
    proc = run(fix.args("--out", out))
    want = {
        "lib_built": "built", "lib_stub": "stub", "lib_some": "some-targets", "lib_cond": "some-targets", "lib_asm": "assembly",
        "lib_nb_def": "not-built", "lib_nb_asm": "not-built", "lib_absent": "absent", fix.address(8): "unnamed", fix.address(9): "unnamed",
    }
    table = read_out(out) if proc.returncode == 0 else {}
    got = {k: v[5] for k, v in table.items()}
    yield "status-one-of-each", verdict(got == want, f"wanted {want}, got {got}"), 0, GOOD
    for line in ("stub: lib_stub\n", "some-targets: lib_cond lib_some\n", "assembly: lib_asm\n", "not-built: lib_nb_asm lib_nb_def\n",
                 "absent: lib_absent\n", f"unnamed: {fix.address(8)} {fix.address(9)}\n", "built: 1\n", "compared: 10 of 10 library functions (those with a caller)\n"):
        yield f"status-line {line.strip()}", proc, 0, line
    place = {k: v[6] for k, v in table.items()}
    wanted_place = {"lib_built": "psyz/src/a.c:1", "lib_some": "psyz/src/t.c:1", "lib_nb_def": "decomp/src/o.c:1", "lib_absent": "-"}
    yield "status-places", verdict(all(place.get(k) == v for k, v in wanted_place.items()), str(place)), 0, GOOD


def symbol_cases(root: Path):
    rows = [("-", 1), ("-", 1), ("func_80100020", 1), ("RealName", 1), ("-", 1), ("-", 1)]
    fix = Fixture(root, "symbols", built={"src/a.c": "int alias_one(void) { return 1; }\nint bb_built(void) { return 1; }\nint func_c(void) { return 1; }\n"}, rows=rows,
                  symbols="/* names */\n"
                  f"alias_one = 0x{BASE:08x};\n"
                  f"aa_missing = 0x{BASE + 16:08x};\nbb_built = 0x{BASE + 16:08X};\n"
                  f"func_c = 0x{BASE + 32:08x};\n"
                  f"ignored_alias = 0x{BASE + 48:08x};\n"
                  f"zz_b = 0x{BASE + 64:08x};\nzz_a = 0x{BASE + 64:08x};\n")
    out = fix.base / "out.tsv"
    proc = run(fix.args("--all", "--out", out))
    lines = read_text(out).splitlines() if proc.returncode == 0 else []
    want = [
        f"{fix.address(0)}\talias_one\tsymbols\tfam\t1\tbuilt\tpsyz/src/a.c:1",
        f"{fix.address(1)}\tbb_built\tsymbols\tfam\t1\tbuilt\tpsyz/src/a.c:2",
        f"{fix.address(2)}\t-\t-\tfam\t1\tunnamed\t-",
        f"{fix.address(3)}\tRealName\tinventory\tfam\t1\tabsent\t-",
        f"{fix.address(4)}\tzz_a\tsymbols\tfam\t1\tabsent\t-",
        f"{fix.address(5)}\t-\t-\tfam\t1\tunnamed\t-",
    ]
    yield "symbols-rows", verdict(lines == want, "\n".join(lines)), 0, GOOD


def format_cases(root: Path):
    rows = [("b1", 2, "sound"), ("s1", 1, "sound"), ("s2", 1, "disc"), ("ab1", 3, "disc"), ("-", 1, "disc"), ("b2", 0, "disc")]
    src = "int b1(void) { return 1; }\nint s1(void) { NOT_IMPLEMENTED; }\nint s2(void) { NOT_IMPLEMENTED; }\nint b2(void) { return 2; }\n"
    fix = Fixture(root, "format", built={"src/a.c": src}, rows=rows)
    out = fix.base / "out.tsv"
    proc = run(fix.args("--out", out))
    expected = (
        "compared: 5 of 6 library functions (those with a caller)\n"
        "built: 1\nstub: 2\nsome-targets: 0\nassembly: 0\nnot-built: 0\nabsent: 1\nunnamed: 1\n"
        "disc: built 0, stub 1, some-targets 0, assembly 0, not-built 0, absent 1, unnamed 1\n"
        "sound: built 1, stub 1, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 0\n"
        "stub: s1 s2\nabsent: ab1\n"
        f"unnamed: {fix.address(4)}\n"
    )
    yield "format-stdout-whole", verdict(proc.returncode == 0 and proc.stdout == expected, f"got:\n{proc.stdout}\n{proc.stderr}"), 0, GOOD
    expected_out = (
        f"{fix.address(0)}\tb1\tinventory\tsound\t2\tbuilt\tpsyz/src/a.c:1\n"
        f"{fix.address(1)}\ts1\tinventory\tsound\t1\tstub\tpsyz/src/a.c:2\n"
        f"{fix.address(2)}\ts2\tinventory\tdisc\t1\tstub\tpsyz/src/a.c:3\n"
        f"{fix.address(3)}\tab1\tinventory\tdisc\t3\tabsent\t-\n"
        f"{fix.address(4)}\t-\t-\tdisc\t1\tunnamed\t-\n"
    )
    got = read_text(out)
    yield "format-out-file-whole", verdict(got == expected_out, f"got:\n{got}"), 0, GOOD
    every = fix.base / "all.tsv"
    proc = run(fix.args("--all", "--out", every))
    first = proc.stdout.splitlines()[0] if proc.stdout else ""
    yield "all-first-line", verdict(first == "compared: 6 of 6 library functions (all)", first), 0, GOOD
    lines = read_text(every).splitlines()
    yield "all-includes-zero-callers", verdict(len(lines) == 6 and lines[5].split("\t")[1:2] == ["b2"], "\n".join(lines)), 0, GOOD
    yield "zero-callers-left-out", verdict(out.exists() and "b2" not in read_text(out), "b2 present or no out file"), 0, GOOD


def duplicate_cases(root: Path):
    rows = [("f_dup", 1), ("f_mix", 1), ("f_one", 1)]
    built = {
        "src/a.c": "int f_dup(void) { return 1; }\nINCLUDE_ASM(\"asm/m\", f_mix);\nint f_one(void) { return 1; }\n",
        "src/b.c": "\n\nint f_dup(void) { return 2; }\nint f_mix(void) { NOT_IMPLEMENTED; }\n",
    }
    fix = Fixture(root, "dups", built=built, rows=rows)
    out = fix.base / "out.tsv"
    proc = run(fix.args("--out", out))
    yield "duplicate-line", proc, 0, "defined more than once: f_dup (psyz/src/a.c:1, psyz/src/b.c:3)\n"
    table = read_out(out) if proc.returncode == 0 else {}
    yield "asm-and-stub-is-stub", verdict(column(table, "f_mix", 5) == "stub", str(table.get("f_mix"))), 0, GOOD
    yield "single-definition-no-duplicate-line", verdict(proc.stdout.count("defined more than once") == 1, proc.stdout), 0, GOOD


def fake_nm(root: Path, tag: str, body: str, code: int = 0) -> dict:
    bindir = root / tag / "bin"
    bindir.mkdir(parents=True, exist_ok=True)
    script = bindir / "nm"
    if code == 0:
        script.write_text(f"#!/bin/sh\ncat <<'EOF'\n{body}\nEOF\n")
    else:
        script.write_text(f"#!/bin/sh\necho 'nm: broken archive' >&2\nexit {code}\n")
    script.chmod(0o755)
    return {**os.environ, "PATH": f"{bindir}{os.pathsep}{os.environ['PATH']}"}


def archive_cases(root: Path):
    rows = [("lib_built", 1), ("lib_stub", 1), ("lib_asm", 1), ("lib_absent", 1), ("lib_some", 1), ("-", 1)]
    built = {"src/a.c": 'int lib_built(void) { return 1; }\nint lib_stub(void) { NOT_IMPLEMENTED; }\nINCLUDE_ASM("asm/x", lib_asm);\n'}
    fix = Fixture(root, "archive", built=built, target={"src/t.c": "int lib_some(void) { return 1; }\n"}, rows=rows)
    lib = fix.base / "libfake.a"
    lib.write_text("")
    good = "a.o:\n0000000000000000 T lib_built\n0000000000000010 W lib_stub\n0000000000000020 t lib_local\n                 U printf\n"
    run_ = lambda tag, nm_body, code=0, more=(): run(fix.args("--archive", lib, *more), env=fake_nm(root, tag, nm_body, code))
    yield "archive-agrees", run_("nm-good", good), 0, "archive: 4 names checked, 0 disagree\n"
    proc = run_("nm-bad", "a.o:\n0000000000000000 T lib_absent\n0000000000000010 T lib_stub\n")
    yield "archive-built-missing", proc, 1, "archive: lib_built is built and is not defined\n"
    yield "archive-absent-defined", proc, 1, "archive: lib_absent is absent and is defined\n"
    yield "archive-disagree-count", proc, 1, "archive: 4 names checked, 2 disagree\n"
    yield "archive-assembly-defined", run_("nm-asm", good + "0000000000000030 T lib_asm\n"), 1, "archive: lib_asm is assembly and is defined\n"
    prefixed = "0000 T _lib_built\n0000 T _lib_stub\n0000 T _other\n"
    yield "archive-leading-underscore", run_("nm-underscore", prefixed), 0, "archive: 4 names checked, 0 disagree\n"
    lower = "0000 t lib_built\n0000 T lib_stub\n"
    yield "archive-lowercase-t-does-not-count", run_("nm-lower", lower), 1, "archive: lib_built is built and is not defined\n"
    yield "archive-some-targets-not-checked", run_("nm-some", good + "0000 T lib_some\n"), 0, "0 disagree\n"
    yield "archive-nm-fails", run_("nm-fail", "", 3), 2, str(lib)
    yield "archive-missing", run(fix.args("--archive", fix.base / "nope.a"), env=fake_nm(root, "nm-none", good)), 2, "nope.a"


def refusal_cases(root: Path):
    rows = [("lib_built", 1)]
    built = {"src/a.c": "int lib_built(void) { return 1; }\n"}
    fix = Fixture(root, "refuse", built=built, rows=rows)
    yield "refuse-missing-inventory", run(fix.args() + ["--inventory", fix.base / "gone.tsv"]), 2, "gone.tsv"

    def with_inventory(tag: str, text: str):
        path = fix.base / f"{tag}.tsv"
        path.write_text(text)
        return run(["--inventory", path, "--symbols", fix.symbols, "--psyz", fix.psyz]), path

    proc, path = with_inventory("columns", "80100000\t16\tlib_built\tfam\t1\n")
    yield "refuse-wrong-column-count", proc, 2, str(path)
    proc, path = with_inventory("address", "80100zz0\t16\tlib_built\tfam\t1\trule\n")
    yield "refuse-address-not-hex", proc, 2, str(path)
    proc, path = with_inventory("callers", "80100000\t16\tlib_built\tfam\tmany\trule\n")
    yield "refuse-callers-not-number", proc, 2, str(path)
    yield "refuse-missing-symbols", run(fix.args() + ["--symbols", fix.base / "gone.ld"]), 2, "gone.ld"
    yield "refuse-missing-psyz", run(fix.args() + ["--psyz", fix.base / "no-tree"]), 2, "no-tree"
    bare = Fixture(root, "refuse-nocmake", built=built, rows=rows)
    cmake = bare.tree / "psyz" / "CMakeLists.txt"
    cmake.write_text("project(x)\n")
    yield "refuse-build-file-without-sources", run(bare.args()), 2, str(cmake)
    broken = Fixture(root, "refuse-nopath", built=built, rows=rows)
    cmake = broken.tree / "psyz" / "CMakeLists.txt"
    cmake.write_text("set(PSYZ_SOURCES\n    src/a.c\n    src/missing.c\n)\n")
    proc = run(broken.args())
    yield "refuse-build-file-path-missing", proc, 2, "src/missing.c"
    yield "refuse-names-build-file", proc, 2, str(cmake)


def cmake_cases(root: Path):
    fix = Fixture(root, "cmake", built={"src/a.c": "int f_built(void) { return 1; }\n"}, target={"src/t.c": "int f_target(void) { return 1; }\n"},
                  rows=[("f_built", 1), ("f_target", 1)], cmake_head="# set(PSYZ_SOURCES src/nope.c)\n")
    out = fix.base / "out.tsv"
    proc = run(fix.args("--out", out))
    got = {k: v[5] for k, v in read_out(out).items()} if proc.returncode == 0 else {}
    yield "cmake-built-and-target", verdict(got == {"f_built": "built", "f_target": "some-targets"}, f"{got} {proc.stderr}"), 0, GOOD


def scanner_cases(root: Path):
    for tag, source, wanted in SCANNER:
        yield status_case(root, "scan-" + tag, source, wanted)
    yield status_case(
        root, "scan-two-built-files", "", {"f_x": "stub"},
        built={"src/a.c": 'INCLUDE_ASM("asm/x", f_x);\n', "src/b.c": "int f_x(void) { NOT_IMPLEMENTED; }\n"},
    )


def groups():
    return [statuses_case, symbol_cases, scanner_cases, format_cases, duplicate_cases, archive_cases, refusal_cases,
            cmake_cases, broken_cases, rename_cases, guard_header_cases]


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for group in groups():
            produced = group(Path(tmp))
            while True:
                try:
                    name, proc, want_status, want_text = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a control must report, not crash
                    print(f"FAIL {group.__name__}: the control itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                output = proc.stdout + proc.stderr
                ok = proc.returncode == want_status and want_text in output
                print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {proc.returncode}, wanted {want_status} with {want_text!r}")
                if not ok:
                    print(output)
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
