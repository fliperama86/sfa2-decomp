#!/usr/bin/env python3
"""Controls for hostneeds.py using synthetic inputs.

Builds small trees in a temporary directory: a build.toml, a units.tsv, empty
object files, a symbol file, the two inventory tables, and a fake `nm` that
prints prepared lines for each object path it is given. No compiler is used.
The expected values are worked out here from each fixture, never read back
from the tool.
"""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "hostneeds.py"

FAKE_NM = """#!{python}
import json, sys
from pathlib import Path
spec = json.loads((Path(__file__).parent / "nm.json").read_text())
args = sys.argv[1:]
paths = [a for a in args if not a.startswith("-")]
flags = "".join(a[1:] for a in args if a.startswith("-"))
if spec.get("status"):
    sys.stderr.write("fake nm failed\\n")
    sys.exit(spec["status"])
if len(paths) > 500:
    sys.stderr.write("too many arguments\\n")
    sys.exit(1)
for path in paths:
    name = Path(path).name
    for line in spec["objects"].get(name, []):
        print((path + ":" if "A" in flags else "") + line)
    if "g" not in flags:
        print((path + ":" if "A" in flags else "") + "                 U local_only_need")
"""

IMAGES = ["slot04", "0c", "slot04_0c"]
ZERO = "0000000000000000"
BLANK = " " * 16


def T(addr: int, name: str) -> str:
    return f"{addr:016x} T {name}"


def D(name: str, kind: str = "D") -> str:
    return f"{ZERO} {kind} {name}"


def U(name: str) -> str:
    return f"{BLANK} U {name}"


class Tree:
    def __init__(self, base: Path):
        self.base = base
        self.build = base / "build"
        self.config = base / "build.toml"
        self.symbols = base / "symbols.ld"
        self.inventory = base / "inventory"
        self.bin = base / "bin"

    def args(self) -> list[str]:
        return [
            "--build", str(self.build), "--config", str(self.config),
            "--symbols", str(self.symbols), "--inventory", str(self.inventory),
        ]


def put(path: Path, text: str) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    return path


def unit(name: str, source: str, funcs=(), image: str | None = None, state: str = "passed") -> dict:
    return {"name": name, "source": source, "funcs": list(funcs), "image": image, "state": state}


def make(root: Path, tag: str, units=(), images=IMAGES, symbols="", game=(), library=(), nm=None, skip_objects=(), extra_rows=()) -> Tree:
    """Units with a C source outside sdk/ get a row of units.tsv; leftover objects exist for all of them."""
    tree = Tree(root / tag)
    toml = ""
    rows = []
    for u in units:
        toml += f'\n[[unit]]\nname = "{u["name"]}"\nsource = "{u["source"]}"\n'
        if u["image"]:
            toml += f'image = "{u["image"]}"\n'
        toml += "functions = [\n"
        for fname, addr in u["funcs"]:
            toml += f'  {{ name = "{fname}", address = {addr:#x}, size = 4 }},\n'
        toml += "]\n"
        if u["source"].endswith(".c") and not u["source"].startswith("sdk/"):
            if u["state"] is not None:
                rows.append(f"{u['name']}\t{u['source']}\t{u['state']}\t0\t0\t0")
    for name in images:
        toml += f'\n[[image]]\nname = "{name}"\n'
    put(tree.config, toml)
    for extra in extra_rows:
        rows.append(extra)
    put(tree.build / "units.tsv", "".join(r + "\n" for r in rows))
    for row in rows:
        name = row.split("\t")[0]
        if name not in skip_objects:
            put(tree.build / "obj" / f"{name}.o", "")
    put(tree.symbols, symbols)
    put(tree.inventory / "game.tsv", "".join(f"{a}\t8\tx\t-\t-\t0\t0\topen\n" for a in game))
    put(tree.inventory / "library.tsv", "".join(f"{a}\t8\tx\tu\t1\t-\n" for a in library))
    put(tree.bin / "nm", FAKE_NM.format(python=sys.executable))
    (tree.bin / "nm").chmod(0o755)
    put(tree.bin / "nm.json", json.dumps({"objects": {f"{k}.o": v for k, v in (nm or {}).items()}}))
    return tree


def run(tree: Tree, *more, path: str | None = None, build: Path | None = None, args: list[str] | None = None):
    argv = [sys.executable, str(SCRIPT), *(args if args is not None else tree.args()), *map(str, more)]
    if build is not None:
        argv += ["--build", str(build)]
    env = dict(os.environ, PATH=path if path is not None else f"{tree.bin}{os.pathsep}{os.environ['PATH']}")
    return subprocess.run(argv, capture_output=True, text=True, timeout=300, env=env)


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def need(proc):
    if proc.returncode != 0:
        return f"exit {proc.returncode}: {proc.stderr.strip()}"
    return None


def read(path: Path) -> str:
    try:
        return path.read_text()
    except OSError:
        return "(missing)"


def out_rows(tree: Tree) -> tuple[subprocess.CompletedProcess, dict[str, list[str]], str]:
    out = tree.base / "needs.tsv"
    proc = run(tree, "--out", out)
    text = read(out)
    rows = {}
    for line in text.splitlines():
        cells = line.split("\t")
        rows[cells[0]] = cells[1:]
    return proc, rows, text


# The twelve classes, with the reason for each, in one fixture.

A_UNITS = [
    unit("sdk_a", "sdk/a.c", [("LibNamed", 0x80150000), ("LibOther", 0x80150010)]),
    unit("sdk_s", "sdk/s.s", [("LibAsmSdk", 0x80150020)]),
    unit("asm_u", "asm/x.s", [("AsmFn", 0x80100000)]),
    unit("bad", "bad.c", [("FailFn", 0x80110000)], state="failed"),
    unit("absent_c", "absent.c", [("AbsentFn", 0x80105000)], state=None),
    unit("game_c", "game.c", [("done_fn", 0x80120000)]),
    unit("mod_c", "mod.c", [("func_80130000_slot04", 0x80130000)], image="slot04"),
    unit("third", "third.c"),
]
A_SYMBOLS = """/* a comment */
FailFn = 0x80160000;
old_name = 0x80120000;
lib_by_addr = 0x80150010;
lib_plain = 0x80170000;
pad_left = 0x1f800064;
pad_img_slot04 = 0x1f800070;
mod_data_slot04_0c = 0x80140000;
cnt_0c = 0x80141000;
st_slot04 = 0x80142000;
res_var = 0x80180000;
in_slot04_mid = 0x80181000;
"""
A_NM = {
    "game_c": [
        T(0x80120000, "done_fn"), D("res_def"), D("dup", "B"),
        U("LibNamed"), U("AsmFn"), U("FailFn"), U("mystery"), U("old_name"),
        U("lib_by_addr"), U("lib_plain"), U("func_80118900"), U("res_var"), U("func_80150020"), U("func_80170100"),
        U("weakdef"), U("vdef"), U("lowt"),
    ],
    "mod_c": [
        T(0x80130000, "func_80130000_slot04"), D("lowdef", "d"), D("dup", "B"),
        U("func_80130010_slot04"), U("func_801e0000"), U("pad_left"), U("mod_data_slot04_0c"),
        U("res_var"), U("pad_img_slot04"), U("in_slot04_mid"), U("cnt_0c"), U("st_slot04"), U("gone"),
    ],
    "third": [
        D("gone", "T"), D("weakdef", "W"), D("vdef", "V"), T(0x10, "sub_def"), D("lowt", "t"),
        U("LibNamed"), U("LibAsmSdk"), U("res_var"), U("AbsentFn"),
        f"{ZERO} U withaddr", f"{BLANK} w weakund", "third.o:", "nm: warning: something", "garbage", "",
    ],
    "bad": [U("leftover_need")],
}
A_ROWS = [
    ("AbsentFn", "unit not compiled", "80105000", "-", "1", "-"),
    ("AsmFn", "assembly", "80100000", "-", "1", "-"),
    ("FailFn", "unit not compiled", "80110000", "-", "1", "-"),
    ("LibAsmSdk", "library by name", "80150020", "-", "1", "-"),
    ("LibNamed", "library by name", "80150000", "-", "2", "-"),
    ("cnt_0c", "module data", "80141000", "0c", "1", "-"),
    ("func_80118900", "game function", "80118900", "-", "1", "-"),
    ("func_80130010_slot04", "module function", "80130010", "slot04", "1", "-"),
    ("func_80150020", "library by address", "80150020", "-", "1", "LibAsmSdk"),
    ("func_80170100", "library by address", "80170100", "-", "1", "-"),
    ("func_801e0000", "function elsewhere", "801e0000", "-", "1", "-"),
    ("in_slot04_mid", "resident data", "80181000", "-", "1", "-"),
    ("lib_by_addr", "library by address", "80150010", "-", "1", "LibOther"),
    ("lib_plain", "library by address", "80170000", "-", "1", "-"),
    ("mod_data_slot04_0c", "module data", "80140000", "slot04_0c", "1", "-"),
    ("mystery", "unknown", "-", "-", "1", "-"),
    ("old_name", "C under another name", "80120000", "-", "1", "done_fn"),
    ("pad_img_slot04", "scratchpad data", "1f800070", "slot04", "1", "-"),
    ("pad_left", "scratchpad data", "1f800064", "-", "1", "-"),
    ("res_var", "resident data", "80180000", "-", "3", "-"),
    ("st_slot04", "module data", "80142000", "slot04", "1", "-"),
]
A_STDOUT = """objects: 3 of 4 units
defined: 10 names
needed: 21 names
library by name: 2 names, needed by 2 units
assembly: 1 names, needed by 1 units
unit not compiled: 2 names, needed by 2 units
unknown: 1 names, needed by 1 units
C under another name: 1 names, needed by 1 units
library by address: 4 names, needed by 1 units
game function: 1 names, needed by 1 units
module function: 1 names, needed by 1 units
function elsewhere: 1 names, needed by 1 units
scratchpad data: 2 names, needed by 1 units
module data: 3 names, needed by 1 units
resident data: 2 names, needed by 3 units
library by address: 3 named, 1 unnamed
unknown: mystery
"""
A_REASON = {
    "AbsentFn": "a C unit absent from units.tsv did not pass",
    "AsmFn": "declared by an assembly unit",
    "FailFn": "declared by a failed unit, though the symbol file gives an address in library.tsv",
    "LibAsmSdk": "declared by an sdk unit whose source is not C",
    "LibNamed": "declared by an sdk unit, also a row of library.tsv; needed by two units",
    "cnt_0c": "image 0c, symbol file address",
    "func_80118900": "no image, address from the name is a row of game.tsv",
    "func_80130010_slot04": "image and func_, address a row of game.tsv does not matter",
    "func_80150020": "func_ name, library row, an sdk unit declares there: other name, counted named",
    "func_80170100": "func_ name, library row, no sdk unit: the unnamed one",
    "func_801e0000": "no image, func_, no row anywhere",
    "in_slot04_mid": "image name in the middle is no image",
    "lib_by_addr": "no image, library row, an sdk unit declares there",
    "lib_plain": "no image, library row, no sdk unit there, but a name of its own: counted named",
    "mod_data_slot04_0c": "longest image wins over 0c",
    "mystery": "no declaration, no symbol, no digits",
    "old_name": "a passed resident unit declares done_fn at its address, also a library row",
    "pad_img_slot04": "image, data, scratchpad",
    "pad_left": "no image, scratchpad",
    "res_var": "no image, ordinary address; needed by three units",
    "st_slot04": "image slot04",
}


def a_tree(root: Path, tag: str = "a") -> Tree:
    return make(
        root, tag, A_UNITS, symbols=A_SYMBOLS,
        game=["80118900", "80130010", "1f800070"],
        library=["80150000", "80150010", "80170000", "80160000", "80120000", "80150020", "80170100"],
        nm=A_NM,
    )


def class_cases(root: Path):
    tree = a_tree(root)
    proc, rows, text = out_rows(tree)
    yield "class fixture runs", need(proc)
    for row in A_ROWS:
        got = rows.get(row[0])
        yield f"{row[0]}: {row[1]} ({A_REASON[row[0]]})", same(got, list(row[1:])) if got else "no row in --out"
    yield "no other needed names", same(sorted(rows), sorted(r[0] for r in A_ROWS))
    yield "whole standard output", same(proc.stdout, A_STDOUT)
    yield "whole --out file", same(text, "".join("\t".join(r) + "\n" for r in A_ROWS))
    yield "nothing on stderr", same(proc.stderr, "")
    # The same tree with no unknown name: the last line is left out and the other lines stay.
    quiet = make(root, "quiet", [unit("p", "p.c")], nm={"p": [D("a")]})
    q = run(quiet)
    want = (
        "objects: 1 of 1 units\ndefined: 1 names\nneeded: 0 names\n"
        + "".join(f"{c}: 0 names, needed by 0 units\n" for c in CLASSES)
        + "library by address: 0 named, 0 unnamed\n"
    )
    yield "nothing needed: twelve zero lines, no unknown line", same(q.stdout, want) if q.returncode == 0 else need(q)
    yield "nothing needed: --out is empty", same(read(quiet.base / "none.tsv") if run(quiet, "--out", quiet.base / "none.tsv").returncode == 0 else "?", "")


CLASSES = (
    "library by name", "assembly", "unit not compiled", "unknown", "C under another name",
    "library by address", "game function", "module function", "function elsewhere",
    "scratchpad data", "module data", "resident data",
)


# Rules in groups: name -> (class, address, image, other); each name is needed by one unit.


def group(root: Path, tag: str, expect: dict, units=(), **kw):
    names = sorted(expect)
    tree = make(root, tag, [*units, unit("p", "p.c")], nm={"p": [U(n) for n in names]}, **kw)
    proc, rows, _ = out_rows(tree)
    yield f"{tag}: runs", need(proc)
    for name in names:
        want = list(expect[name][:3]) + ["1", expect[name][3]]
        got = rows.get(name)
        yield f"{tag}: {name} is {expect[name][0]}", same(got, [want[0], *want[1:]]) if got else "no row in --out"
    yield f"{tag}: nothing else needed", same(sorted(rows), names)
    unknown = sorted(n for n in names if expect[n][0] == "unknown")
    last = proc.stdout.splitlines()[-1] if proc.stdout else ""
    yield f"{tag}: unknown line lists the unknown names in order of name", same(last, ("unknown: " + " ".join(unknown)) if unknown else last)


def X(cls, addr="-", image="-", other="-"):
    return (cls, addr, image, other)


def decl_cases(root: Path):
    units = [
        unit("sdk_a", "sdk/a.c", [("Both", 0x80150000), ("SdkFail", 0x80150100)]),
        unit("asm_a", "asm/a.s", [("Both", 0x80150000), ("AsmFail", 0x80100000), ("AsmDup", 0x80100100)]),
        unit("bad_a", "bad.c", [("AsmFail", 0x80100000), ("SdkFail", 0x80150100), ("FailAlias", 0x80110100)], state="failed"),
        unit("good_a", "good.c", [("cdup", 0x80100100), ("okfn", 0x80110100)]),
        unit("sdkish", "sdk_x/foo.c", [("SdkishFn", 0x80111000)], state="failed"),
        unit("cs", "lib.c.s", [("CsFn", 0x80112000)]),
        unit("sdkdotc", "sdk.c", [("SdkDotC", 0x80113000)], state="failed"),
    ]
    expect = {
        "Both": X("library by name", "80150000"),
        "SdkFail": X("library by name", "80150100"),
        "AsmFail": X("assembly", "80100000"),
        "AsmDup": X("assembly", "80100100"),
        "FailAlias": X("unit not compiled", "80110100"),
        "SdkishFn": X("unit not compiled", "80111000"),
        "CsFn": X("assembly", "80112000"),
        "SdkDotC": X("unit not compiled", "80113000"),
    }
    yield from group(root, "decl", expect, units, library=["80150000"])


def order_cases(root: Path):
    units = [
        unit("r1", "r1.c", [("real_game", 0x80118A00), ("real_res", 0x80132000)]),
        unit("m1", "m1.c", [("real_mod", 0x80131000)], image="slot04"),
    ]
    expect = {
        "old_game": X("C under another name", "80118a00", "-", "real_game"),
        "func_80131000_slot04": X("C under another name", "80131000", "slot04", "real_mod"),
        "func_80132000": X("C under another name", "80132000", "-", "real_res"),
        "func_80157090": X("library by address", "80157090"),
        "func_1f800010_slot04": X("module function", "1f800010", "slot04"),
        "func_1f800020": X("function elsewhere", "1f800020"),
        "pad_game": X("game function", "1f800030"),
        "gamerow_slot04": X("module data", "80118908", "slot04"),
        "func_80118908_slot04": X("module function", "80118908", "slot04"),
        "libimg_slot04": X("module data", "80134100", "slot04"),
        "bnd_lo": X("scratchpad data", "1f800000"),
        "bnd_hi": X("scratchpad data", "1f8003ff"),
        "bnd_below": X("resident data", "1f7fffff"),
        "bnd_above": X("resident data", "1f800400"),
    }
    symbols = """old_game = 0x80118a00;
pad_game = 0x1f800030;
gamerow_slot04 = 0x80118908;
libimg_slot04 = 0x80134100;
bnd_lo = 0x1f800000;
bnd_hi = 0x1f8003ff;
bnd_below = 0x1f7fffff;
bnd_above = 0x1f800400;
"""
    yield from group(
        root, "order", expect, units, symbols=symbols,
        game=["80118a00", "80118908", "1f800030"], library=["80157090", "80134100", "80118a00"],
    )


def image_match_cases(root: Path):
    units = [
        unit("s_img1", "s1.c", [("other_fn", 0x80134000)], image="slot04_0c"),
        unit("s_res", "s2.c", [("res_fn", 0x80134100)]),
        unit("s_mod", "s3.c", [("real_a", 0x80134200)], image="slot04"),
        unit("z1", "z1.c", [("Zed", 0x80134300)]),
        unit("z2", "z2.c", [("Alpha", 0x80134300)]),
        unit("f1", "f1.c", [("failed_fn", 0x80134400)], state="failed"),
        unit("sd", "sdk/sd.c", [("sdk_fn", 0x80134500)]),
        unit("sd2", "sdk/sd2.c", [("sdk_fn2", 0x80134600)]),
        unit("a1", "asm/a1.s", [("asm_fn", 0x80134700)]),
        unit("selfu", "selfu.c", [("selfy", 0x80134800)]),
        unit("sdz", "sdk/sdz.s", [("Zed2", 0x80134900)]),
        unit("sda", "sdk/sda.c", [("Alpha2", 0x80134900)]),
    ]
    expect = {
        "x_slot04": X("module data", "80134000", "slot04"),
        "rx": X("resident data", "80134200"),
        "y_slot04": X("module data", "80134100", "slot04"),
        "alias_slot04": X("C under another name", "80134200", "slot04", "real_a"),
        "res_alias": X("C under another name", "80134100", "-", "res_fn"),
        "zz": X("C under another name", "80134300", "-", "Alpha"),
        "fz": X("resident data", "80134400"),
        "sz": X("resident data", "80134500"),
        "sz2": X("library by address", "80134600", "-", "sdk_fn2"),
        "az": X("resident data", "80134700"),
        "selfy": X("resident data", "80134800"),
        "lz": X("library by address", "80134900", "-", "Alpha2"),
    }
    symbols = "".join(
        f"{n} = 0x{expect[n][1]};\n"
        for n in ("x_slot04", "rx", "y_slot04", "alias_slot04", "res_alias", "zz", "fz", "sz", "sz2", "az", "lz")
    )
    yield from group(root, "image-match", expect, units, symbols=symbols, library=["80134600", "80134900"])


def address_cases(root: Path):
    units = [unit("p1", "p1.c", [("decl_a", 0x80140100)])]
    expect = {
        "decl_a": X("resident data", "80140100"),
        "func_80150300": X("function elsewhere", "80150400"),
        "D_80160000": X("resident data", "80160000"),
        "data_80161111_extra_bits": X("resident data", "80161111"),
        "D_8016000": X("unknown"),
        "D_801600000": X("unknown"),
        "D_801600000_x": X("unknown"),
        "D_8016ABCD": X("unknown"),
        "D2_80160000": X("unknown"),
        "_80160000": X("unknown"),
        "my_func_80160000": X("unknown"),
        "UP": X("resident data", "8016abcd"),
        "ghost": X("unknown"),
        "after": X("resident data", "80162000"),
        "lowaddr": X("resident data", "00001234"),
        "twice": X("resident data", "80163000"),
        "unk_b": X("unknown"),
        "unk_a": X("unknown"),
    }
    symbols = """decl_a = 0x80140200;
func_80150300 = 0x80150400;
UP = 0X8016ABCD;
/*
ghost = 0x80199999;
*/
after = 0x80162000;
lowaddr = 0x00001234;
twice = 0x80163000;
twice = 0x80164000;
"""
    yield from group(root, "address", expect, units, symbols=symbols)


def image_cases(root: Path):
    expect = {
        "D_80170001_slot04": X("module data", "80170001", "slot04"),
        "D_80170002_slot04_0c": X("module data", "80170002", "slot04_0c"),
        "D_80170003_0c": X("module data", "80170003", "0c"),
        "D_80170004_slot04_mid": X("resident data", "80170004"),
        "D_80170005_xslot04": X("resident data", "80170005"),
        "slot04": X("resident data", "80170006"),
        "_slot04": X("module data", "80170007", "slot04"),
        "D_80170008_x_0c": X("module data", "80170008", "0c"),
    }
    yield from group(root, "image", expect, symbols="slot04 = 0x80170006;\n_slot04 = 0x80170007;\n")


# Objects, units and nm.


def unit_cases(root: Path):
    tree = make(
        root, "units",
        [unit("a", "a.c"), unit("b", "b.c", state="failed"), unit("c", "c.c")],
        extra_rows=["orphan\torphan.c\tpassed\t0\t0\t0"],
        nm={"a": [U("need_a"), U("need_both")], "b": [U("leftover_need")], "c": [U("need_both"), D("made_c")],
            "orphan": [U("made_c"), U("orph_need")]},
    )
    proc, rows, _ = out_rows(tree)
    yield "passed units only: failed leftover not read", same(sorted(rows), ["need_a", "need_both", "orph_need"]) if proc.returncode == 0 else need(proc)
    yield "objects line counts passed and all rows", same(proc.stdout.splitlines()[0], "objects: 3 of 4 units")
    yield "need_both counts two units", same(rows.get("need_both", [None, None, None, None])[3], "2")
    missing = make(root, "missing-obj", [unit("a", "a.c"), unit("b", "b.c")], skip_objects={"b"})
    yield from refusal("passed unit without its object", run(missing), "b.o")
    many = 650
    names = [f"u{i:04d}" for i in range(many)]
    units = [unit(n, f"{n}.c") for n in names]
    big = make(root, "many", units, nm={n: [U("shared"), U("own_" + n)] for n in names})
    proc, rows, _ = out_rows(big)
    yield "650 objects, all read in batches", same((len(rows), rows.get("shared", ["", "", "", ""])[3]), (many + 1, str(many))) if proc.returncode == 0 else need(proc)
    yield "650 objects: objects line", same(proc.stdout.splitlines()[0] if proc.stdout else "", f"objects: {many} of {many} units")
    colon = make(root, "colon", [unit("a", "a.c")], nm={"a": [U("x_need")]})
    moved = colon.base / "bu:ild"
    colon.build.rename(moved)
    proc = run(colon, build=moved)
    yield "object path with a colon", same(proc.stdout.splitlines()[2:3], ["needed: 1 names"]) if proc.returncode == 0 else need(proc)


def refusal(name: str, proc: subprocess.CompletedProcess, token: str):
    lines = proc.stderr.splitlines()
    detail = None
    if proc.returncode != 2:
        detail = f"exit {proc.returncode}, stderr {proc.stderr!r}"
    elif len(lines) != 1:
        detail = f"stderr is not one line: {proc.stderr!r}"
    elif token not in lines[0]:
        detail = f"stderr {lines[0]!r} does not name {token!r}"
    elif proc.stdout:
        detail = f"standard output not empty: {proc.stdout!r}"
    yield f"refuses: {name}", detail


def refusal_cases(root: Path):
    def fresh(tag: str, **kw) -> Tree:
        return make(root, tag, [unit("a", "a.c")], nm={"a": [U("x")]}, **kw)

    t = fresh("r1")
    yield from refusal("missing build folder", run(t, build=t.base / "nowhere"), "nowhere")
    t = fresh("r2")
    (t.build / "units.tsv").unlink()
    yield from refusal("missing units.tsv", run(t), "units.tsv")
    for label, text in (
        ("too few columns", "a\ta.c\tpassed\t0\t0\n"),
        ("too many columns", "a\ta.c\tpassed\t0\t0\t0\t1\n"),
        ("third column neither word", "a\ta.c\tmaybe\t0\t0\t0\n"),
    ):
        t = fresh("r3")
        put(t.build / "units.tsv", text)
        yield from refusal(f"units.tsv {label}", run(t), "units.tsv")
    t = fresh("r4")
    t.config.unlink()
    yield from refusal("missing configuration", run(t), "build.toml")
    t = fresh("r5")
    put(t.config, "[[unit]\nname = ")
    yield from refusal("invalid TOML", run(t), "build.toml")
    t = fresh("r6")
    t.symbols.unlink()
    yield from refusal("missing symbol file", run(t), "symbols.ld")
    for table in ("game.tsv", "library.tsv"):
        t = fresh("r7")
        (t.inventory / table).unlink()
        yield from refusal(f"missing {table}", run(t), table)
        t = fresh("r8")
        put(t.inventory / table, "80118900\t8\tx\n80118zz0\t8\tx\n")
        yield from refusal(f"{table} address not hexadecimal", run(t), table)
    t = fresh("r9")
    empty = t.base / "nobin"
    empty.mkdir()
    yield from refusal("nm not found", run(t, path=str(empty)), "nm")
    t = fresh("r10")
    put(t.bin / "nm.json", json.dumps({"objects": {}, "status": 3}))
    yield from refusal("nm ends with a non-zero status", run(t), "nm")
    t = fresh("r11")
    yield "a run of the fresh tree is fine", need(run(t))


def groups(root: Path):
    yield class_cases(root)
    yield decl_cases(root)
    yield order_cases(root)
    yield image_match_cases(root)
    yield address_cases(root)
    yield image_cases(root)
    yield unit_cases(root)
    yield refusal_cases(root)


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostneeds-test-") as tmp:
        root = Path(tmp)
        for produced in groups(root):
            while True:
                try:
                    name, detail = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a control must report, not crash
                    print(f"FAIL the control itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
