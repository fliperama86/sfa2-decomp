#!/usr/bin/env python3
"""Controls for hostbuild.py using made-up inputs; no real compiler is needed.

The pure parts (the rename, the names, the tables, the verification, the unit
selection) are called as functions on made-up text. The flow of a whole run
uses a stand-in compiler and a stand-in `nm`, scripts that this file writes:
the compiler hands out assembly that the case prescribes, "assembles" it to a
file that lists the labels, "links" the lists and the names file into one
file, and the `nm` reads that file back, so that a run is checked from the
sources to the last line. The expected values are worked out here from each
fixture, never read back from the tool.
"""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "hostbuild.py"
sys.path.insert(0, str(TOOLS))

import hostbuild as hb  # noqa: E402

Problem = hb.Problem


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def raises(fn, *needles):
    """None when fn raises a Problem whose text has every needle."""
    try:
        fn()
    except Problem as err:
        missing = [n for n in needles if n not in str(err)]
        return None if not missing else f"the error {str(err)!r} does not name {missing}"
    return "no error was raised"


def write(path: Path, text: str) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    return path


# The rename.

COFF_F = (
    "\t.file\t\"f.c\"\n\t.text\n\t.globl\t_f\n\t.def\t_f;\t.scl\t2;\t.type\t32;\t.endef\n_f:\n\tret\n"
)


def func(name: str, body: str = "\tret\n", us: bool = True) -> str:
    n = ("_" if us else "") + name
    return f"\t.globl\t{n}\n\t.def\t{n};\t.scl\t2;\t.type\t32;\t.endef\n{n}:\n{body}"


def rename_cases():
    text, defined = hb.rename_definitions(COFF_F, True)
    yield "rename-function", same(
        (text, defined),
        ("\t.file\t\"f.c\"\n\t.text\n\t.globl\t_impl_f\n\t.def\t_impl_f;\t.scl\t2;\t.type\t32;\t.endef\n_impl_f:\n\tret\n", {"_f"}),
    )
    src = func("a", "\tcall\t_b\n\tret\n") + func("b", "\tcall\t_a\n\tret\n")
    want = func("impl_a", "\tcall\t_b\n\tret\n").replace("_impl_a", "_impl_a") + func("impl_b", "\tcall\t_a\n\tret\n")
    text, defined = hb.rename_definitions(src, True)
    yield "rename-two-functions-calling-each-other", same((text, defined), (want, {"_a", "_b"}))
    src = func("f", "\tmovl\t$_f, %eax\n\tret\n")
    text, _ = hb.rename_definitions(src, True)
    yield "rename-address-taken-keeps-operand", same("movl\t$_f, %eax" in text and "_impl_f:" in text, True)
    src = "\t.globl\t_v\n\t.data\n\t.align 4\n_v:\n\t.long\t1\n" + func("g", "\tmovl\t_v, %eax\n\tret\n")
    text, defined = hb.rename_definitions(src, True)
    yield "rename-global-variable", same(
        (text, defined),
        ("\t.globl\t_impl_v\n\t.data\n\t.align 4\n_impl_v:\n\t.long\t1\n" + func("impl_g", "\tmovl\t_v, %eax\n\tret\n"), {"_v", "_g"}),
    )
    text, defined = hb.rename_definitions("\t.globl\t_x\n\t.comm\t_c, 4, 2\n\t.comm\t_d,8\n\t.globl\t_c\n", True)
    yield "rename-comm", same((text, defined), ("\t.globl\t_x\n\t.comm\t_impl_c, 4, 2\n\t.comm\t_impl_d,8\n\t.globl\t_impl_c\n", {"_c", "_d"}))
    src = (
        "\t.def\t_s;\t.scl\t3;\t.type\t32;\t.endef\n_s:\n\tjmp\tL1\nL1:\n\tret\n" + func("pub", "\tcall\t_s\n\tret\n")
    )
    text, defined = hb.rename_definitions(src, True)
    yield "rename-static-and-local-label-untouched", same(
        (text, defined),
        ("\t.def\t_s;\t.scl\t3;\t.type\t32;\t.endef\n_s:\n\tjmp\tL1\nL1:\n\tret\n" + func("impl_pub", "\tcall\t_s\n\tret\n"), {"_pub"}),
    )
    src = func("h", "\tcall\t_ext\n\tret\n") + "\t.def\t_ext;\t.scl\t2;\t.type\t32;\t.endef\n"
    text, defined = hb.rename_definitions(src, True)
    yield "rename-referenced-only-untouched", same(
        (text, defined),
        (func("impl_h", "\tcall\t_ext\n\tret\n") + "\t.def\t_ext;\t.scl\t2;\t.type\t32;\t.endef\n", {"_h"}),
    )
    # f is defined; f2, xf, f_, _f (no: with the underscore of the target the name is __f) are other symbols.
    src = func("f") + func("f2") .replace("_f2", "_f2") + "\tcall\t_xf\n\tcall\t_f2x\n\t.def\t_xf;\t.scl\t2;\t.type\t32;\t.endef\n"
    src = func("f") + "\t.def\t_f2;\t.scl\t2;\t.type\t32;\t.endef\n\t.def\t_xf;\t.scl\t2;\t.type\t32;\t.endef\n\tcall\t_f2\n\tcall\t_xf\n"
    text, defined = hb.rename_definitions(src, True)
    yield "rename-prefix-and-suffix-names-untouched", same(
        (text, defined),
        (func("impl_f") + "\t.def\t_f2;\t.scl\t2;\t.type\t32;\t.endef\n\t.def\t_xf;\t.scl\t2;\t.type\t32;\t.endef\n\tcall\t_f2\n\tcall\t_xf\n", {"_f"}),
    )
    src = "\t.globl\t_only\n\t.globl\t_def\n" + func("def", "\tcall\t_only\n\tret\n").split("\n", 1)[1]
    text, defined = hb.rename_definitions(src, True)
    yield "rename-globl-without-definition-untouched", same(
        (text, defined),
        ("\t.globl\t_only\n\t.globl\t_impl_def\n" + func("impl_def", "\tcall\t_only\n\tret\n").split("\n", 1)[1], {"_def"}),
    )
    crlf = COFF_F.replace("\n", "\r\n")
    text, defined = hb.rename_definitions(crlf, True)
    yield "rename-crlf", same((text, defined), (COFF_F.replace("_f", "_impl_f").replace("\n", "\r\n"), {"_f"}))
    spaced = "    .globl _f\n   .def   _f ;  .scl 2;  .type 32; .endef\n_f:\n  ret\n"
    text, defined = hb.rename_definitions(spaced, True)
    yield "rename-spaces", same((text, defined), ("    .globl _impl_f\n   .def   _impl_f ;  .scl 2;  .type 32; .endef\n_impl_f:\n  ret\n", {"_f"}))
    mixed = "\t.global\tf\n\t.globl g\n"
    text, defined = hb.rename_definitions(mixed + "f:\ng:\n", False)
    yield "rename-global-spelling", same((text, defined), ("\t.global\timpl_f\n\t.globl impl_g\nimpl_f:\nimpl_g:\n", {"f", "g"}))
    elf = (
        "\t.text\n\t.globl\tf\n\t.type\tf, @function\nf:\n\tcall\tg\n\tret\n\t.size\tf, .-f\n"
        "\t.globl\tcnt\n\t.bss\n\t.type\tcnt, @object\n\t.size\tcnt, 4\ncnt:\n\t.zero\t4\n"
        "\t.type\tg, @function\n"
    )
    want = elf.replace("\tf, ", "\timpl_f, ").replace(".-f", ".-impl_f").replace("globl\tf", "globl\timpl_f").replace("\nf:", "\nimpl_f:")
    want = want.replace("globl\tcnt", "globl\timpl_cnt").replace("\tcnt, ", "\timpl_cnt, ").replace("\ncnt:", "\nimpl_cnt:")
    text, defined = hb.rename_definitions(elf, False)
    yield "rename-elf-type-and-size", same((text, defined), (want, {"f", "cnt"}))
    yield "rename-elf-keeps-call-to-undefined", same("\tcall\tg\n" in text and "\t.type\tg, @function" in text, True)
    text, defined = hb.rename_definitions("\t.globl\tf\nf:\n", False)
    text2, defined2 = hb.rename_definitions("\t.globl\t_f\n_f:\tret\n_g: nop\n", True)
    yield "rename-label-keeps-instruction-on-its-line", same((text2, defined2), ("\t.globl\t_impl_f\n_impl_f:\tret\n_g: nop\n", {"_f"}))
    yield "rename-no-underscore-prefix", same(text, "\t.globl\timpl_f\nimpl_f:\n")
    text, defined = hb.rename_definitions("", True)
    yield "rename-empty", same((text, defined), ("", set()))
    text, defined = hb.rename_definitions("L5:\n\tjmp L5\n", True)
    yield "rename-local-only", same((text, defined), ("L5:\n\tjmp L5\n", set()))
    # Idempotence guard: names already renamed are just other names.
    twice, _ = hb.rename_definitions(hb.rename_definitions(COFF_F, True)[0], True)
    yield "rename-again-renames-again", same("_impl_impl_f:" in twice, True)
    yield "rename-labels-left-none", same(hb.labels_left(hb.rename_definitions(COFF_F, True)[0], {"_f"}), [])
    yield "rename-labels-left-found", same(hb.labels_left(COFF_F, {"_f", "_g"}), ["_f"])
    yield "rename-c-name", same((hb.c_name("_f", True), hb.c_name("f", False), hb.c_name("f", True)), ("f", "f", "f"))


# The names.


def names_cases():
    text = "/* head\n   multi line */\na = 0x10;\n b=32 ; /* c = 0x1; */\nc_2 = 0X1F800000;\n"
    got = hb.read_symbols(text)
    yield "names-read-symbols", same(got, [("a", 0x10, "symbols.ld"), ("b", 32, "symbols.ld"), ("c_2", 0x1F800000, "symbols.ld")])
    yield "names-unreadable-line", raises(lambda: hb.read_symbols("a = 0x10;\nPROVIDE(b = 1);\n"), "symbols.ld")
    names = hb.merge_names([("a", 1, "symbols.ld"), ("a", 1, "unit u"), ("b", 2, "unit u")])
    yield "names-same-address-twice-is-fine", same(names, {"a": 1, "b": 2})
    yield "names-conflict", raises(lambda: hb.merge_names([("a", 0x80000001, "symbols.ld"), ("a", 0x80000002, "unit u")]), "a", "0x80000001", "0x80000002", "unit u")
    yield "names-render-underscore", same(hb.render_names({"b": 0x80000010, "a": 5}, [], True), "_ps1_a = 0x00000005;\n_ps1_b = 0x80000010;\n")
    yield "names-render-none", same(hb.render_names({"a": 0x1F800000}, [], False), "ps1_a = 0x1f800000;\n")
    yield "names-render-alias", same(hb.render_names({"a": 1}, ["z", "d"], True), "_ps1_a = 0x00000001;\n_ps1_d = _impl_d;\n_ps1_z = _impl_z;\n")
    yield "names-redefine-file", same(hb.render_redefine({"b": 2, "a": 1}, ["z", "a"], True), "_a _ps1_a\n_b _ps1_b\n_z _ps1_z\n")
    yield "names-redefine-file-no-underscore", same(hb.render_redefine({"memcpy": 1}, [], False), "memcpy ps1_memcpy\n")


# The unit selection.


def config_tree(root: Path, tag: str, units: str, images: str = "", nonmatching: dict[str, str] | None = None) -> tuple[dict, Path]:
    base = root / tag
    toml = units + images
    config = write(base / "build.toml", toml)
    for rel, text in (nonmatching or {}).items():
        write(base / rel, text)
    for src in ("a.c", "b.c", "c.c", "sdk/s.c", "asm/x.s", "d.c", "e.c", "f.c"):
        write(base / src, "int x;\n")
    return hb.hostcheck.read_config(config), config


def unit(name: str, source: str, functions: list[tuple[str, int]] = (), image: str | None = None, kind: str | None = None) -> str:
    text = f'\n[[unit]]\nname = "{name}"\nsource = "{source}"\n'
    if image:
        text += f'image = "{image}"\n'
    if kind:
        text += f'kind = "{kind}"\n'
    if functions:
        text += "functions = [\n" + "".join(f'  {{ name = "{n}", address = {a:#x}, size = 8 }},\n' for n, a in functions) + "]\n"
    return text


SHA = bytes(range(32)).hex()

IMG_SHA = ["%02x" % (0x10 + k) * 32 for k in range(3)]   # an invented pinned hash for each image: 32 equal bytes

def sha_line(k: int) -> str:
    return f"static const unsigned char port_sha256_{k}[32] = {{ " + ", ".join(["0x%02x" % (0x10 + k)] * 32) + " };\n"


IMAGES = f"""
[[image]]
name = "mod"
slot = 5
address = 0x801e0000
archive = "../x/A.PAC"
sha256 = "{IMG_SHA[0]}"

[[image]]
name = "mod2"
like = "mod"
slot = 6
address = 0x801f0000
archive = "../x/B.PAC"
sha256 = "{IMG_SHA[1]}"
"""


IMAGES_3 = IMAGES + f"""
[[image]]
name = "mod3"
slot = 7
address = 0x80200000
archive = "../x/C.PAC"
sha256 = "{IMG_SHA[2]}"
"""


def selection_cases(root: Path):
    units = (
        unit("ua", "a.c", [("fa", 0x80100000), ("fb", 0x80100010)])
        + unit("us", "sdk/s.c", [("lib", 0x80100100)])
        + unit("ux", "asm/x.s", [("asmf", 0x80100200)], kind="asm")
        + unit("um", "b.c", [("mf", 0x801e0000)], image="mod")
    )
    nm = {
        "n_nonmatching/func_801e0100_mod.c": "int x;\n",
        "n_nonmatching/func_80100300.c": "int y;\n",
        "n_nonmatching/readme.txt": "no",
        "other/func_80100400.c": "not scanned\n",
    }
    config, path = config_tree(root, "sel", units, IMAGES_3, nm)
    sel = hb.select_units(config, path)
    yield "select-jobs", same([(j.name, j.nonmatching) for j in sel.jobs], [("ua", False), ("um", False), ("func_80100300", True), ("func_801e0100_mod", True)])
    yield "select-sdk-and-asm-left-out", same(any(j.name in ("us", "ux") for j in sel.jobs), False)
    yield "select-declared-includes-all-units", same(
        sorted((f.name, f.address, f.image) for f in sel.declared),
        sorted([("fa", 0x80100000, None), ("fb", 0x80100010, None), ("lib", 0x80100100, None), ("asmf", 0x80100200, None), ("mf", 0x801e0000, "mod"),
                ("func_801e0100_mod", 0x801e0100, "mod"), ("func_80100300", 0x80100300, None)]),
    )
    yield "select-like-images-counted", same(sel.like_images, 1)
    yield "select-nonmatching-image-from-name", same([(f.name, f.image) for f in sel.jobs[2].functions + sel.jobs[3].functions], [("func_80100300", None), ("func_801e0100_mod", "mod")])
    config, path = config_tree(root, "sel-dbl", unit("ua", "a.c", [("func_80100300", 0x80100300)]), "", {"n_nonmatching/func_80100300.c": "int y;\n"})
    yield "select-double-definition", raises(lambda: hb.select_units(config, path), "func_80100300", "unit ua", "n_nonmatching")
    config, path = config_tree(root, "sel-dbl-asm", unit("ux", "asm/x.s", [("func_80100300", 0x80100300)], kind="asm"), "", {"n_nonmatching/func_80100300.c": "int y;\n"})
    yield "select-double-definition-with-asm-unit", raises(lambda: hb.select_units(config, path), "func_80100300", "ux")
    config, path = config_tree(root, "sel-img", "", IMAGES, {"n_nonmatching/func_801e0100_nosuch.c": "int y;\n"})
    yield "select-unknown-image-suffix", raises(lambda: hb.select_units(config, path), "nosuch")
    config, path = config_tree(root, "sel-bad", "", "", {"n_nonmatching/func_1234.c": "int y;\n"})
    yield "select-bad-file-name", raises(lambda: hb.select_units(config, path), "func_1234.c")
    config, path = config_tree(root, "sel-like", unit("um", "b.c", image="mod2"), IMAGES)
    yield "select-unit-in-like-image", raises(lambda: hb.select_units(config, path), "um", "mod2")
    config, path = config_tree(root, "sel-noimg", unit("um", "b.c", image="nope"), IMAGES)
    yield "select-unit-in-undeclared-image", raises(lambda: hb.select_units(config, path), "um", "nope")
    config, path = config_tree(root, "sel-stem", unit("func_80100300", "a.c"), "", {"n_nonmatching/func_80100300.c": "int y;\n"})
    yield "select-stem-is-unit-name", raises(lambda: hb.select_units(config, path), "func_80100300")


# The second placements.

PLACE_IMAGES = [
    {"name": "mod", "address": 0x1000, "slot": 5},
    {"name": "mod2", "address": 0x3000, "slot": 6, "like": "mod", "symbols": {"own": 0x3800, "far": 0x9000}},
]
PLACE_DECLARED = [
    hb.Function("f", 0x1000, "mod", "u"), hb.Function("g", 0x1100, "mod", "u"), hb.Function("r", 0x500, None, "ur"),
    hb.Function("h", 0x1800, "mod", "left"),
]
PLACE_SYMBOLS = [("data_in", 0x1400, "s"), ("data_low", 0xfff, "s"), ("data_edge", 0x2fff, "s"), ("data_end", 0x3000, "s"), ("own", 0x1500, "s")]
PLACE_UNITS = {"mod": ["u", "left", "asm_in_mod"], None: ["ur"]}


def placement_cases():
    got = hb.plan_placements(PLACE_IMAGES, PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS)
    yield "place-one-for-the-like-image", same([(p.image, p.first, p.shift, p.suffix) for p in got], [("mod2", "mod", 0x2000, "__mod2")])
    moved = got[0].moved
    yield "place-functions-of-the-first-image-move", same((moved.get("f"), moved.get("g"), moved.get("h")), (0x3000, 0x3100, 0x3800))
    yield "place-resident-function-stays", same("r" in moved, False)
    yield "place-symbols-in-the-range-move", same(moved.get("data_in"), 0x3400)
    yield "place-range-edges", same(("data_low" in moved, moved.get("data_edge"), "data_end" in moved), (False, 0x4fff, False))
    yield "place-own-symbols-win-and-add", same((moved.get("own"), moved.get("far")), (0x3800, 0x9000))
    yield "place-moved-names-are-exactly", same(sorted(moved), ["data_edge", "data_in", "f", "far", "g", "h", "own"])
    left = [dict(PLACE_IMAGES[0]), {**PLACE_IMAGES[1], "leave_out": ["left"]}]
    yield "place-left-out-unit-still-gives-names", same(
        (hb.plan_placements(left, PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS)[0].left_out, hb.plan_placements(left, PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS)[0].moved.get("h")),
        ({"left"}, 0x3800))
    yield "place-left-out-asm-unit-is-known", same(
        hb.plan_placements([PLACE_IMAGES[0], {**PLACE_IMAGES[1], "leave_out": ["asm_in_mod"]}], PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS)[0].left_out, {"asm_in_mod"})
    yield "place-left-out-unknown-unit", raises(lambda: hb.plan_placements([PLACE_IMAGES[0], {**PLACE_IMAGES[1], "leave_out": ["nope"]}], PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS), "nope", "mod2")
    yield "place-below-the-first-is-refused", raises(lambda: hb.plan_placements([PLACE_IMAGES[0], {**PLACE_IMAGES[1], "address": 0x1000}], [], [], PLACE_UNITS), "mod2")
    yield "place-bad-own-symbols", raises(lambda: hb.plan_placements([PLACE_IMAGES[0], {**PLACE_IMAGES[1], "symbols": {"x": "y"}}], [], [], PLACE_UNITS), "image.symbols")
    yield "place-like-of-a-like", raises(lambda: hb.plan_placements([PLACE_IMAGES[0], PLACE_IMAGES[1], {"name": "m3", "address": 0x5000, "slot": 7, "like": "mod2"}], [], [], PLACE_UNITS), "m3")
    yield "place-none-without-like", same(hb.plan_placements([PLACE_IMAGES[0]], PLACE_DECLARED, PLACE_SYMBOLS, PLACE_UNITS), [])
    text, defined = hb.rename_definitions(COFF_F, True, "__mod2")
    yield "place-rename-with-suffix", same((text, defined), (COFF_F.replace("_f", "_impl_f__mod2"), {"_f"}))
    text, _ = hb.rename_definitions("\t.globl\tf\nf:\n\tcall\tf\n", False, "__x")
    yield "place-rename-with-suffix-keeps-calls", same(text, "\t.globl\timpl_f__x\nimpl_f__x:\n\tcall\tf\n")
    yield "place-labels-left-with-suffix", same(hb.labels_left(hb.rename_definitions(COFF_F, True, "__mod2")[0], {"_f"}), [])
    yield "place-redefine-moved", same(
        hb.render_redefine({"a": 1, "b": 2, "c": 3}, ["d"], True, moved={"b", "d", "zz"}, suffix="__m"),
        "_a _ps1_a\n_b _ps1_b__m\n_c _ps1_c\n_d _ps1_d__m\n")
    yield "place-redefine-without-moved-is-plain", same(hb.render_redefine({"a": 1}, [], False, suffix="__m"), "a ps1_a\n")


# The tables.


def inventory_dir(root: Path, tag: str) -> Path:
    base = root / tag
    write(base / "game.tsv", "80100000\t16\tfunc_80100000\t-\t-\t0\t0\topen\n80100010\t16\t-\t-\t-\t0\t0\topen\n80100200\t16\t-\t-\t-\t0\t0\topen\n")
    write(base / "library.tsv", "80100100\t16\tfunc_80100100\tfam\t1\t-\n80100110\t16\t-\tfam\t1\t-\n")
    write(base / "modules.tsv", "A.PAC\t0x5\t801e0000\t16\taaaa\nA.PAC\t0x5\t801e0010\t16\tbbbb\nB.PAC\t0x6\t801f0000\t16\taaaa\n")
    write(base / "contents.tsv", "A.PAC\t0x5\t1\tA.PAC\nB.PAC\t0x6\t1\tB.PAC\n")
    return base


def table_cases(root: Path):
    units = unit("ua", "a.c", [("fa", 0x80100000)]) + unit("ux", "asm/x.s", [("asmf", 0x80100200)], kind="asm") + unit("us", "sdk/s.c", [("libf", 0x80100100)])
    units += unit("um", "b.c", [("mf", 0x801e0000)], image="mod")
    config, path = config_tree(root, "tab", units, IMAGES)
    sel = hb.select_units(config, path)
    inv = hb.read_inventory(inventory_dir(root, "inv"), config)
    yield "tables-inventory-rows", same(
        sorted((r.address, r.image, r.library) for r in inv),
        sorted([(0x80100000, None, False), (0x80100010, None, False), (0x80100200, None, False), (0x80100100, None, True), (0x80100110, None, True),
                (0x801e0000, "mod", False), (0x801e0010, "mod", False), (0x801f0000, "mod2", False)]),
    )
    fa = next(f for f in sel.declared if f.name == "fa")
    mf = next(f for f in sel.declared if f.name == "mf")
    functions, absents = hb.build_tables(sel.images, [(mf, "impl_mf"), (fa, "impl_fa")], inv, sel.declared)
    yield "tables-functions-sorted-by-image-then-address", same(functions, [(-1, 0x80100000, "impl_fa", "fa"), (0, 0x801e0000, "impl_mf", "mf")])
    yield "tables-absents", same(absents, [
        (-1, 0x80100010, "func_80100010", 0),
        (-1, 0x80100100, "libf", 1),
        (-1, 0x80100110, "func_80100110", 1),
        (-1, 0x80100200, "asmf", 0),
        (0, 0x801e0010, "func_801e0010_mod", 0),
        (1, 0x801f0000, "func_801f0000_mod2", 0),
    ])
    lo = hb.Function("lo", 0x80000100, "mod", "u")
    yield "tables-image-sorts-before-address", same(
        [f[3] for f in hb.build_tables(sel.images, [(lo, "impl_lo"), (fa, "impl_fa")], inv, sel.declared)[0]], ["fa", "lo"])
    yield "tables-second-placement-rows-are-listed", same([a for a in absents if a[0] == 1], [(1, 0x801f0000, "func_801f0000_mod2", 0)])
    moved = hb.Function("mf__mod2", 0x801f0000, "mod2", "um")
    yield "tables-second-placement-name-from-moved-function", same(
        [a for a in hb.build_tables(sel.images, [], inv, sel.declared + [moved])[1] if a[0] == 1], [(1, 0x801f0000, "mf__mod2", 0)])
    yield "tables-second-placement-function-leaves-no-absent", same(
        [a for a in hb.build_tables(sel.images, [(moved, "impl_mf__mod2")], inv, sel.declared + [moved])[1] if a[0] == 1], [])
    functions, absents = hb.build_tables(sel.images, [], inv, sel.declared)
    yield "tables-all-absent-without-c", same(len(absents), 8)
    other = hb.Function("again", 0x80100000, None, "uz")
    yield "tables-two-at-one-address", raises(lambda: hb.build_tables(sel.images, [(fa, "impl_fa"), (other, "impl_again")], inv, sel.declared), "fa", "again", "0x80100000")
    yield "tables-same-address-other-image-is-fine", same(
        len(hb.build_tables(sel.images, [(fa, "impl_fa"), (hb.Function("g", 0x80100000, "mod", "u"), "impl_g")], inv, sel.declared)[0]), 2
    )
    text = hb.render_tables(sel.images, [(-1, 0x80100000, "impl_fa", "fa"), (0, 0x801e0000, "impl_mf", "mf")], [(-1, 0x80100100, "libf", 1)], SHA)
    want = (
        '/* Written by hostbuild.py; not to be edited. */\n#include "port_tables.h"\n\n'
        "extern void impl_fa(void);\nextern void impl_mf(void);\n\n"
        + sha_line(0) + sha_line(1) + "\n"
        "const struct port_image port_images[] = {\n"
        '    { "mod", 0x801e0000u, 0, 0x5u, 0, port_sha256_0 },\n'
        '    { "mod2", 0x801f0000u, "mod", 0x6u, 0, port_sha256_1 },\n'
        "};\nconst unsigned port_image_count = 2;\n\n"
        "const struct port_function port_functions[] = {\n"
        '    { 0x80100000u, (void *)impl_fa, "fa", -1 },\n'
        '    { 0x801e0000u, (void *)impl_mf, "mf", 0 },\n'
        "};\nconst unsigned port_function_count = 2;\n\n"
        "const struct port_absent port_absents[] = {\n"
        '    { 0x80100100u, "libf", -1, 1 },\n'
        "};\nconst unsigned port_absent_count = 1;\n\n"
        "const unsigned char port_program_sha256[32] = {\n"
        "    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,\n"
        "    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,\n"
        "    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,\n"
        "    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,\n"
        "};\n"
    )
    yield "tables-render-text", same(text, want)
    archives = hb.read_image_archives(inventory_dir(root, "inv"), config)
    yield "tables-image-archives-read-from-contents", same(archives, {"mod": ["A.PAC"], "mod2": ["B.PAC"]})
    many = root / "inv-many"
    write(many / "game.tsv", "")
    write(many / "library.tsv", "")
    write(many / "modules.tsv", "A.PAC\t0x5\t801e0000\t16\taaaa\nB.PAC\t0x6\t801f0000\t16\taaaa\n")
    write(many / "contents.tsv", "A.PAC\t0x5\t3\tA.PAC,A2.PAC,A3.PAC\nB.PAC\t0x6\t1\tB.PAC\n")
    yield "tables-image-archives-lists-every-carrier", same(hb.read_image_archives(many, config)["mod"], ["A.PAC", "A2.PAC", "A3.PAC"])
    write(many / "contents.tsv", "")
    os.remove(many / "contents.tsv")
    yield "tables-image-archives-without-contents-names-the-first", same(hb.read_image_archives(many, config), {"mod": ["A.PAC"], "mod2": ["B.PAC"]})
    text2 = hb.render_tables(sel.images, [], [], SHA, {"mod": ["A.PAC", "A2.PAC"], "mod2": ["B.PAC"]})
    yield "tables-render-archives", same(
        ('static const char *const port_archives_0[] = { "A.PAC", "A2.PAC", 0 };\n' in text2,
         '    { "mod", 0x801e0000u, 0, 0x5u, port_archives_0, port_sha256_0 },\n' in text2,
         '    { "mod2", 0x801f0000u, "mod", 0x6u, port_archives_1, port_sha256_1 },\n' in text2,
         text2.index("port_archives_1[]") < text2.index("const struct port_image port_images")), (True, True, True, True))
    yield "tables-the-image-table-carries-each-pinned-hash-as-32-bytes", same(
        [text.count(sha_line(k)) for k in range(2)] + ["port_sha256_2" in text], [1, 1, False])
    for tag, bad in (("missing", None), ("short", IMG_SHA[0][:62]), ("not-hex", "g" * 64), ("not-text", 5)):
        broken = {**config, "image": [{k: v for k, v in config["image"][0].items() if k != "sha256"} | ({} if bad is None else {"sha256": bad}), config["image"][1]]}
        yield f"tables-an-image-with-a-{tag}-pinned-hash-is-refused", raises(lambda b=broken: hb.read_images(b, Path("c.toml")), "c.toml", "mod", "sha256")
    yield "tables-baseline-hash-read-lower-case", same(hb.read_baseline_hash({"baseline": {"sha256": SHA.upper()}}, Path("c")), SHA)
    for tag, cfg in (("absent", {}), ("no-section", {"baseline": 3}), ("short", {"baseline": {"sha256": SHA[:62]}}), ("not-hex", {"baseline": {"sha256": "g" * 64}}),
                     ("not-text", {"baseline": {"sha256": 5}})):
        yield f"tables-baseline-hash-{tag}-is-refused", raises(lambda cfg=cfg: hb.read_baseline_hash(cfg, Path("c.toml")), "c.toml", "sha256")
    empty = hb.render_tables([], [], [], SHA)
    yield "tables-render-empty-has-count-zero", same(("port_function_count = 0;" in empty, "port_image_count = 0;" in empty, "{ 0, 0, 0, 0, 0, 0 }" in empty), (True, True, True))
    yield "tables-string-escape", same(hb.c_string('a"b\\c'), '"a\\"b\\\\c"')
    bad = root / "inv-bad"
    write(bad / "game.tsv", "80100000\t16\tx\n")
    write(bad / "library.tsv", "")
    write(bad / "modules.tsv", "")
    yield "tables-inventory-without-image-is-refused", raises(lambda: hb.read_inventory(bad, config), "mod")
    yield "tables-inventory-missing", raises(lambda: hb.read_inventory(root / "nowhere", config), "nowhere")


# The verification.

NAMES = {"fa": 0x80100000, "v": 0x80190000, "pad": 0x1F800010}
GOOD_NM = (
    "80100000 A _ps1_fa\n80190000 A _ps1_v\n1f800010 A _ps1_pad\n00401000 T _impl_fa\n00402000 B _impl_host\n00402000 B _ps1_host\n00403000 T _main\n00404000 T _fa\n"
)


def shown_case():
    import os
    with tempfile.TemporaryDirectory(prefix="hostbuild-shown-") as tmp:
        here = os.getcwd()
        os.chdir(tmp)
        try:
            inside = hb.shown(Path(tmp) / "b" / "x.exe")
            outside = hb.shown(Path(tmp).parent / "elsewhere" / "y.exe")
        finally:
            os.chdir(here)
    return same((inside, outside), ("b/x.exe", str(Path(tmp).parent.resolve() / "elsewhere" / "y.exe") if False else str(Path(tmp).parent / "elsewhere" / "y.exe")))


def verify_cases():
    def check(nm, names=NAMES, aliases=("host",), impls=("fa",), underscore=True):
        return hb.verify_link(nm, dict(names), list(aliases), list(impls), underscore)

    yield "verify-all-good", same(check(GOOD_NM), [])
    yield "verify-name-at-other-address", same(check(GOOD_NM.replace("80190000 A _ps1_v", "80190004 A _ps1_v")), ["ps1_v: at 0x80190004, wanted 0x80190000"])
    yield "verify-name-missing", same(check(GOOD_NM.replace("80190000 A _ps1_v\n", "")), ["ps1_v: not in the linked file"])
    yield "verify-plain-name-at-host-address-is-fine", same(check(GOOD_NM + "00405000 T _pad\n"), [])
    yield "verify-plain-name-absolute-at-ps1-address", same(check(GOOD_NM + "80100000 A _fa\n"), ["fa: a plain game name at the PS1 address 0x80100000"])
    yield "verify-plain-name-in-scratchpad", same(check(GOOD_NM + "1f800010 A _pad\n"), ["pad: a plain game name at the PS1 address 0x1f800010"])
    yield "verify-impl-missing", same(check(GOOD_NM.replace("00401000 T _impl_fa\n", "")), ["impl_fa: not in the linked file"])
    yield "verify-impl-in-main-memory", same(check(GOOD_NM.replace("00401000 T _impl_fa", "80100040 T _impl_fa")), ["impl_fa: at a PS1 address 0x80100040"])
    yield "verify-impl-in-scratchpad", same(check(GOOD_NM.replace("00401000 T _impl_fa", "1f800020 T _impl_fa")), ["impl_fa: at a PS1 address 0x1f800020"])
    yield "verify-alias-copy-elsewhere", same(check(GOOD_NM.replace("00402000 B _ps1_host", "00402004 B _ps1_host")), ["host: host data at 0x402004, its copy at 0x402000"])
    yield "verify-alias-in-ps1-range", same(check(GOOD_NM.replace("00402000", "80100800")), ["host: host data at 0x80100800, its copy at 0x80100800"])
    yield "verify-alias-missing", same(check(GOOD_NM.replace("00402000 B _ps1_host\n", "")), ["host: host data or its copy is not in the linked file"])
    yield "verify-ps1-name-listed-twice-one-wrong", same(check(GOOD_NM + "80100004 A _ps1_fa\n"), ["ps1_fa: at 0x80100000, 0x80100004, wanted 0x80100000"])
    yield "verify-impl-listed-twice-one-in-ps1", same(check(GOOD_NM + "80100040 T _impl_fa\n"), ["impl_fa: at a PS1 address 0x401000"])
    yield "verify-shown-path", shown_case()
    yield "verify-no-underscore", same(check("80100000 A ps1_fa\n80190000 A ps1_v\n1f800010 A ps1_pad\n00401000 T impl_fa\n00402000 B impl_host\n00402000 B ps1_host\n", underscore=False), [])
    yield "verify-boundary-addresses", same(
        (hb.in_ps1(0x801FFFFF), hb.in_ps1(0x80200000), hb.in_ps1(0x1F8003FF), hb.in_ps1(0x1F800400), hb.in_ps1(0x7FFFFFFF)), (True, False, True, False, False)
    )
    yield "verify-two-misses-both-named", same(len(check(GOOD_NM.replace("80190000 A _ps1_v\n", "").replace("00401000 T _impl_fa\n", ""))), 2)
    yield "verify-ignores-lines-it-cannot-read", same(check(GOOD_NM + "                 U _printf\nwarning: junk\n"), [])


# The game-code markers.

MARK_NM = "00401000 T _port_game_text_begin\n00401100 T _port_game_text_end\n00401010 T _impl_fa\n00401020 t _impl_fb\n00500000 T _main\n"


def marker_cases():
    def check(nm, impls=("fa", "fb"), runtime=("_main",), underscore=True):
        return hb.verify_markers(nm, list(impls), list(runtime), underscore)

    yield "markers-all-good", same(check(MARK_NM), [])
    yield "markers-begin-missing", same(check(MARK_NM.replace("00401000 T _port_game_text_begin\n", "")), ["port_game_text_begin: not in the linked file"])
    yield "markers-end-missing", same(check(MARK_NM.replace("00401100 T _port_game_text_end\n", "")), ["port_game_text_end: not in the linked file"])
    yield "markers-begin-not-below-end", same(len(check(MARK_NM.replace("00401100 T _port_game_text_end", "00401000 T _port_game_text_end"))), 1)
    yield "markers-reversed", same(len(check(MARK_NM.replace("00401000 T _port_game_text_begin", "00402000 T _port_game_text_begin"))), 1)
    yield "markers-impl-below-begin", same(check(MARK_NM.replace("00401010 T _impl_fa", "00400ff0 T _impl_fa")), ["impl_fa: at 0x400ff0, outside the game's code 0x401000-0x401100"])
    yield "markers-impl-at-end", same(check(MARK_NM.replace("00401010 T _impl_fa", "00401100 T _impl_fa")), ["impl_fa: at 0x401100, outside the game's code 0x401000-0x401100"])
    yield "markers-impl-above-end", same(len(check(MARK_NM.replace("00401020 t _impl_fb", "00402020 t _impl_fb"))), 1)
    yield "markers-runtime-symbol-inside", same(check(MARK_NM + "00401050 T _rt\n", runtime=("_main", "_rt")), ["_rt: a runtime symbol at 0x401050, inside the game's code 0x401000-0x401100"])
    yield "markers-runtime-symbol-at-begin", same(len(check(MARK_NM + "00401000 t _rt\n", runtime=("_rt",))), 1)
    yield "markers-runtime-symbol-outside-is-fine", same(check(MARK_NM + "00402000 t _rt\n", runtime=("_rt",)), [])
    yield "markers-defined-twice", same(len(check(MARK_NM + "00401200 T _port_game_text_end\n")), 1)
    yield "markers-no-underscore", same(check(MARK_NM.replace(" _", " "), runtime=("main",), underscore=False), [])
    yield "markers-source", same(hb.marker_source("port_game_text_end", True), "\t.text\n\t.globl\t_port_game_text_end\n_port_game_text_end:\n")
    yield "markers-source-no-underscore", same(hb.marker_source("port_game_text_begin", False), "\t.text\n\t.globl\tport_game_text_begin\nport_game_text_begin:\n")
    yield "markers-text-symbols", same(hb.text_symbols("00000000 T _a\n00000010 t _b\n00000000 D _c\n         U _d\n00000020 b _e\n00000000 t .text\n"), ["_a", "_b"])


def marker_flow_cases(root: Path):
    config = run_tree(root, "mark")
    cc, nm = fake(root, "mark", defs=DEFS, rt_defs=["rtfunc"])
    proc = run(root, "mark", config, cc, nm)
    names = [Path(json.loads(x)).name for x in read(root / "mark" / "build" / "link.rsp").splitlines()]
    yield "mark-run-verifies", same((proc.returncode, proc.stderr), (0, ""))
    yield "mark-first-and-last-among-game-objects", same((names[0], names[names.index("um__mod2.o") + 1], names[-3:]), ("port_game_text_begin.o", "port_game_text_end.o", ["main.o", "port_tables.o", "names.ld"]))
    yield "mark-assembly", same(read(root / "mark" / "build" / "gen" / "port_game_text_end.s"), "\t.text\n\t.globl\t_port_game_text_end\n_port_game_text_end:\n")
    cc, nm = fake(root, "mark2", defs=DEFS, rt_defs=["rtfunc"], nm_extra=["00401020 T _rtfunc"])
    config = run_tree(root, "mark2")
    proc = run(root, "mark2", config, cc, nm)
    yield "mark-runtime-symbol-inside-fails-the-link-check", same((proc.returncode, "_rtfunc: a runtime symbol" in proc.stderr, "linked:" in proc.stdout), (1, True, False))


# The whole run, with a stand-in compiler.

FAKE = """#!{python}
import json, re, sys
from pathlib import Path
spec = json.loads({spec!r})
args = sys.argv[1:]
name = Path(sys.argv[0]).name
if name.startswith("fakenm"):
    data = Path(args[0]).read_text().splitlines()
    out, address = [], 0x401000
    labels = {{}}
    for line in data:
        if line.startswith("def "):
            labels[line[4:]] = address
            out.append("%08x T %s" % (address, line[4:]))
            address += 16
    for line in data:
        m = re.match(r"^(\\S+) = 0x([0-9a-f]+);$", line)
        if m:
            out.append("%s A %s" % (m.group(2), m.group(1)))
        m = re.match(r"^(\\S+) = (\\S+);$", line)
        if m and not m.group(2).startswith("0x"):
            out.append("%08x B %s" % (labels[m.group(2)], m.group(1)))
    out += spec.get("nm_extra", [])
    print("\\n".join(out))
    sys.exit(0)
if name.startswith("fakeobjcopy"):
    table = dict(l.split() for l in Path(args[0].split("=", 1)[1]).read_text().splitlines())
    obj = Path(args[1])
    obj.write_text("".join(
        ("%s %s\\n" % (l.split()[0], table.get(l.split()[1], l.split()[1]))) for l in obj.read_text().splitlines()))
    sys.exit(0)
if args == ["--version"]:
    print(spec.get("version", "fakecc 1.0"))
    print("second line")
    sys.exit(0)
with open(spec["log"], "a") as log:
    log.write(" ".join(args) + "\\n")
out = Path(args[args.index("-o") + 1]) if "-o" in args else None
source = Path(args[-1]) if not args[-1].startswith("@") else None
if "-S" in args:
    base = source.name
    us = spec.get("underscore", True)
    if base == "underscore.c":
        out.write_text("\\t.globl\\t%sport_probe_symbol\\n" % ("_" if us else ""))
        sys.exit(0)
    unit = out.stem
    if unit in spec.get("fail", []):
        sys.stderr.write("%s:1:1: warning: w\\n%s:3:5: error: boom in %s\\nmore\\n" % (base, base, unit))
        sys.exit(1)
    text = ""
    for n in spec["defs"].get(unit, []):
        sym = ("_" if us else "") + n
        text += "\\t.globl\\t%s\\n\\t.def\\t%s;\\t.scl\\t2;\\t.type\\t32;\\t.endef\\n%s:\\n\\tcall\\t%s\\n\\tret\\n" % (sym, sym, sym, ("_" if us else "") + spec.get("call", "callee"))
    for c in spec.get("calls", {{}}).get(unit, []):
        text += "\\tcall\\t%s\\n" % (("_" if us else "") + c)
    out.write_text(text)
    sys.exit(0)
if "-c" in args:
    if source.suffix == ".s":
        if source.stem in spec.get("asm_fail", []):
            sys.stderr.write("asm error\\n")
            sys.exit(1)
        labels = re.findall(r"^([A-Za-z_]\\w*):", source.read_text(), re.M)
        refs = re.findall(r"^\\s*call\\s+(\\S+)", source.read_text(), re.M)
        out.write_text("".join("def %s\\n" % l for l in labels) + "".join("ref %s\\n" % r for r in refs))
    else:
        if source.stem in spec.get("rt_fail", []):
            sys.stderr.write("%s:1:1: error: nope\\n" % source.name)
            sys.exit(1)
        out.write_text("".join("def %s\\n" % d for d in spec.get("rt_defs", [])))
    sys.exit(0)
rsp = Path(args[0][1:]).read_text().splitlines()
text = "".join("def %s\\n" % h for h in spec.get("host", []))
for line in rsp:
    text += Path(json.loads(line)).read_text()
defs = {{l[4:] for l in text.splitlines() if l.startswith("def ")}}
ldn = {{m.group(1) for m in re.finditer(r"^(\\S+) = ", text, re.M)}}
for l in text.splitlines():
    if l.startswith("ref ") and l[4:] not in defs | ldn:
        sys.stderr.write("x.o: undefined reference to `%s'\\n" % l[4:])
        sys.exit(1)
for n in ldn & defs:
    sys.stderr.write("ld: multiple definition of `%s'\\n" % n)
    sys.exit(1)
if spec.get("link_fail"):
    sys.stderr.write("x.o: undefined reference to `gone'\\nx.o: undefined reference to `gone'\\ncollect2: error: ld returned 1 exit status\\n")
    sys.exit(1)
out.write_text(text)
"""


def fake(root: Path, tag: str, **spec) -> tuple[Path, Path]:
    log = root / f"{tag}.log"
    spec["log"] = str(log)
    cc, nm, oc = root / f"fakecc-{tag}", root / f"fakenm-{tag}", root / f"fakeobjcopy-{tag}"
    for path in (cc, nm, oc):
        path.write_text(FAKE.format(python=sys.executable, spec=json.dumps(spec)))
        path.chmod(0o755)
    return cc, nm


RUN_UNITS = (
    unit("ua", "a.c", [("fa", 0x80100000), ("fb", 0x80100010)])
    + unit("ub", "b.c", [("fc", 0x80100020)])
    + unit("uc", "c.c", [("fd", 0x80100030)])
    + unit("um", "d.c", [("mf", 0x801e0000)], image="mod")
    + unit("us", "sdk/s.c", [("libf", 0x80100100)])
    + unit("ux", "asm/x.s", [("asmf", 0x80100200), ("callee", 0x80100210)], kind="asm")
)
RUN_TYPES = '[types]\nfields = "t.fields"\nheader = "t.h"\n'


def run_tree(root: Path, tag: str, units: str = RUN_UNITS, symbols: str = "sym_a = 0x80190000;\n/* c */\nsym_b = 0x1f800010;\n", nonmatching=None, baseline: bool = True, images: str = IMAGES) -> Path:
    base = root / tag / "src"
    toml = (f'[baseline]\nsha256 = "{SHA}"\n' if baseline else "") + RUN_TYPES + units + images
    config = write(base / "build.toml", toml)
    write(base / "t.fields", "struct S size=0x4\n0x000 u32 a\n")
    write(base / "symbols.ld", symbols)
    for src in ("a.c", "b.c", "c.c", "d.c", "sdk/s.c", "asm/x.s"):
        write(base / src, "int x;\n")
    for rel, text in (nonmatching or {}).items():
        write(base / rel, text)
    inv = root / tag / "inventory"
    inventory_dir(root, f"{tag}/inventory")
    write(inv / "game.tsv", "80100000\t16\tx\n80100010\t16\tx\n80100020\t16\tx\n80100030\t16\tx\n80100040\t16\tx\n80100200\t16\tx\n")
    write(root / tag / "runtime" / "port_tables.h", "/* made up */\n")
    write(root / tag / "runtime" / "main.c", "int main(void) { return 0; }\n")
    return config


DEFS = {"ua": ["fa", "fb"], "ub": ["fc"], "uc": ["fd"], "um": ["mf"]}


def run(root: Path, tag: str, config: Path, cc: Path, nm: Path, *more) -> subprocess.CompletedProcess:
    argv = [
        sys.executable, str(SCRIPT), "--config", str(config), "--cc", str(cc), "--nm", str(nm), "--objcopy", str(nm).replace("fakenm", "fakeobjcopy"), "--build", str(root / tag / "build"),
        "--runtime", str(root / tag / "runtime"), "--jobs", "2", *map(str, more),
    ]
    return subprocess.run(argv, capture_output=True, text=True, timeout=300)


def read(path: Path) -> str:
    try:
        return path.read_text()
    except OSError:
        return "(missing)"


def flow_cases(root: Path):
    config = run_tree(root, "flow")
    cc, nm = fake(root, "flow", defs=DEFS)
    proc = run(root, "flow", config, cc, nm)
    build = root / "flow" / "build"
    exe = build / "sfa2.exe"
    want = (
        "compiler: fakecc 1.0\n"
        "units: 4 compiled, 0 of them nonmatching, 0 failed\n"
        "like images built: 1\n"
        "functions with C: 6\n"
        "functions without C: 5, library 2, game and modules 3\n"
        "names at PS1 addresses: 11\n"
        "data defined in C, at host addresses: 0\n"
        f"linked: {exe}, verified\n"
    )
    yield "flow-stdout-exact", same((proc.returncode, proc.stdout, proc.stderr), (0, want, ""))
    yield "flow-names-file", same(read(build / "gen" / "names.ld"), "".join(
        f"_ps1_{n} = 0x{a:08x};\n" for n, a in sorted({
            "sym_a": 0x80190000, "sym_b": 0x1F800010, "fa": 0x80100000, "fb": 0x80100010, "fc": 0x80100020, "fd": 0x80100030,
            "mf": 0x801E0000, "mf__mod2": 0x801F0000, "libf": 0x80100100, "asmf": 0x80100200, "callee": 0x80100210}.items())))
    tables = read(build / "gen" / "port_tables.c")
    yield "flow-tables-functions", same([l.strip() for l in tables.splitlines() if l.strip().startswith("{ 0x") and "impl" in l], [
        '{ 0x80100000u, (void *)impl_fa, "fa", -1 },', '{ 0x80100010u, (void *)impl_fb, "fb", -1 },',
        '{ 0x80100020u, (void *)impl_fc, "fc", -1 },', '{ 0x80100030u, (void *)impl_fd, "fd", -1 },',
        '{ 0x801e0000u, (void *)impl_mf, "mf", 0 },', '{ 0x801f0000u, (void *)impl_mf__mod2, "mf__mod2", 1 },'])
    yield "flow-tables-carry-the-configurations-hash", same(
        [l.strip() for l in tables.split("port_program_sha256[32] = {")[1].splitlines()[1:5]],
        [", ".join(f"0x{b:02x}" for b in range(i, i + 8)) + "," for i in range(0, 32, 8)])
    config = run_tree(root, "nobaseline", baseline=False)
    cc2, nm2 = fake(root, "nobaseline", defs=DEFS)
    proc = run(root, "nobaseline", config, cc2, nm2)
    yield "flow-without-baseline-hash-is-refused", same((proc.returncode, proc.stdout, "sha256" in proc.stderr, len(proc.stderr.strip().splitlines())), (2, "", True, 1))
    asm = read(build / "asm" / "ua.s")
    yield "flow-assembly-renamed", same(("_impl_fa:" in asm, "call\t_callee" in asm, "\t_fa:" in asm or "\n_fa:" in asm), (True, True, False))
    rsp = read(build / "link.rsp").splitlines()
    yield "flow-response-file", same(
        [Path(json.loads(x)).name for x in rsp],
        ["port_game_text_begin.o", "ua.o", "ub.o", "uc.o", "um.o", "um__mod2.o", "port_game_text_end.o", "main.o", "port_tables.o", "names.ld"],
    )
    calls = [x.split() for x in read(root / "flow.log").splitlines()]
    units = [c for c in calls if "-S" in c and c[-1].endswith((".c",)) and not c[-1].endswith("underscore.c")]
    flags = hb.COMPILE_FLAGS
    yield "flow-compile-flags", same((len(units), all(c[:len(flags)] == flags and c[len(flags):len(flags) + 3] == ["-S", "-I", str(build / "gen")] for c in units)), (4, True))
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "flow-link-flags", same((len(link), link[0][1:4] if link else None), (1, ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]))
    rt = [c for c in calls if c[:4] == ["-O1", "-Wall", "-Wextra", "-c"]]
    yield "flow-runtime-flags", same(len(rt), 2)
    yield "flow-header-written", same("struct S" in read(build / "gen" / "t.h"), True)
    yield "flow-failed-table-empty", same(read(build / "failed.tsv"), "")

    # The namespace: references are renamed, the names file defines only ps1_ names.
    config = run_tree(root, "flow")
    names_ld = read(build / "gen" / "names.ld")
    yield "flow-redefine-file", same(read(build / "gen" / "redefine.txt"), "".join(
        f"_{n} _ps1_{n}\n" for n in sorted(["sym_a", "sym_b", "fa", "fb", "fc", "fd", "mf", "libf", "asmf", "callee"])))
    yield "flow-names-file-defines-no-plain-name", same([l for l in names_ld.splitlines() if not l.startswith("_ps1_")], [])
    yield "flow-object-references-renamed", same(("ref _ps1_callee" in read(build / "obj" / "ua.o"), "ref _callee" in read(build / "obj" / "ua.o")), (True, False))
    yield "flow-object-definitions-kept", same("def _impl_fa" in read(build / "obj" / "ua.o"), True)
    # The collision itself: a game function named like a function of the host's library.
    mem = RUN_UNITS.replace('name = "callee"', 'name = "memcpy"')
    config = run_tree(root, "flow6", units=mem)
    cc, nm = fake(root, "flow6", defs=DEFS, call="memcpy", host=["_memcpy"])
    proc = run(root, "flow6", config, cc, nm)
    b6 = root / "flow6" / "build"
    yield "collision-run-links-and-verifies", same((proc.returncode, proc.stdout.splitlines()[-1] if proc.stdout else None, proc.stderr), (0, f"linked: {b6 / 'sfa2.exe'}, verified", ""))
    yield "collision-no-plain-memcpy-defined", same(
        [l for l in read(b6 / "gen" / "names.ld").splitlines() if l.startswith(("_memcpy ", "_memcpy="))], [])
    yield "collision-ps1-memcpy-defined", same("_ps1_memcpy = 0x80100210;\n" in read(b6 / "gen" / "names.ld"), True)
    yield "collision-host-name-untouched-in-link", same("def _memcpy" in read(b6 / "sfa2.exe") and "def _ps1_memcpy" not in read(b6 / "sfa2.exe"), True)

    # Relative paths work.
    cc, nm = fake(root, "flow", defs=DEFS)
    rel = subprocess.run(
        [sys.executable, str(SCRIPT), "--config", "flow/src/build.toml", "--cc", str(cc), "--nm", str(nm),
         "--objcopy", str(root / "fakeobjcopy-flow"), "--build", "relbuild", "--runtime", "flow/runtime", "--jobs", "2"],
        capture_output=True, text=True, cwd=root, timeout=300)
    yield "flow-relative-paths", same((rel.returncode, rel.stdout.splitlines()[-1] if rel.stdout else None), (0, "linked: relbuild/sfa2.exe, verified"))

    # Without the underscore.
    config = run_tree(root, "flow-elf")
    cc, nm = fake(root, "flow-elf", defs=DEFS, underscore=False)
    proc = run(root, "flow-elf", config, cc, nm)
    yield "flow-elf-names", same(read(root / "flow-elf" / "build" / "gen" / "names.ld").splitlines()[0], "ps1_asmf = 0x80100200;")
    yield "flow-elf-run", same((proc.returncode, proc.stderr), (0, ""))
    yield "flow-elf-assembly", same("impl_fa:" in read(root / "flow-elf" / "build" / "asm" / "ua.s"), True)

    # --list, a failing unit, a nonmatching function.
    nmfile = {"n_nonmatching/func_80100040.c": "int z;\n"}
    config = run_tree(root, "flow2", nonmatching=nmfile)
    cc, nm = fake(root, "flow2", defs={**DEFS, "func_80100040": ["func_80100040"]}, fail=["ub"])
    proc = run(root, "flow2", config, cc, nm, "--list")
    exe = root / "flow2" / "build" / "sfa2.exe"
    out = proc.stdout.splitlines()
    yield "flow-failed-status-1", same(proc.returncode, 1)
    yield "flow-failed-lines", same(out[1:5], [
        "units: 5 compiled, 1 of them nonmatching, 1 failed", "like images built: 1", "functions with C: 6", "functions without C: 5, library 2, game and modules 3"])
    yield "flow-failed-names-count", same(out[5], "names at PS1 addresses: 12")
    yield "flow-failed-link-still-made", same(out[7], f"linked: {exe}, verified")
    yield "flow-list-failed", same(out[8], "failed: ub: b.c:3:5: error: boom in ub")
    yield "flow-list-absent-lines", same(out[10:], [
        "absent: fc 0x80100020 -", "absent: asmf 0x80100200 -", "absent: func_801e0010_mod 0x801e0010 mod"])
    absent = out[10:]
    yield "flow-list-absent-count-and-no-library", same((len(absent), any("libf" in x for x in absent)), (3, False))
    yield "flow-failed-table", same(read(root / "flow2" / "build" / "failed.tsv"), "ub\tb.c:3:5: error: boom in ub\n")
    yield "flow-nonmatching-is-function-with-c", same("func_80100040" in read(root / "flow2" / "build" / "gen" / "port_tables.c"), True)
    yield "flow-without-list-nothing-extra", same(len(run(root, "flow2", config, cc, nm).stdout.splitlines()), 8)

    # Data defined in C.
    cc, nm = fake(root, "flow3", defs={**DEFS, "ub": ["fc", "hostvar"]})
    config = run_tree(root, "flow3")
    proc = run(root, "flow3", config, cc, nm, "--list")
    out = proc.stdout.splitlines()
    yield "flow-data-line", same((proc.returncode, out[6], out[-1] if out else None), (0, "data defined in C, at host addresses: 1", out[-1] if out else None))
    yield "flow-data-listed", same("data: hostvar" in out, True)
    yield "flow-data-alias-line", same("_ps1_hostvar = _impl_hostvar;\n" in read(root / "flow3" / "build" / "gen" / "names.ld"), True)
    yield "flow-data-names-count-unchanged", same(out[5], "names at PS1 addresses: 11")

    # A data symbol that symbols.ld places is bound there, not aliased.
    cc, nm = fake(root, "flow3b", defs={**DEFS, "ub": ["fc", "sym_a"]})
    config = run_tree(root, "flow3b")
    proc = run(root, "flow3b", config, cc, nm)
    yield "flow-data-in-symbols-is-not-host", same((proc.returncode, proc.stdout.splitlines()[6]), (0, "data defined in C, at host addresses: 0"))

    # Verification misses end the tool with status 1.
    cc, nm = fake(root, "flow4", defs=DEFS, nm_extra=["80100000 A _fa"])
    config = run_tree(root, "flow4")
    proc = run(root, "flow4", config, cc, nm)
    yield "flow-verify-miss", same((proc.returncode, "fa: a plain game name" in proc.stderr, "linked:" in proc.stdout), (1, True, False))
    cc, nm = fake(root, "flow4b", defs=DEFS, link_fail=True)
    config = run_tree(root, "flow4b")
    proc = run(root, "flow4b", config, cc, nm)
    yield "flow-link-failure", same((proc.returncode, proc.stderr.count("undefined reference to `gone'"), "linked:" in proc.stdout), (1, 1, False))
    cc, nm = fake(root, "flow4c", defs=DEFS, rt_fail=["main"])
    config = run_tree(root, "flow4c")
    proc = run(root, "flow4c", config, cc, nm)
    yield "flow-runtime-failure", same((proc.returncode, "main.c" in proc.stderr, "linked:" in proc.stdout), (1, True, False))
    cc, nm = fake(root, "flow4d", defs=DEFS, asm_fail=["ua"])
    config = run_tree(root, "flow4d")
    proc = run(root, "flow4d", config, cc, nm)
    yield "flow-assembler-failure-is-a-failed-unit", same((proc.returncode, proc.stdout.splitlines()[1]), (1, "units: 4 compiled, 0 of them nonmatching, 1 failed"))

    # Refusals, status 2 with one line.
    cc, nm = fake(root, "flow5", defs=DEFS)
    config = run_tree(root, "flow5", symbols="fa = 0x80100004;\n")
    proc = run(root, "flow5", config, cc, nm)
    yield "flow-conflict-refused", same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), "fa" in proc.stderr), (2, "", 1, True))
    config = run_tree(root, "flow5b")
    (root / "flow5b" / "runtime" / "port_tables.h").unlink()
    proc = run(root, "flow5b", config, cc, nm)
    yield "flow-missing-runtime-header-refused", same((proc.returncode, "port_tables.h" in proc.stderr), (2, True))
    config = run_tree(root, "flow5c", symbols="garbage here\n")
    proc = run(root, "flow5c", config, cc, nm)
    yield "flow-bad-symbols-refused", same((proc.returncode, "symbols.ld" in proc.stderr), (2, True))
    config = run_tree(root, "flow5d")
    proc = run(root, "flow5d", config, root / "nowhere" / "cc", nm)
    yield "flow-missing-compiler-refused", same((proc.returncode, "nowhere" in proc.stderr), (2, True))
    config = run_tree(root, "flow5e", nonmatching={"n_nonmatching/func_80100000.c": "int z;\n"}, units=unit("ua", "a.c", [("func_80100000", 0x80100000)]))
    proc = run(root, "flow5e", config, cc, nm)
    yield "flow-double-definition-refused", same((proc.returncode, "n_nonmatching" in proc.stderr and "ua" in proc.stderr), (2, True))
    # Two C functions at one address.
    cc2, nm2 = fake(root, "flow5f", defs={"ua": ["fa", "fb"], "ub": ["fc"]})
    config = run_tree(root, "flow5f", units=unit("ua", "a.c", [("fa", 0x80100000), ("fb", 0x80100000)]))
    proc = run(root, "flow5f", config, cc2, nm2)
    yield "flow-two-at-one-address-refused", same((proc.returncode, "0x80100000" in proc.stderr), (2, True))


def psyz_cases(root: Path):
    """--psyz: gpu.c alone sees PsyZ's headers, the link line gets its libraries, and a bad folder is refused."""
    def tree(tag: str):
        config = run_tree(root, tag)
        write(root / tag / "runtime" / "gpu.c", "int gpu;\n")
        folder = root / tag / "psyzdir"
        (folder / "inc").mkdir(parents=True)
        libs = [write(folder / "libpsyz.a", ""), write(folder / "libSDL3.a", "")]
        write(folder / "psyz.json", json.dumps({"include": str(folder / "inc"), "define": ["__psyz", "EXTRA=1"], "link": [str(libs[0]), str(libs[1]), "-lm"], "commit": "abc123"}))
        return config, folder, libs

    config, folder, libs = tree("ps")
    cc, nm = fake(root, "ps", defs=DEFS)
    proc = run(root, "ps", config, cc, nm, "--psyz", folder)
    out = proc.stdout.splitlines()
    yield "psyz-run-ok-and-psyz-line-after-compiler", same((proc.returncode, out[:2], proc.stderr), (0, ["compiler: fakecc 1.0", "psyz: abc123"], ""))
    calls = [x.split() for x in read(root / "ps.log").splitlines()]
    rt = {Path(c[-1]).name: c for c in calls if c[:4] == ["-O1", "-Wall", "-Wextra", "-c"]}
    inc = str(folder / "inc")
    yield "psyz-gpu-c-gets-the-flags", same(rt["gpu.c"][4:10], ["-DPORT_HAVE_PSYZ", "-D__psyz", "-DEXTRA=1", "-isystem", inc, "-I"])
    yield "psyz-other-runtime-files-do-not", same(["PORT_HAVE_PSYZ" in " ".join(rt["main.c"]), inc in rt["main.c"]], [False, False])
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "psyz-link-line-has-the-libraries-after-the-flags", same(link[0][1:] if link else None, [*hb.LINK_FLAGS, str(libs[0]), str(libs[1]), "-lm", "-o", str(root / "ps" / "build" / "sfa2.exe")])

    config, folder, libs = tree("ps2")
    cc, nm = fake(root, "ps2", defs=DEFS)
    proc = run(root, "ps2", config, cc, nm)
    calls = [x.split() for x in read(root / "ps2.log").splitlines()]
    rt = {Path(c[-1]).name: c for c in calls if c[:4] == ["-O1", "-Wall", "-Wextra", "-c"]}
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "psyz-without-the-option-nothing-changes", same((proc.returncode, "psyz" in proc.stdout, "PORT_HAVE_PSYZ" in " ".join(rt["gpu.c"]), link[0][1:] if link else None),
                                                          (0, False, False, [*hb.LINK_FLAGS, "-o", str(root / "ps2" / "build" / "sfa2.exe")]))

    config, folder, libs = tree("ps3")
    cc, nm = fake(root, "ps3", defs=DEFS)
    (folder / "psyz.json").unlink()
    proc = run(root, "ps3", config, cc, nm, "--psyz", folder)
    yield "psyz-folder-without-psyz-json-is-refused", same((proc.returncode, proc.stdout, "psyz.json" in proc.stderr and "psyzbuild.py" in proc.stderr, len(proc.stderr.strip().splitlines())), (2, "", True, 1))

    config, folder, libs = tree("ps4")
    cc, nm = fake(root, "ps4", defs=DEFS)
    libs[1].unlink()
    proc = run(root, "ps4", config, cc, nm, "--psyz", folder)
    yield "psyz-missing-library-is-refused", same((proc.returncode, proc.stdout, str(libs[1]) in proc.stderr), (2, "", True))

    config, folder, libs = tree("ps5")
    cc, nm = fake(root, "ps5", defs=DEFS)
    (folder / "psyz.json").write_text('{"include": 3}')
    proc = run(root, "ps5", config, cc, nm, "--psyz", folder)
    yield "psyz-malformed-psyz-json-is-refused", same((proc.returncode, proc.stdout, "psyz.json" in proc.stderr), (2, "", True))


IMAGES_SYM = IMAGES + "\n[image.symbols]\nsym_own = 0x801f0200\n"
LIKE_SYMBOLS = "sym_out = 0x80190000;\nsym_own = 0x80190010;\nsym_in = 0x801e0100;\n"
LIKE_UNITS = (
    unit("ur", "a.c", [("r1", 0x80100000)])
    + unit("um", "d.c", [("mf", 0x801e0000), ("mg", 0x801e0010)], image="mod")
)
LIKE_CALLS = {"um": ["mg", "r1", "sym_in", "sym_out", "sym_own", "hv"]}


def like_cases(root: Path):
    config = run_tree(root, "like", units=LIKE_UNITS, symbols=LIKE_SYMBOLS, images=IMAGES_SYM)
    cc, nm = fake(root, "like", defs={"ur": ["r1"], "um": ["mf", "mg", "hv"]}, call="r1", calls=LIKE_CALLS)
    proc = run(root, "like", config, cc, nm, "--list")
    build = root / "like" / "build"
    out = proc.stdout.splitlines()
    yield "like-run-verifies", same((proc.returncode, proc.stderr, out[2], out[-1] if False else out[7]), (0, "", "like images built: 1", f"linked: {build / 'sfa2.exe'}, verified"))
    names = read(build / "gen" / "names.ld").splitlines()
    yield "like-moved-names-in-the-names-file", same([l for l in names if "__mod2" in l], [
        "_ps1_mf__mod2 = 0x801f0000;", "_ps1_mg__mod2 = 0x801f0010;", "_ps1_sym_in__mod2 = 0x801f0100;",
        "_ps1_sym_own__mod2 = 0x801f0200;", "_ps1_hv__mod2 = _impl_hv__mod2;"])
    yield "like-plain-names-unchanged", same(
        [l for l in names if l.startswith(("_ps1_sym_out ", "_ps1_sym_own ", "_ps1_sym_in ", "_ps1_mg "))],
        ["_ps1_mg = 0x801e0010;", "_ps1_sym_in = 0x801e0100;", "_ps1_sym_out = 0x80190000;", "_ps1_sym_own = 0x80190010;"])
    yield "like-names-outside-the-image-do-not-move", same([l for l in names if l.startswith(("_ps1_sym_out__", "_ps1_r1__"))], [])
    first, second = read(build / "obj" / "um.o"), read(build / "obj" / "um__mod2.o")
    yield "like-first-object-references", same(sorted(l for l in first.splitlines() if l.startswith("ref ")), sorted(
        ["ref _ps1_r1", "ref _ps1_r1", "ref _ps1_r1", "ref _ps1_r1", "ref _ps1_mg", "ref _ps1_sym_in", "ref _ps1_sym_out", "ref _ps1_sym_own", "ref _ps1_hv"]))
    yield "like-second-object-references", same(sorted(l for l in second.splitlines() if l.startswith("ref ")), sorted(
        ["ref _ps1_r1", "ref _ps1_r1", "ref _ps1_r1", "ref _ps1_r1", "ref _ps1_mg__mod2", "ref _ps1_sym_in__mod2", "ref _ps1_sym_out", "ref _ps1_sym_own__mod2", "ref _ps1_hv__mod2"]))
    yield "like-second-object-definitions", same(sorted(l for l in second.splitlines() if l.startswith("def ")), ["def _impl_hv__mod2", "def _impl_mf__mod2", "def _impl_mg__mod2"])
    yield "like-first-object-definitions", same(sorted(l for l in first.splitlines() if l.startswith("def ")), ["def _impl_hv", "def _impl_mf", "def _impl_mg"])
    yield "like-second-assembly-from-the-same-compile", same(
        len([x for x in read(root / "like.log").splitlines() if " -S " in f" {x} " and x.endswith("d.c")]), 1)
    tables = read(build / "gen" / "port_tables.c")
    yield "like-tables-second-functions", same([l.strip() for l in tables.splitlines() if "__mod2" in l and l.strip().startswith("{ 0x")], [
        '{ 0x801f0000u, (void *)impl_mf__mod2, "mf__mod2", 1 },', '{ 0x801f0010u, (void *)impl_mg__mod2, "mg__mod2", 1 },'])
    yield "like-redefine-file-moves-only-moved", same(
        sorted(l.split()[0] for l in read(build / "gen" / "redefine__mod2.txt").splitlines() if "__mod2" in l),
        ["_hv", "_mf", "_mg", "_sym_in", "_sym_own"])
    yield "like-listing-names-the-image", same([l for l in out if l.startswith("like:")], ["like: mod2 of mod, shift +0x10000, 4 names move, units left out: -"])
    yield "like-response-file-has-both-objects", same(
        [Path(json.loads(x)).name for x in read(build / "link.rsp").splitlines()][:3], ["port_game_text_begin.o", "um.o", "um__mod2.o"])
    yield "like-verification-covers-the-new-names", same(
        run(root, "like", config, *fake(root, "like-bad", defs={"ur": ["r1"], "um": ["mf", "mg", "hv"]}, call="r1", calls=LIKE_CALLS,
                                        nm_extra=["801f0004 A _ps1_mf__mod2"])).returncode, 1)

    # A unit left out is not linked a second time; its names are still given.
    left = IMAGES_SYM.replace('archive = "../x/B.PAC"', 'archive = "../x/B.PAC"\nleave_out = ["um"]')
    config = run_tree(root, "like2", units=LIKE_UNITS, symbols=LIKE_SYMBOLS, images=left)
    cc, nm = fake(root, "like2", defs={"ur": ["r1"], "um": ["mf", "mg"]}, call="r1", calls={"um": ["mg"]})
    proc = run(root, "like2", config, cc, nm, "--list")
    b2 = root / "like2" / "build"
    yield "like-left-out-runs", same((proc.returncode, proc.stderr), (0, ""))
    yield "like-left-out-no-second-object", same((b2 / "obj" / "um__mod2.o").exists(), False)
    yield "like-left-out-names-and-absents", same(
        ("_ps1_mf__mod2 = 0x801f0000;" in read(b2 / "gen" / "names.ld"), "absent: mf__mod2 0x801f0000 mod2" in proc.stdout, "units left out: um" in proc.stdout), (True, True, True))
    yield "like-left-out-not-in-tables", same("impl_mf__mod2" in read(b2 / "gen" / "port_tables.c"), False)
    cc, nm = fake(root, "like3", defs={"ur": ["r1"], "um": ["mf", "mg", "hv"]}, call="r1", calls={"um": ["mg"]})
    config = run_tree(root, "like3", units=LIKE_UNITS, symbols=LIKE_SYMBOLS, images=left)
    proc = run(root, "like3", config, cc, nm)
    yield "like-left-out-unit-with-data-is-refused", same((proc.returncode, "hv" in proc.stderr and "um" in proc.stderr), (2, True))
    config = run_tree(root, "like4", units=LIKE_UNITS.replace('name = "um"', 'name = "um__mod2"'), symbols=LIKE_SYMBOLS, images=IMAGES_SYM)
    cc, nm = fake(root, "like4", defs={"ur": ["r1"]}, call="r1")
    proc = run(root, "like4", config, cc, nm)
    yield "like-unit-named-like-a-second-object-is-refused", same((proc.returncode, "um__mod2" in proc.stderr), (2, True))


def groups(root: Path):
    yield rename_cases()
    yield placement_cases()
    yield names_cases()
    yield selection_cases(root)
    yield table_cases(root)
    yield verify_cases()
    yield marker_cases()
    yield flow_cases(root)
    yield like_cases(root)
    yield marker_flow_cases(root)
    yield psyz_cases(root)


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostbuild-test-") as tmp:
        for produced in groups(Path(tmp)):
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
