#!/usr/bin/env python3
"""Controls for hostcheck.py using synthetic inputs.

Builds small trees (a build.toml, a field table, unit sources) in a temporary
directory. Cases about parsing and counting use a fake compiler script that
prints fixed lines; cases about layouts and warnings use the real `cc` of the
host and assume 8-byte pointers, which is checked first with the tool's own
probe. The expected values are worked out here from each fixture, never read
back from the tool.
"""

from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "hostcheck.py"

FAKE = """#!{python}
import json, sys
from pathlib import Path
spec = json.loads({spec!r})
args = sys.argv[1:]
if args == ["--version"]:
    if "version" in spec.get("sleep", []):
        import time
        time.sleep(5)
    print(spec.get("version", "fakecc 1.0"))
    print("second line")
    sys.exit(spec.get("version_status", 0))
if spec.get("record"):
    with open(spec["record"], "a") as log:
        log.write(" ".join(args) + "\\n")
source = Path(args[-1])
name = source.name
kind = "pointer" if name.startswith("pointer") else "layout" if name.startswith("layout_") else "unit"
obj = Path(args[args.index("-o") + 1]) if "-o" in args else None
unit = obj.stem if obj else name
sys.stderr.write(spec.get("stderr", {{}}).get(unit, "") if kind == "unit" else "")
sys.stderr.flush()
if kind in spec.get("sleep", []):
    import time
    time.sleep(5)
if kind == "pointer":
    sys.exit(0 if int(name[7:-2]) == spec.get("pointer", 8) else 1)
if kind == "layout":
    text = source.read_text()
    bad = any(t in text for t in spec.get("fail_if_contains", []))
    sys.exit(1 if bad or name[7:-2] in spec.get("lost", []) else 0)
if unit in spec.get("fail", []):
    sys.exit(1)
if obj:
    obj.write_text("object")
"""

FIELDS = """struct Scalars size=0x8
0x000 u32 a
0x004 s16 b
0x006 u8 c

struct WithPtr size=0x8
0x000 void* p
0x004 u32 n
"""

REAL_FIELDS = FIELDS + """
struct Holder size=0x10
0x000 WithPtr inner
0x008 u32 tail

struct Arr size=0x8
0x000 u8 bytes[4]
0x004 u32 w

struct Last size=0x4
0x000 Scalars* link
"""


def write(path: Path, text: str) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    return path


def fake_cc(root: Path, tag: str, **spec) -> Path:
    path = root / f"fakecc-{tag}"
    path.write_text(FAKE.format(python=sys.executable, spec=json.dumps(spec)))
    path.chmod(0o755)
    return path


def tree(root: Path, tag: str, units: dict[str, tuple[str, str | None]], fields: str = FIELDS, types: str | None = None, order=None, flags=None) -> Path:
    """A tree under root/tag. units: name -> (source path, text or None to not write it)."""
    base = root / tag
    toml = '[types]\nfields = "t.fields"\nheader = "t.h"\n' if types is None else types
    for name in order or units:
        source, text = units[name]
        toml += f'\n[[unit]]\nname = "{name}"\n'
        if source:
            toml += f'source = "{source}"\n'
        toml += f"flags = {json.dumps((flags or {}).get(name, ['-O2', '-G0']))}\n"
    config = write(base / "build.toml", toml)
    write(base / "t.fields", fields)
    for name, (source, text) in units.items():
        if source and text is not None:
            write(base / source, text)
    return config


def run(config: Path, build: Path, cc, *more) -> subprocess.CompletedProcess:
    argv = [sys.executable, str(SCRIPT), "--config", str(config), "--build", str(build), "--cc", str(cc), *map(str, more)]
    return subprocess.run(argv, capture_output=True, text=True, timeout=300)


def lines(proc: subprocess.CompletedProcess) -> list[str]:
    return proc.stdout.splitlines()


def need(proc: subprocess.CompletedProcess):
    """None when the run was made, else a detail for a failed case."""
    if proc.returncode != 0:
        return f"exit {proc.returncode}: {proc.stderr.strip()}"
    return None


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def row(build: Path, unit: str) -> list[str]:
    for text in read(build / "units.tsv").splitlines():
        cols = text.split("\t")
        if cols[0] == unit:
            return cols
    return []


def read(path: Path) -> str:
    try:
        return path.read_text()
    except OSError:
        return "(missing)"


# Fixtures for the fake compiler.

DIAG_STDERR = {
    "ua": (
        "ua.c:3:5: warning: foo [-Wfoo]\n"
        "ua.c:4:5: error: bar [-Wbar]\n"
        "ua.c:5:5: warning: baz\n"
        "ua.c:5:5: note: qux [-Wnote]\n"
        "ua.c:6: warning: nocol [-Wnocol]\n"
        "something without a level [-Wzzz]\n"
        "ua.c:7:1: warning: bad index [3]\n"
    ),
    "ub": (
        "ub.c:1:1: warning: x [-Wfoo]\n"
        "ub.c:2:1: warning: x [-Wfoo]\n"
        "ub.c:3:1: warning: y [-Wtwo]\n"
        "ub.c:4:1: warning: y [-Wtwo]\n"
    ),
    "uc": "uc.c:1:1: error: z [-Werror=implicit]\n",
}


def diag_tree(root: Path, tag: str) -> tuple[Path, Path]:
    units = {
        "uc": ("c/uc.c", "unsigned a = 0xbfc00000;\n"),
        "ua": ("a/ua.c", "unsigned a = 0x80000000;\nunsigned b = 0x1f800010;\n"),
        "ub": ("ub.c", "int b;\n"),
        "libx": ("sdk/libx.c", "this is not C\n"),
        "asm": ("asm/y.s", "nop\n"),
    }
    config = tree(root, tag, units)
    cc = fake_cc(root, tag, stderr=DIAG_STDERR, fail=["ua", "uc"], lost=["WithPtr"])
    return config, cc


DIAG_STDOUT = """compiler: fakecc 1.0
language level: gnu89
pointer size: 8 bytes
units: 3 compiled, 1 under sdk/ and 1 not C left out
passed: 1
failed: 2
warning -Wfoo: 3 in 2 units
warning (no option): 2 in 1 units
warning -Wtwo: 2 in 1 units
error -Wbar: 1 in 1 units
error -Werror=implicit: 1 in 1 units
warning -Wnocol: 1 in 1 units
structs: 1 of 2 keep their layout
structs with a pointer: 1, of which 1 lose their layout
structs without a pointer: 1, of which 0 lose their layout
fixed addresses: 3 literals in 2 units
main memory: 1 literals in 1 units
main memory, uncached: 0 literals in 0 units
scratchpad: 1 literals in 1 units
ports: 0 literals in 0 units
BIOS: 1 literals in 1 units
failed: ua uc
"""

DIAG_UNITS = (
    "ua\ta/ua.c\tfailed\t4\t1\t2\n"
    "ub\tub.c\tpassed\t4\t0\t0\n"
    "uc\tc/uc.c\tfailed\t0\t1\t1\n"
)
DIAG_STRUCTS = "Scalars\t8\tkept\tplain\nWithPtr\t8\tlost\tpointer\n"


def whole_output_cases(root: Path):
    config, cc = diag_tree(root, "whole")
    build = root / "whole-build"
    proc = run(config, build, cc)
    yield "output-whole-stdout", need(proc) or same(proc.stdout, DIAG_STDOUT)
    yield "output-units-table", same(read(build / "units.tsv"), DIAG_UNITS)
    yield "output-structs-table", same(read(build / "structs.tsv"), DIAG_STRUCTS)
    yield "output-nothing-on-stderr", same(proc.stderr, "")
    yield "output-header-written", same("typedef struct Scalars" in read(build / "gen" / "t.h"), True)
    one = run(config, root / "jobs1-build", cc, "--jobs", 1)
    four = run(config, root / "jobs4-build", cc, "--jobs", 4)
    yield "jobs-same-stdout", need(one) or need(four) or same(one.stdout, four.stdout)
    yield "jobs-same-units-table", same(read(root / "jobs1-build" / "units.tsv"), read(root / "jobs4-build" / "units.tsv"))
    yield "jobs-same-structs-table", same(read(root / "jobs1-build" / "structs.tsv"), read(root / "jobs4-build" / "structs.tsv"))
    yield "jobs-stdout-is-expected", same(one.stdout, DIAG_STDOUT)


def diagnostic_cases(root: Path):
    config, cc = diag_tree(root, "diag")
    proc = run(config, root / "diag-build", cc)
    out = lines(proc)
    err = need(proc)
    kinds = [x for x in out if x.startswith(("warning ", "error "))]
    yield "diag-warning-with-option", err or same("warning -Wfoo: 3 in 2 units" in out, True)
    yield "diag-error-with-option", err or same("error -Wbar: 1 in 1 units" in out, True)
    yield "diag-warning-without-option", err or same("warning (no option): 2 in 1 units" in out, True)
    yield "diag-note-not-counted", err or same([x for x in kinds if "note" in x], [])
    yield "diag-line-without-level-not-counted", err or same([x for x in kinds if "zzz" in x], [])
    yield "diag-two-in-one-unit", err or same("warning -Wtwo: 2 in 1 units" in out, True)
    yield "diag-without-column", err or same("warning -Wnocol: 1 in 1 units" in out, True)
    yield "diag-option-taken-as-printed", err or same("error -Werror=implicit: 1 in 1 units" in out, True)
    yield "diag-order-by-count-then-text", err or same(
        kinds,
        [
            "warning -Wfoo: 3 in 2 units",
            "warning (no option): 2 in 1 units",
            "warning -Wtwo: 2 in 1 units",
            "error -Wbar: 1 in 1 units",
            "error -Werror=implicit: 1 in 1 units",
            "warning -Wnocol: 1 in 1 units",
        ],
    )


def host_checked(root: Path):
    """Return (cc path, None) when the real compiler is usable, else (None, one line)."""
    cc = shutil.which("cc")
    if cc is None:
        return None, "test_hostcheck.py: no C compiler `cc` on PATH; the cases that need one cannot run"
    config = tree(root, "probe", {"u": ("u.c", "int u;\n")})
    proc = run(config, root / "probe-build", cc)
    if proc.returncode != 0:
        return None, f"test_hostcheck.py: the tool could not use `cc`: {proc.stderr.strip()}"
    if "pointer size: 8 bytes" not in lines(proc):
        return None, "test_hostcheck.py: the real-compiler cases need a host with 8-byte pointers; this host has another size"
    return cc, None


UNIT_SOURCES = {
    "good": ("good.c", "int good(void) { return 1; }\n"),
    "bad": ("bad.c", "int bad(void) { return ; ) }\n"),
    "flagged": ("flagged.c", "int flagged(void) { return 2; }\n"),
    "hdr": ("hdr.c", "#include <t.h>\nint hdr(void) { return sizeof(Scalars); }\n"),
    "libx": ("sdk/libx.c", "this is not C\n"),
    "asm": ("asm/y.s", "this is not C either\n"),
}


def unit_cases(root: Path, cc: str):
    config = tree(root, "units", UNIT_SOURCES, flags={"flagged": ["-mips1", "-G0", "-nosuchoption"]})
    build = root / "units-build"
    proc = run(config, build, cc)
    out = lines(proc)
    err = need(proc)
    yield "units-count-line", err or same(out[3] if len(out) > 3 else None, "units: 4 compiled, 1 under sdk/ and 1 not C left out")
    yield "units-passed-and-failed", err or same(out[4:6], ["passed: 3", "failed: 1"])
    yield "units-failed-named-last", err or same(out[-1:], ["failed: bad"])
    yield "units-object-of-passing-unit", same((build / "obj" / "good.o").is_file(), True)
    yield "units-flags-not-passed", err or same(row(build, "flagged")[:3], ["flagged", "flagged.c", "passed"])
    yield "units-include-path-has-generated-header", err or same(row(build, "hdr")[2:3], ["passed"])
    yield "units-sdk-not-compiled", same((build / "obj" / "libx.o").exists(), False)
    ok = tree(root, "units-ok", {"good": UNIT_SOURCES["good"]})
    proc = run(ok, root / "units-ok-build", cc)
    yield "units-no-failed-line", need(proc) or same(any(x.startswith("failed: ") and x != "failed: 0" for x in lines(proc)), False)
    yield "units-last-line-when-none-failed", need(proc) or same(lines(proc)[-1], "BIOS: 0 literals in 0 units")


def real_diag_cases(root: Path, cc: str):
    src = "unsigned int f(char *p) { return (unsigned int)p; }\n"
    config = tree(root, "realdiag", {"cast": ("cast.c", src)})
    proc = run(config, root / "realdiag-build", cc)
    got = [x for x in lines(proc) if x.startswith("warning ") and "pointer-to-int-cast" in x]
    yield "real-diag-pointer-to-int-cast", need(proc) or same(len(got), 1)
    yield "real-diag-count", need(proc) or same(got[0].endswith(": 1 in 1 units") if got else None, True)


def real_struct_cases(root: Path, cc: str):
    config = tree(root, "realstruct", {"u": ("u.c", "int u;\n")}, fields=REAL_FIELDS)
    build = root / "realstruct-build"
    proc = run(config, build, cc)
    err = need(proc)
    table = {}
    for row in read(build / "structs.tsv").splitlines():
        cols = row.split("\t")
        table[cols[0]] = cols[1:]
    # Names in order: Arr, Holder, Last, Scalars, WithPtr.
    yield "struct-scalars-keep-layout", err or same(table.get("Scalars"), ["8", "kept", "plain"])
    yield "struct-pointer-then-field-loses-layout", err or same(table.get("WithPtr"), ["8", "lost", "pointer"])
    yield "struct-holding-pointer-struct-by-value", err or same(table.get("Holder"), ["16", "lost", "pointer"])
    yield "struct-with-array-field-is-checked", err or same(table.get("Arr"), ["8", "kept", "plain"])
    yield "struct-pointer-to-struct-field", err or same(table.get("Last"), ["4", "lost", "pointer"])
    yield "struct-table-order", err or same(list(table), ["Arr", "Holder", "Last", "Scalars", "WithPtr"])
    out = lines(proc)
    yield "struct-line-kept", err or same("structs: 2 of 5 keep their layout" in out, True)
    yield "struct-line-with-pointer", err or same("structs with a pointer: 3, of which 3 lose their layout" in out, True)
    yield "struct-line-without-pointer", err or same("structs without a pointer: 2, of which 0 lose their layout" in out, True)
    # A valid table gives a plain struct that loses its layout on no host that the
    # probe has: sizes and alignments of scalars are the same everywhere. The
    # surprise is shown with a fake compiler that rejects one plain struct.
    fake = fake_cc(root, "surprise", lost=["Scalars"])
    proc = run(config, root / "surprise-build", fake)
    yield "struct-surprise-line", need(proc) or same(
        "structs without a pointer: 2, of which 1 lose their layout" in lines(proc), True
    )
    yield "struct-surprise-tsv", same(read(root / "surprise-build" / "structs.tsv").splitlines()[3], "Scalars\t8\tlost\tplain")
    # An opaque type is defined for the probe, so it must not make a plain struct lose its layout.
    typed = "type Opaque size=0x8 align=8\n\nstruct HasType size=0x10\n0x000 u32 a\n0x008 Opaque o\n"
    config = tree(root, "realtype", {"u": ("u.c", "int u;\n")}, fields=typed)
    proc = run(config, root / "realtype-build", cc)
    yield "struct-with-declared-type-keeps-layout", need(proc) or same(read(root / "realtype-build" / "structs.tsv"), "HasType\t16\tkept\tplain\n")


LITERAL_CASES = {
    "ranges": (
        "unsigned a = 0x80000000, b = 0xa0000000, c = 0x1f800000, d = 0x1f801000, e = 0xbfc00000;\n",
        [1, 1, 1, 1, 1],
    ),
    "edges": (
        "unsigned in_[] = {0x80000000, 0x801fffff, 0xa0000000, 0xa01fffff, 0x1f800000, 0x1f8003ff,\n"
        "                  0x1f801000, 0x1f802fff, 0xbfc00000, 0xbfc7ffff};\n"
        "unsigned out[] = {0x7fffffff, 0x80200000, 0x9fffffff, 0xa0200000, 0x1f7fffff, 0x1f800400,\n"
        "                  0x1f800fff, 0x1f803000, 0xbfbfffff, 0xbfc80000};\n",
        [2, 2, 2, 2, 2],
    ),
    "hidden": (
        '/* 0x80000000 */ // 0x80000000\nconst char *s = "0x80000000 \\" 0x80000000";\nint c = \'0x80000000\'; /* 0x1f800000\n 0xbfc00000 */\n',
        [0, 0, 0, 0, 0],
    ),
    "forms": (
        "unsigned v[] = {0X80000000, 0x801FFFFF, 0x80000000U, 0x80000000UL, 0x80000000LL, 0x80000000ul,\n"
        "                0x80000000llu, %d, %du, %s, 0b%s};\n" % (0x80000000, 0x80000000, "0" + oct(0x80000000)[2:], bin(0x80000000)[2:]),
        [11, 0, 0, 0, 0],
    ),
    "names": (
        "int x80000000, _0x80000000, a0xa0000000, v%d, name0x1f800000b;\n"
        "double d[] = {1e5, 0x1p3, %d.0, %d.5f, 1.0e+9, .5};\n"
        "unsigned long long big = 0x8000000000000000;\n" % (0x80000000, 0x80000000, 0x80000000),
        [0, 0, 0, 0, 0],
    ),
    "minus": ("int m = -0x80000000;\nint n = (0x80000000);\n", [2, 0, 0, 0, 0]),
}
RANGE_NAMES = ["main memory", "main memory, uncached", "scratchpad", "ports", "BIOS"]


def literal_cases(root: Path):
    units = {name: (f"{name}.c", text) for name, (text, _) in LITERAL_CASES.items()}
    config = tree(root, "lit", units)
    cc = fake_cc(root, "lit")
    build = root / "lit-build"
    proc = run(config, build, cc)
    err = need(proc)
    table = {}
    for row in read(build / "units.tsv").splitlines():
        cols = row.split("\t")
        table[cols[0]] = cols
    for name, (_, counts) in LITERAL_CASES.items():
        yield f"literal-{name}-units-tsv", err or same(table.get(name, [None] * 6)[5], str(sum(counts)))
    total = [sum(c[i] for _, c in LITERAL_CASES.values()) for i in range(5)]
    in_units = [sum(1 for _, c in LITERAL_CASES.values() if c[i]) for i in range(5)]
    out = lines(proc)
    yield "literal-total-line", err or same(
        f"fixed addresses: {sum(total)} literals in {sum(1 for _, c in LITERAL_CASES.values() if sum(c))} units" in out, True
    )
    for i, name in enumerate(RANGE_NAMES):
        yield f"literal-range-{name}", err or same(f"{name}: {total[i]} literals in {in_units[i]} units" in out, True)
    yield "literal-range-order", err or same([x.split(":")[0] for x in out if x.split(":")[0] in RANGE_NAMES], RANGE_NAMES)


def refusal_cases(root: Path):
    good = {"u": ("u.c", "int u;\n")}
    cc = fake_cc(root, "refuse")

    def refuse(name: str, proc: subprocess.CompletedProcess, named: str):
        err = proc.stderr.strip()
        ok = proc.returncode == 2 and named in err and len(err.splitlines()) == 1 and proc.stdout == ""
        return name, None if ok else f"exit {proc.returncode}, wanted 2 with one line naming {named!r}; stdout {proc.stdout!r}, stderr {proc.stderr!r}"

    missing = root / "nowhere" / "build.toml"
    yield refuse("refuse-missing-config", run(missing, root / "r-build", cc), str(missing))
    broken = write(root / "broken" / "build.toml", "[types\nfields = \n")
    yield refuse("refuse-invalid-toml", run(broken, root / "r-build", cc), str(broken))
    config = tree(root, "nosource", {"u": ("", None)})
    yield refuse("refuse-unit-without-source", run(config, root / "r-build", cc), str(config))
    config = tree(root, "nofile", {"u": ("gone.c", None)})
    yield refuse("refuse-source-missing", run(config, root / "r-build", cc), str(config.parent / "gone.c"))
    config = tree(root, "notypes", good, types="")
    yield refuse("refuse-types-missing", run(config, root / "r-build", cc), str(config))
    config = tree(root, "badfields", good, fields="struct S size=4\n0x000 nosuchtype x\n")
    yield refuse("refuse-field-table-rejected", run(config, root / "r-build", cc), str(config.parent / "t.fields"))
    config = tree(root, "nocc", good)
    gone = root / "no" / "such" / "cc"
    yield refuse("refuse-compiler-missing", run(config, root / "r-build", gone), str(gone))
    failing = fake_cc(root, "failver", version_status=3)
    yield refuse("refuse-compiler-fails-version", run(config, root / "r-build", failing), str(failing))
    nopointer = fake_cc(root, "nopointer", pointer=3)
    yield refuse("refuse-no-pointer-size", run(config, root / "r-build", nopointer), str(nopointer))
    yield "refuse-bad-jobs", None if run(config, root / "r-build", cc, "--jobs", 0).returncode == 2 else "--jobs 0 was accepted"


def calls(path: Path) -> list[list[str]]:
    try:
        return [x.split() for x in path.read_text().splitlines()]
    except OSError:
        return []


def std_cases(root: Path):
    config = tree(root, "std", {"ua": ("ua.c", "int a;\n"), "ub": ("ub.c", "int b;\n")})
    for std in ("gnu89", "c99"):
        log = root / f"std-{std}.log"
        cc = fake_cc(root, f"std-{std}", record=str(log))
        extra = [] if std == "gnu89" else ["--std", std]
        proc = run(config, root / f"std-{std}-build", cc, *extra)
        got = calls(log)
        units = [c for c in got if "-c" in c]
        probes = [c for c in got if "-fsyntax-only" in c]
        pointer = [c for c in probes if c[-1].split("/")[-1].startswith("pointer")]
        layout = [c for c in probes if c[-1].split("/")[-1].startswith("layout_")]
        want = f"-std={std}"
        err = need(proc)
        yield f"std-{std}-every-unit-call", err or same((len(units), all(want in c for c in units)), (2, True))
        yield f"std-{std}-struct-probes", err or same((len(layout), all(want in c for c in layout)), (2, True))
        yield f"std-{std}-pointer-probes", err or same((len(pointer) >= 1, all(want in c for c in pointer)), (True, True))
        yield f"std-{std}-line", err or same(lines(proc)[1], f"language level: {std}")
        yield f"std-{std}-version-call-has-none", same([c for c in got if c == ["--version"]], [])


OFFSET_FIELDS = """struct Plain size=0x4
0x000 u8 a
0x002 u8 b

struct PlainArr size=0x8
0x000 u8 a
0x004 u8 b[4]

struct Other size=0x4
0x000 u8 a
"""


def offset_cases(root: Path):
    config = tree(root, "offset", {"u": ("u.c", "int u;\n")}, fields=OFFSET_FIELDS)
    cc = fake_cc(root, "offset", fail_if_contains=["offsetof(Plain, b) == 2", "offsetof(PlainArr, b[0]) == 4"])
    build = root / "offset-build"
    proc = run(config, build, cc)
    table = read(build / "structs.tsv")
    yield "offset-checked-for-plain-field", need(proc) or same(table.splitlines()[1:2], ["Plain\t4\tlost\tplain"])
    yield "offset-checked-for-array-field-at-first-element", need(proc) or same(table.splitlines()[2:3], ["PlainArr\t8\tlost\tplain"])
    yield "offset-other-struct-kept", need(proc) or same(table.splitlines()[0:1], ["Other\t4\tkept\tplain"])


def selection_cases(root: Path):
    cc = fake_cc(root, "select")
    config = tree(root, "select-a", {"sdkfoo": ("sdkfoo.c", "int a;\n"), "ub": ("sdk/ub.c", "x\n")})
    proc = run(config, root / "select-a-build", cc)
    yield "select-top-level-sdk-prefix-is-compiled", need(proc) or same(lines(proc)[3], "units: 1 compiled, 1 under sdk/ and 0 not C left out")
    config = tree(root, "select-b", {"ua": ("x.C", "int a;\n"), "ub": ("y.c", "int b;\n"), "uc": ("sdk/z.s", "nop\n")})
    proc = run(config, root / "select-b-build", cc)
    yield "select-upper-case-c-is-not-c", need(proc) or same(lines(proc)[3], "units: 1 compiled, 1 under sdk/ and 1 not C left out")


def entries(root: Path, tag: str, rows: list[tuple[str, str]]) -> Path:
    base = root / tag
    toml = '[types]\nfields = "t.fields"\nheader = "t.h"\n'
    for name, source in rows:
        toml += f'\n[[unit]]\nname = "{name}"\nsource = "{source}"\n'
        write(base / source, "int a;\n")
    write(base / "t.fields", FIELDS)
    return write(base / "build.toml", toml)


def duplicate_cases(root: Path):
    cc = fake_cc(root, "dup")
    config = entries(root, "dup-a", [("same", "a.c"), ("other", "o.c"), ("same", "b.c")])
    proc = run(config, root / "dup-a-build", cc)
    err = proc.stderr.strip()
    yield "duplicate-compiled-names-refused", None if proc.returncode == 2 and "same" in err and len(err.splitlines()) == 1 else f"exit {proc.returncode}, stderr {err!r}"
    config = entries(root, "dup-b", [("same", "a.c"), ("same", "sdk/b.c"), ("same", "c.s")])
    proc = run(config, root / "dup-b-build", cc)
    yield "duplicate-name-with-left-out-units-is-fine", need(proc) or same(lines(proc)[3], "units: 1 compiled, 1 under sdk/ and 1 not C left out")


def timeout_cases(root: Path):
    two = {"ua": ("ua.c", "int a;\n"), "ub": ("ub.c", "int b;\n")}
    config = tree(root, "slow", two)
    warn = {"ua": "ua.c:1:1: warning: x [-Wfoo]\n"}
    fake = fake_cc(root, "slow-unit", sleep=["unit"], stderr=warn)
    proc = run(config, root / "slow-unit-build", fake, "--timeout", 1)
    out = lines(proc)
    yield "timeout-unit-fails", need(proc) or same(out[3:6], ["units: 2 compiled, 0 under sdk/ and 0 not C left out", "passed: 0", "failed: 2"])
    yield "timeout-unit-named-last", need(proc) or same(out[-1:], ["failed: ua ub"])
    yield "timeout-unit-has-no-diagnostics", need(proc) or same([x for x in out if x.startswith(("warning ", "error "))], [])
    yield "timeout-unit-tsv", same(read(root / "slow-unit-build" / "units.tsv"), "ua\tua.c\tfailed\t0\t0\t0\nub\tub.c\tfailed\t0\t0\t0\n")
    fake = fake_cc(root, "slow-struct", sleep=["layout"])
    proc = run(config, root / "slow-struct-build", fake, "--timeout", 1)
    yield "timeout-struct-loses-layout", need(proc) or same(
        [x for x in lines(proc) if x.startswith("structs")],
        ["structs: 0 of 2 keep their layout", "structs with a pointer: 1, of which 1 lose their layout", "structs without a pointer: 1, of which 1 lose their layout"],
    )
    yield "timeout-struct-units-still-pass", need(proc) or same(lines(proc)[4:6], ["passed: 2", "failed: 0"])
    fake = fake_cc(root, "slow-pointer", sleep=["pointer"])
    proc = run(config, root / "slow-pointer-build", fake, "--timeout", 1)
    err = proc.stderr.strip()
    yield "timeout-pointer-size-is-an-error", None if proc.returncode == 2 and str(fake) in err and len(err.splitlines()) == 1 and proc.stdout == "" else f"exit {proc.returncode}, stderr {err!r}"
    fake = fake_cc(root, "slow-version", sleep=["version"])
    proc = run(config, root / "slow-version-build", fake, "--timeout", 1)
    err = proc.stderr.strip()
    yield "timeout-version-is-an-error", None if proc.returncode == 2 and str(fake) in err and len(err.splitlines()) == 1 and proc.stdout == "" else f"exit {proc.returncode}, stderr {err!r}"
    proc = run(config, root / "slow-zero-build", fake_cc(root, "fast"), "--timeout", 0)
    yield "timeout-zero-refused", None if proc.returncode == 2 and proc.stderr.strip() else f"exit {proc.returncode}, stderr {proc.stderr!r}"


def groups(root: Path, cc):
    yield whole_output_cases(root)
    yield diagnostic_cases(root)
    yield literal_cases(root)
    yield refusal_cases(root)
    yield std_cases(root)
    yield offset_cases(root)
    yield selection_cases(root)
    yield duplicate_cases(root)
    yield timeout_cases(root)
    if cc:
        yield unit_cases(root, cc)
        yield real_diag_cases(root, cc)
        yield real_struct_cases(root, cc)


def main() -> int:
    failed = 0
    count = 0
    with tempfile.TemporaryDirectory(prefix="hostcheck-test-") as tmp:
        root = Path(tmp)
        cc, problem = host_checked(root)
        if problem:
            print(problem)
            return 2
        for produced in groups(root, cc):
            while True:
                try:
                    name, detail = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a control must report, not crash
                    print(f"FAIL the control itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                count += 1
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
