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

    python3 test_hostbuild.py [--cc CROSS_CC [--run PREFIX]]

With --cc the overrides are also built and run for real: hostbuild.py makes a made-up game of four
units and a small runtime of this file's own with the cross compiler CC, and the Windows program is started
(PREFIX is a command prefix that starts one, as in test_hostlaunch.py; without it the program is started directly).
The cases show that an override runs in place of the unit's C for a call from another unit and for a call from
the unit that defines the function, that the unit's other functions keep their own C, that a call through a
pointer declared without parameters reaches a callee that reads its parameter, and that both placements of
a `like` image run the override with their own moved names. Without --cc these cases are not run and the
file says so. A program that cannot be started ends this file with status 2. No run is repeated: a run that
ends with status 1 and prints nothing fails its case, and a note at the end says how many did.
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
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
        items = [(f + (None,))[:3] for f in functions]
        ordered = sorted(a for _, a, _ in items)
        rows = []
        for n, a, size in items:
            if size is None:
                later = [b for b in ordered if b > a]
                size = (later[0] - a) if later and later[0] - a <= 0x40 else 8
            rows.append(f'  {{ name = "{n}", address = {a:#x}, size = {size} }},\n')
        text += "functions = [\n" + "".join(rows) + "]\n"
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
        "library_nonmatching/func_80100500.c": "the library's C: left out\n",
    }
    config, path = config_tree(root, "sel", units, IMAGES_3, nm)
    sel = hb.select_units(config, path)
    yield "select-jobs", same([(j.name, j.nonmatching) for j in sel.jobs], [("ua", False), ("um", False), ("func_80100300", True), ("func_801e0100_mod", True)])
    yield "select-sdk-and-asm-left-out", same(any(j.name in ("us", "ux") for j in sel.jobs), False)
    yield "select-library-nonmatching-left-out", same(
        (any(j.name == "func_80100500" for j in sel.jobs), any(f.name == "func_80100500" for f in sel.declared)), (False, False))
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


# The reviewed table of sweep rows.


def entry(**over):
    base = dict(image="mod", address=0x1000, function="f", function_address=0x1010, data_bytes=16, evidence="two tables", data_symbol=None)
    base.update(over)
    return hb.SweepEntry(base["image"], base["address"], base["function"], base["function_address"], base["data_bytes"], base["evidence"], base["data_symbol"])


def sweep_table_cases(root: Path):
    R, F = hb.Row, hb.Function
    rows = [R(0x1000, "mod", False, 0x30), R(0x2000, "mod", False, 0x40), R(0x3000, "mod", False, 0x10), R(0x1000, "mod2", False, 0x30), R(0x2000, "mod2", False, 0x40)]
    f = F("f", 0x1010, "mod", "u", 0x20)
    g = F("g", 0x2000 - 0x40 + 0x40 - 0x40 + 0x40, "mod", "u", 0x40) if False else F("g", 0x2000 - 0x0, "mod", "u", 0x40)
    f2 = F("f__mod2", 0x1010, "mod2", "u", 0x20)
    decl = [f, g, f2]

    def check(entries, with_c=(f, g, f2), rows=rows, declared=decl):
        return hb.verify_sweep_rows(entries, rows, list(with_c), list(declared))

    kept, misses = check([entry()])
    yield "sweep-listed-data-before-is-dropped", same(([(r.image, r.address) for r in kept if r.address == 0x1000], misses), ([("mod2", 0x1000)], []))
    kept, misses = check([entry(image="mod2", function="f__mod2")])
    yield "sweep-second-placement-is-listed-on-its-own", same(([(r.image, r.address) for r in kept if r.address == 0x1000], misses), ([("mod", 0x1000)], []))
    yield "sweep-not-listed-keeps-every-row", same(check([])[0], rows)
    # The owner's fixtures: nothing is inferred from what lies near a row.
    missing_before = [R(0x4000, "mod", False, 0x20)]
    c_after = F("later", 0x4010, "mod", "u", 8)
    yield "sweep-missing-function-before-a-later-c-function-keeps-its-stop", same(check([], with_c=[c_after], rows=missing_before, declared=[c_after])[0], missing_before)
    # Rule 2: a row that begins inside the range of a built unit.
    S = hb.Span
    spans = [S("u1", "mod", 0x1000, 0x1100, {0x1000, 0x1040}), S("u2", None, 0x3000, 0x3040, {0x3000}), S("u1", "mod2", 0x4000, 0x4100, {0x4000, 0x4040})]
    rows2 = [R(0x1000, "mod", False, 0x40), R(0x1020, "mod", False, 0x20), R(0x1040, "mod", False, 0x40), R(0x1100, "mod", False, 0x10),
             R(0x0fff, "mod", False, 0x10), R(0x3020, None, False, 0x10), R(0x4020, "mod2", False, 0x10), R(0x4040, "mod2", False, 0x10)]
    kept, dropped = hb.split_span_rows(rows2, spans)
    yield "span-row-inside-a-unit-is-dropped", same([(r.address, sp.unit, sp.start, sp.end) for r, sp in dropped], [
        (0x1020, "u1", 0x1000, 0x1100), (0x3020, "u2", 0x3000, 0x3040), (0x4020, "u1", 0x4000, 0x4100)])
    yield "span-declared-function-address-is-normal", same([r.address for r in kept], [0x1000, 0x1040, 0x1100, 0x0fff, 0x4040])
    yield "span-other-image-does-not-count", same(hb.split_span_rows([R(0x1020, "mod2", False, 0x10)], [S("u1", "mod", 0x1000, 0x1100, {0x1000})])[1], [])
    yield "span-no-spans-keeps-the-row", same(hb.split_span_rows([R(0x1020, "mod", False, 0x10)], [])[1], [])
    yield "span-start-itself-counts", same(len(hb.split_span_rows([R(0x2000, "mod", False, 0x10)], [S("u3", "mod", 0x2000, 0x2100, {0x2040})])[1]), 1)
    yield "span-contiguous", same((hb.contiguous([F("a", 0x100, "m", "u", 0x10), F("b", 0x110, "m", "u", 8)]), hb.contiguous([F("b", 0x110, "m", "u", 8), F("a", 0x100, "m", "u", 0x10)])), (True, True))
    yield "span-gap-is-not-contiguous", same(hb.contiguous([F("a", 0x100, "m", "u", 0x10), F("b", 0x118, "m", "u", 8)]), False)
    yield "span-overlap-is-not-contiguous", same(hb.contiguous([F("a", 0x100, "m", "u", 0x10), F("b", 0x108, "m", "u", 8)]), False)
    yield "span-one-function-is-contiguous", same(hb.contiguous([F("a", 0x100, "m", "u", 0x10)]), True)

    # The unknown prefix: bytes in front of a later function with C that nothing owns keep their stop.
    prefix = [R(0x4000, "mod", False, 0x20), R(0x4000, "mod2", False, 0x20)]
    later = [F("later", 0x4010, "mod", "u", 8), F("later__mod2", 0x4010, "mod2", "u", 8)]
    yield "sweep-unknown-prefix-keeps-its-stop", same(check([], with_c=later, rows=prefix[:1], declared=later)[0], prefix[:1])
    yield "sweep-unknown-prefix-keeps-its-stop-second-placement", same(check([], with_c=later, rows=prefix[1:], declared=later)[0], prefix[1:])
    yield "sweep-an-entry-for-another-row-does-not-drop-the-prefix", same(
        [(r.image, r.address) for r in check([entry()], with_c=later + [f, f2], rows=prefix + rows, declared=later + decl)[0] if r.address == 0x4000],
        [("mod", 0x4000), ("mod2", 0x4000)])

    def one(**over):
        return check([entry(**over)])[1]

    miss = lambda **o: (len(one(**o)), "mod 0x1000" in (one(**o) or [""])[0] or "mod 0x" in (one(**o) or [""])[0])
    yield "sweep-miss-no-such-row", same(len(one(address=0x1004, function_address=0x1010, data_bytes=12)), 1)
    yield "sweep-miss-no-such-row-in-this-image", same(len(one(image="mod3")), 1)
    yield "sweep-miss-function-without-c", same(len(check([entry()], with_c=[g, f2])[1]), 1)
    yield "sweep-miss-function-of-another-image", same(len(check([entry(function="f__mod2")])[1]), 1)
    yield "sweep-miss-function-at-another-address", same(len(one(function_address=0x1014, data_bytes=20)), 1)
    yield "sweep-miss-data-bytes-wrong", same(len(one(data_bytes=12)), 1)
    yield "sweep-miss-data-bytes-zero", same(len(check([entry(address=0x1010, function_address=0x1010, data_bytes=0)], rows=rows + [R(0x1010, "mod", False, 0x30)])[1]), 1)
    yield "sweep-miss-function-at-the-row-end", same(len(check([entry(address=0x1000, function="f", function_address=0x1010, data_bytes=16)], rows=[R(0x1000, "mod", False, 0x10)])[1]), 1)
    yield "sweep-miss-function-before-the-row", same(len(check([entry(address=0x1020, function_address=0x1010, data_bytes=-16)], rows=rows + [R(0x1020, "mod", False, 0x10)])[1]), 1)
    yield "sweep-miss-a-function-at-the-row-start", same(len(check([entry()], declared=decl + [F("own", 0x1000, "mod", "asm")])[1]), 1)
    yield "sweep-miss-another-function-between", same(len(check([entry()], declared=decl + [F("mid", 0x1008, "mod", "asm")])[1]), 1)
    yield "sweep-another-function-in-another-image-is-no-miss", same(check([entry()], declared=decl + [F("mid", 0x1008, "mod3", "asm")])[1], [])
    symbols = {"data_x": 0x1000, "data_y": 0x1004}
    yield "sweep-data-symbol-at-the-row-is-fine", same(hb.verify_sweep_rows([entry(data_symbol="data_x")], rows, [f, g, f2], decl, symbols)[1], [])
    yield "sweep-data-symbol-elsewhere-is-a-miss", same(len(hb.verify_sweep_rows([entry(data_symbol="data_y")], rows, [f, g, f2], decl, symbols)[1]), 1)
    yield "sweep-data-symbol-unknown-is-a-miss", same(len(hb.verify_sweep_rows([entry(data_symbol="data_z")], rows, [f, g, f2], decl, symbols)[1]), 1)
    yield "sweep-data-symbol-without-symbols-is-a-miss", same(len(hb.verify_sweep_rows([entry(data_symbol="data_x")], rows, [f, g, f2], decl)[1]), 1)
    yield "sweep-no-data-symbol-rests-on-the-evidence", same(hb.verify_sweep_rows([entry()], rows, [f, g, f2], decl, {})[1], [])
    yield "sweep-misses-name-the-entry", same("mod 0x1000" in one(data_bytes=12)[0], True)
    yield "sweep-every-miss-is-reported", same(len(check([entry(data_bytes=12), entry(address=0x2000, function="g", function_address=0x2000, data_bytes=0)])[1]), 2)
    yield "sweep-a-missed-entry-drops-nothing", same(check([entry(data_bytes=12)])[0], rows)

    # The file.
    def table(text, name="t.toml"):
        return write(root / "sweeptab" / name, text)

    good = (
        '[[row]]\nimage = "mod"\naddress = 0x1000\nfunction = "f"\nfunction_address = 0x1010\n'
        'data_bytes = 16\nevidence = "two tables"\n'
    )
    got = hb.read_sweep_rows(table(good))
    yield "sweep-file-read", same([(e.image, e.address, e.function, e.function_address, e.data_bytes, e.evidence) for e in got], [("mod", 0x1000, "f", 0x1010, 16, "two tables")])
    yield "sweep-file-resident", same(hb.read_sweep_rows(table(good.replace('"mod"', '"resident"')))[0].image, None)
    yield "sweep-file-missing-drops-nothing", same(hb.read_sweep_rows(root / "sweeptab" / "nowhere.toml"), [])
    yield "sweep-file-none-drops-nothing", same(hb.read_sweep_rows(None), [])
    yield "sweep-file-empty-drops-nothing", same(hb.read_sweep_rows(table("# nothing\n", "e.toml")), [])
    for key in ("image", "address", "function", "function_address", "data_bytes", "evidence"):
        text = "".join(l + "\n" for l in good.splitlines() if not l.startswith(key + " ") and l != "[[row]]" or l == "[[row]]")
        yield f"sweep-file-field-missing-{key}", raises(lambda t=text: hb.read_sweep_rows(table(t, f"m{key}.toml")), key)
    yield "sweep-file-unknown-field", raises(lambda: hb.read_sweep_rows(table(good + 'kind = "data-before"\n', "k.toml")), "kind")
    yield "sweep-file-data-symbol", same(hb.read_sweep_rows(table(good + 'data_symbol = "data_x"\n', "s.toml"))[0].data_symbol, "data_x")
    yield "sweep-file-listed-twice", raises(lambda: hb.read_sweep_rows(table(good + "\n" + good, "d.toml")), "twice")
    yield "sweep-file-empty-evidence", raises(lambda: hb.read_sweep_rows(table(good.replace("two tables", "  "), "ev.toml")), "evidence")
    yield "sweep-file-bad-type", raises(lambda: hb.read_sweep_rows(table(good.replace("0x1000", '"x"'), "ty.toml")), "type")
    yield "sweep-file-not-toml", raises(lambda: hb.read_sweep_rows(table("[[row\n", "bad.toml")), "bad.toml")



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
        '    { 0x80100000u, (void *)impl_fa, "fa", -1, 0 },\n'
        '    { 0x801e0000u, (void *)impl_mf, "mf", 0, 0 },\n'
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


def image_flow_cases(root: Path):
    config = run_tree(root, "img")
    cc, nm = fake(root, "img", defs=DEFS)
    proc = run(root, "img", config, cc, nm)
    build = root / "img" / "build"
    yield "image-run-verifies", same((proc.returncode, proc.stdout.splitlines()[-1], proc.stderr), (0, f"linked: {build / 'sfa2.exe'}, verified", ""))
    yield "image-hole-assembly-written", same(read(build / "gen" / "hole.s"), '\t.section\t.hole,"b"\n\t.space\t0x1ef000\n')
    # a linker that does not honour one of the settings: the check names it and the tool ends with status 1
    for tag, ignore, skip, want in (
        ("base", ["-Wl,--image-base="], False, "image base 0x400000, wanted 0x10000"),
        ("reloc", ["-Wl,--disable-reloc-section"], False, "relocations are not stripped (flag)"),
        ("holestart", ["-Wl,--section-start=.hole="], False, "the first section is .text, wanted .hole"),
        ("textstart", ["-Wl,--section-start=.text="], False, "the filler ends at 0x11000, below 0x200000"),
        ("nohole", [], True, "the first section is .text, wanted .hole"),
        ("dynamic", ["-Wl,--disable-dynamicbase"], False, "the image is relocatable (dynamic base)"),
    ):
        config = run_tree(root, "img-" + tag)
        cc, nm = fake(root, "img-" + tag, defs=DEFS, ignore=ignore, skip_hole=skip)
        proc = run(root, "img-" + tag, config, cc, nm)
        yield f"image-flow-{tag}-not-honoured-ends-with-status-1", same((proc.returncode, want in proc.stderr, "linked:" in proc.stdout), (1, True, False))


def marker_flow_cases(root: Path):
    config = run_tree(root, "mark")
    cc, nm = fake(root, "mark", defs=DEFS, rt_defs=["rtfunc"])
    proc = run(root, "mark", config, cc, nm)
    names = [Path(json.loads(x)).name for x in read(root / "mark" / "build" / "link.rsp").splitlines()]
    yield "mark-run-verifies", same((proc.returncode, proc.stderr), (0, ""))
    yield "mark-first-and-last-among-game-objects", same((names[1], names[names.index("um__mod2.o") + 1], names[-3:]), ("port_game_text_begin.o", "port_game_text_end.o", ["main.o", "port_tables.o", "names.ld"]))
    yield "mark-assembly", same(read(root / "mark" / "build" / "gen" / "port_game_text_end.s"), "\t.text\n\t.globl\t_port_game_text_end\n_port_game_text_end:\n")
    cc, nm = fake(root, "mark2", defs=DEFS, rt_defs=["rtfunc"], nm_extra=["00401020 T _rtfunc"])
    config = run_tree(root, "mark2")
    proc = run(root, "mark2", config, cc, nm)
    yield "mark-runtime-symbol-inside-fails-the-link-check", same((proc.returncode, "_rtfunc: a runtime symbol" in proc.stderr, "linked:" in proc.stdout), (1, True, False))


# The linked file's header (verify_image) on invented headers.

def pe_bytes(base=0x10000, sections=None, stripped=True, reloc_dir=False, dynamic=False, align=0x1000, headers=0x400, bits=0x10B) -> bytes:
    """A PE image header with the given sections [(name, address, size, raw size, flags)]; the default is the good one."""
    if sections is None:
        sections = [(".hole", 0x11000, 0x1EF000, 0, 0xC0000080), (".text", 0x200000, 0x1800, 0x1800, 0x60000020),
                    (".data", 0x202000, 0x100, 0x200, 0xC0000040), (".bss", 0x203000, 0x80, 0, 0xC0000080)]
    data = bytearray(0x400)
    data[0:2] = b"MZ"
    data[0x3C:0x40] = (0x80).to_bytes(4, "little")
    pe, opt = 0x80, 0x80 + 24
    data[pe:pe + 4] = b"PE\0\0"
    data[pe + 4:pe + 6] = (0x14C).to_bytes(2, "little")
    data[pe + 6:pe + 8] = len(sections).to_bytes(2, "little")
    data[pe + 20:pe + 22] = (224).to_bytes(2, "little")
    data[pe + 22:pe + 24] = (0x127 if stripped else 0x126).to_bytes(2, "little")
    data[opt:opt + 2] = bits.to_bytes(2, "little")
    data[opt + 28:opt + 32] = base.to_bytes(4, "little")
    data[opt + 32:opt + 36] = align.to_bytes(4, "little")
    data[opt + 60:opt + 64] = headers.to_bytes(4, "little")
    data[opt + 70:opt + 72] = (0x140 if dynamic else 0x100).to_bytes(2, "little")
    if reloc_dir:
        data[opt + 96 + 40:opt + 96 + 44] = (0x5000).to_bytes(4, "little")
        data[opt + 96 + 44:opt + 96 + 48] = (0x100).to_bytes(4, "little")
    for i, (name, address, size, raw, flags) in enumerate(sections):
        o = opt + 224 + 40 * i
        data[o:o + 8] = name.encode().ljust(8, b"\0")
        data[o + 8:o + 12] = size.to_bytes(4, "little")
        data[o + 12:o + 16] = ((address - base) & 0xFFFFFFFF).to_bytes(4, "little")
        data[o + 16:o + 20] = raw.to_bytes(4, "little")
        data[o + 36:o + 40] = flags.to_bytes(4, "little")
    return bytes(data)


def pe_for_link(args: list[str], objects: list[str], ignore: list[str], skip_hole: bool) -> bytes:
    """What a linker that honoured the flags would write, for the stand-in compiler: the header of the image."""
    args = [a for a in args if not any(a.startswith(i) for i in ignore)]
    base = 0x400000
    starts: dict[str, int] = {}
    for a in args:
        if a.startswith("-Wl,--image-base="):
            base = int(a.split("=", 1)[1], 16)
        if a.startswith("-Wl,--section-start="):
            name, address = a.split("=", 2)[1:]
            starts[name] = int(address, 16)
    hole = any(o.endswith("hole.o") for o in objects) and not skip_hole
    text = starts.get(".text", base + 0x1000)
    sections = [(".text", text, 0x1800, 0x1800, 0x60000020), (".data", text + 0x2000, 0x100, 0x200, 0xC0000040)]
    if hole and ".hole" in starts:
        sections.insert(0, (".hole", starts[".hole"], text - starts[".hole"], 0, 0xC0000080))   # sections are listed in address order
    elif hole:
        sections.append((".hole", text + 0x3000, 0x1000, 0, 0xC0000080))   # no address given: the linker puts the orphan section last
    return pe_bytes(base=base, sections=sections, stripped="-Wl,--disable-reloc-section" in args, reloc_dir="-Wl,--disable-reloc-section" not in args,
                    dynamic="-Wl,--disable-dynamicbase" not in args)


def image_cases():
    def check(**kw):
        return hb.verify_image(pe_bytes(**kw))

    yield "image-good-header", same(check(), [])
    yield "image-link-flags-are-the-settings", same(hb.IMAGE_FLAGS, ["-Wl,--image-base=0x10000", "-Wl,--disable-reloc-section", "-Wl,--section-start=.hole=0x11000", "-Wl,--section-start=.text=0x200000"])
    yield "image-hole-source", same(hb.hole_source(), '\t.section\t.hole,"b"\n\t.space\t0x1ef000\n')
    yield "image-wrong-base", same(check(base=0x400000, sections=[(".hole", 0x401000, 0x1EF000, 0, 0xC0000080), (".text", 0x5F0000, 0x100, 0x200, 0x60000020)]),
                                   ["image base 0x400000, wanted 0x10000", "the first section begins at 0x401000, wanted 0x11000"])
    yield "image-relocations-not-stripped-flag", same(check(stripped=False), ["relocations are not stripped (flag)"])
    yield "image-relocation-table-present", same(check(reloc_dir=True), ["the image has a relocation table"])
    yield "image-dynamic-base", same(check(dynamic=True), ["the image is relocatable (dynamic base)"])
    yield "image-section-alignment", same(check(align=0x10000), [
        "section alignment 0x10000, wanted 0x1000",
        "a gap or an overlap between .hole (0x11000, 0x1ef000 bytes) and .text (0x200000)",
        "a gap or an overlap between .text (0x200000, 0x1800 bytes) and .data (0x202000)",
        "a gap or an overlap between .data (0x202000, 0x100 bytes) and .bss (0x203000)"])
    yield "image-headers-too-large", same(check(headers=0x1200), ["the headers (0x1200 bytes) do not fit the first page"])
    sec = lambda *rows: [(".hole", 0x11000, 0x1EF000, 0, 0xC0000080), *rows]
    yield "image-first-section-is-not-the-filler", same(check(sections=[(".text", 0x11000, 0x1EF000, 0, 0xC0000080), (".data", 0x200000, 0x100, 0x200, 0xC0000040)]), ["the first section is .text, wanted .hole"])
    yield "image-filler-begins-late", same(check(sections=[(".hole", 0x12000, 0x1EE000, 0, 0xC0000080), (".text", 0x200000, 0x100, 0x200, 0x60000020)]), ["the first section begins at 0x12000, wanted 0x11000"])
    yield "image-filler-has-file-bytes", same(check(sections=[(".hole", 0x11000, 0x1EF000, 0x200, 0xC0000040), (".text", 0x200000, 0x100, 0x200, 0x60000020)]), ["the filler section is not uninitialized"])
    yield "image-gap-after-the-filler", same(check(sections=[(".hole", 0x11000, 0x1EF000, 0, 0xC0000080), (".text", 0x210000, 0x100, 0x200, 0x60000020)]),
                                            ["a gap or an overlap between .hole (0x11000, 0x1ef000 bytes) and .text (0x210000)"])
    yield "image-overlap-after-the-filler", same(check(sections=[(".hole", 0x11000, 0x1EF000, 0, 0xC0000080), (".text", 0x1FF000, 0x100, 0x200, 0x60000020)]),
                                                 ["a gap or an overlap between .hole (0x11000, 0x1ef000 bytes) and .text (0x1ff000)", "section .text begins at 0x1ff000, below 0x200000"])
    yield "image-filler-short", same(check(sections=[(".hole", 0x11000, 0x1E0000, 0, 0xC0000080), (".text", 0x1F1000, 0x100, 0x200, 0x60000020)]),
                                     ["the filler ends at 0x1f1000, below 0x200000", "section .text begins at 0x1f1000, below 0x200000"])
    yield "image-gap-between-real-sections", same(check(sections=sec((".text", 0x200000, 0x1800, 0x1800, 0x60000020), (".data", 0x205000, 0x100, 0x200, 0xC0000040))),
                                                  ["a gap or an overlap between .text (0x200000, 0x1800 bytes) and .data (0x205000)"])
    yield "image-one-section", same(check(sections=[(".hole", 0x11000, 0x1EF000, 0, 0xC0000080)]), ["1 sections: the filler and the program's own are needed"])
    yield "image-not-32-bit", same(hb.verify_image(pe_bytes(bits=0x20B)), ["the file is not a 32-bit PE image"])
    yield "image-no-dos-header", same(hb.verify_image(b"\x7fELF" + bytes(100)), ["the file has no DOS header"])
    yield "image-short-file", same(hb.verify_image(b"MZ"), ["the file has no DOS header"])
    yield "image-no-pe-header", same(hb.verify_image(b"MZ" + bytes(0x3E) + (0x80).to_bytes(4, "little") + bytes(0x100)), ["the file has no PE header"])
    yield "image-pe-offset-past-the-file", same(hb.verify_image(pe_bytes()[:0x60]), ["the file has no PE header"])
    yield "image-section-table-cut", same(hb.verify_image(pe_bytes()[:0x80 + 24 + 224 + 60]), ["4 sections: the filler and the program's own are needed"])


# The whole run, with a stand-in compiler.

FAKE = """#!{python}
import json, re, sys
from pathlib import Path
spec = json.loads({spec!r})
args = sys.argv[1:]
name = Path(sys.argv[0]).name
if name.startswith("fakenm"):
    data = Path(args[0]).read_bytes().decode("latin-1").splitlines()
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
    if args[0].endswith(".o") and Path(args[0]).stem in spec.get("nm_fail", []):
        sys.exit(1)
    if args[0].endswith(".o"):
        out += ["         U %s" % l[4:] for l in data if l.startswith("ref ")]
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
        out.write_text("".join("def %s\\n" % d for d in spec.get("rt_defs", []) + spec.get("rt_only", {{}}).get(source.stem, [])))
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
for m in re.finditer(r"^(\\S+) = (_\\S+);$", text, re.M):
    if m.group(2) not in defs:
        sys.stderr.write("ld: names.ld:1: undefined symbol `%s' referenced in expression\\ncollect2: error: ld returned 1 exit status\\n" % m.group(2))
        sys.exit(1)
if spec.get("link_fail"):
    sys.stderr.write("x.o: undefined reference to `gone'\\nx.o: undefined reference to `gone'\\ncollect2: error: ld returned 1 exit status\\n")
    sys.exit(1)
sys.path.insert(0, {tools!r})
import test_hostbuild as T
pe = T.pe_for_link(args, [json.loads(l) for l in rsp], spec.get("ignore", []), spec.get("skip_hole", False))
out.write_bytes(pe + b"\\n" + text.encode("latin-1"))
"""


def fake(root: Path, tag: str, **spec) -> tuple[Path, Path]:
    log = root / f"{tag}.log"
    spec["log"] = str(log)
    cc, nm, oc = root / f"fakecc-{tag}", root / f"fakenm-{tag}", root / f"fakeobjcopy-{tag}"
    for path in (cc, nm, oc):
        path.write_text(FAKE.format(python=sys.executable, spec=json.dumps(spec), tools=str(TOOLS)))
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
        "--runtime", str(root / tag / "runtime"), "--jobs", "2", "--sweep-rows", str(root / "no-sweep-rows.toml"), "--programs", str(root / "no-programs.toml"), *map(str, more),
    ]
    return subprocess.run(argv, capture_output=True, text=True, timeout=300)


def read(path: Path) -> str:
    try:
        return path.read_bytes().decode("latin-1")   # the linked file starts with a PE header
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
        "functions overridden in C: 0\n"
        "functions without C: 5, library 2, game and modules 3\n"
        "sweep rows that are not functions: 0\n"
        "names at PS1 addresses: 11\n"
        "data defined in C, at host addresses: 0\n"
        "programs built in: 0\n"
        f"linked: {exe}, verified\n"
    )
    yield "flow-stdout-exact", same((proc.returncode, proc.stdout, proc.stderr), (0, want, ""))
    yield "flow-names-file", same(read(build / "gen" / "names.ld"), "".join(
        f"_ps1_{n} = 0x{a:08x};\n" for n, a in sorted({
            "sym_a": 0x80190000, "sym_b": 0x1F800010, "fa": 0x80100000, "fb": 0x80100010, "fc": 0x80100020, "fd": 0x80100030,
            "mf": 0x801E0000, "mf__mod2": 0x801F0000, "libf": 0x80100100, "asmf": 0x80100200, "callee": 0x80100210}.items())))
    tables = read(build / "gen" / "port_tables.c")
    yield "flow-tables-functions", same([l.strip() for l in tables.splitlines() if l.strip().startswith("{ 0x") and "impl" in l], [
        '{ 0x80100000u, (void *)impl_fa, "fa", -1, 0 },', '{ 0x80100010u, (void *)impl_fb, "fb", -1, 0 },',
        '{ 0x80100020u, (void *)impl_fc, "fc", -1, 0 },', '{ 0x80100030u, (void *)impl_fd, "fd", -1, 0 },',
        '{ 0x801e0000u, (void *)impl_mf, "mf", 0, 0 },', '{ 0x801f0000u, (void *)impl_mf__mod2, "mf__mod2", 1, 0 },'])
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
        ["hole.o", "port_game_text_begin.o", "ua.o", "ub.o", "uc.o", "um.o", "um__mod2.o", "port_game_text_end.o", "main.o", "port_tables.o", "names.ld"],
    )
    calls = [x.split() for x in read(root / "flow.log").splitlines()]
    units = [c for c in calls if "-S" in c and c[-1].endswith((".c",)) and not c[-1].endswith("underscore.c")]
    flags = hb.COMPILE_FLAGS
    yield "flow-compile-flags", same((len(units), all(c[:len(flags)] == flags and c[len(flags):len(flags) + 3] == ["-S", "-I", str(build / "gen")] for c in units)), (4, True))
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "flow-link-flags", same((len(link), link[0][1:8] if link else None), (1, ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase", "-Wl,--image-base=0x10000", "-Wl,--disable-reloc-section", "-Wl,--section-start=.hole=0x11000", "-Wl,--section-start=.text=0x200000"]))
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
         "--objcopy", str(root / "fakeobjcopy-flow"), "--build", "relbuild", "--runtime", "flow/runtime", "--jobs", "2", "--sweep-rows", "none.toml"],
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
    yield "flow-failed-lines", same(out[1:6], [
        "units: 5 compiled, 1 of them nonmatching, 1 failed", "like images built: 1", "functions with C: 6", "functions overridden in C: 0",
        "functions without C: 5, library 2, game and modules 3"])
    yield "flow-failed-names-count", same(out[7], "names at PS1 addresses: 12")
    yield "flow-failed-link-still-made", same(out[10], f"linked: {exe}, verified")
    yield "flow-list-failed", same(out[11], "failed: ub: b.c:3:5: error: boom in ub")
    yield "flow-list-absent-lines", same(out[13:], [
        "absent: fc 0x80100020 -", "absent: asmf 0x80100200 -", "absent: func_801e0010_mod 0x801e0010 mod"])
    absent = out[13:]
    yield "flow-list-absent-count-and-no-library", same((len(absent), any("libf" in x for x in absent)), (3, False))
    yield "flow-failed-table", same(read(root / "flow2" / "build" / "failed.tsv"), "ub\tb.c:3:5: error: boom in ub\n")
    yield "flow-nonmatching-is-function-with-c", same("func_80100040" in read(root / "flow2" / "build" / "gen" / "port_tables.c"), True)
    yield "flow-without-list-nothing-extra", same(len(run(root, "flow2", config, cc, nm).stdout.splitlines()), 11)

    # Data defined in C.
    cc, nm = fake(root, "flow3", defs={**DEFS, "ub": ["fc", "hostvar"]})
    config = run_tree(root, "flow3")
    proc = run(root, "flow3", config, cc, nm, "--list")
    out = proc.stdout.splitlines()
    yield "flow-data-line", same((proc.returncode, out[8], out[-1] if out else None), (0, "data defined in C, at host addresses: 1", out[-1] if out else None))
    yield "flow-data-listed", same("data: hostvar" in out, True)
    yield "flow-data-alias-line", same("_ps1_hostvar = _impl_hostvar;\n" in read(root / "flow3" / "build" / "gen" / "names.ld"), True)
    yield "flow-data-names-count-unchanged", same(out[7], "names at PS1 addresses: 11")

    # A data symbol that symbols.ld places is bound there, not aliased.
    cc, nm = fake(root, "flow3b", defs={**DEFS, "ub": ["fc", "sym_a"]})
    config = run_tree(root, "flow3b")
    proc = run(root, "flow3b", config, cc, nm)
    yield "flow-data-in-symbols-is-not-host", same((proc.returncode, proc.stdout.splitlines()[8]), (0, "data defined in C, at host addresses: 0"))

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
    """--psyz: gpu.c, input.c, iopage.c and press.c alone see PsyZ's headers, the link line gets its libraries, and a bad folder is refused."""
    def tree(tag: str):
        config = run_tree(root, tag)
        write(root / tag / "runtime" / "gpu.c", "int gpu;\n")
        write(root / tag / "runtime" / "input.c", "int input;\n")
        write(root / tag / "runtime" / "iopage.c", "int iopage;\n")
        write(root / tag / "runtime" / "press.c", "int press;\n")
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
    yield "psyz-input-c-gets-the-flags", same(rt["input.c"][4:10], ["-DPORT_HAVE_PSYZ", "-D__psyz", "-DEXTRA=1", "-isystem", inc, "-I"])
    yield "psyz-iopage-c-gets-the-flags", same(rt["iopage.c"][4:10], ["-DPORT_HAVE_PSYZ", "-D__psyz", "-DEXTRA=1", "-isystem", inc, "-I"])
    yield "psyz-press-c-gets-the-flags", same(rt["press.c"][4:10], ["-DPORT_HAVE_PSYZ", "-D__psyz", "-DEXTRA=1", "-isystem", inc, "-I"])
    yield "psyz-other-runtime-files-do-not", same(["PORT_HAVE_PSYZ" in " ".join(rt["main.c"]), inc in rt["main.c"]], [False, False])
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "psyz-link-line-has-the-libraries-after-the-flags", same(link[0][1:] if link else None, [*hb.LINK_FLAGS, str(libs[0]), str(libs[1]), "-lm", "-o", str(root / "ps" / "build" / "sfa2.exe")])

    config, folder, libs = tree("ps2")
    cc, nm = fake(root, "ps2", defs=DEFS)
    proc = run(root, "ps2", config, cc, nm)
    calls = [x.split() for x in read(root / "ps2.log").splitlines()]
    rt = {Path(c[-1]).name: c for c in calls if c[:4] == ["-O1", "-Wall", "-Wextra", "-c"]}
    link = [c for c in calls if c and c[0].startswith("@")]
    yield "psyz-without-the-option-nothing-changes", same((proc.returncode, any(line.startswith("psyz:") for line in proc.stdout.splitlines()), "PORT_HAVE_PSYZ" in " ".join(rt["gpu.c"] + rt["input.c"]), link[0][1:] if link else None),
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


SOUND_UNITS = (
    unit("ua", "a.c", [("fa", 0x80100000), ("fb", 0x80100010)])
    + unit("ub", "b.c", [("fc", 0x80100020)])
    + unit("sn1", "sdk/libsnd/sn1.c", [("snf", 0x80100100)])
    + unit("sn2", "sdk/libsnd/sn2.c", [("snf2", 0x80100110)])
    + unit("sp1", "sdk/libspu/sp1.c", [("spf", 0x80100120)])
    + unit("so", "sdk/other/o.c", [("of", 0x80100130)])
    + unit("sa", "sdk/libsnd/sa.s", [("saf", 0x80100140)], kind="asm")
    + unit("ux", "asm/x.s", [("asmf", 0x80100200), ("callee", 0x80100210)], kind="asm")
)
SOUND_DEFS = {"ua": ["fa", "fb"], "ub": ["fc"], "sn1": ["snf"], "sn2": ["snf2"], "sp1": ["spf"], "func_80100300": ["func_80100300"], "func_80100310": ["func_80100310"]}
SOUND_NONMATCHING = {"library_nonmatching/func_80100300.c": "int y;\n", "n_nonmatching/func_80100310.c": "int z;\n"}


def sound_tree(root: Path, tag: str, units: str = SOUND_UNITS, prefix: dict[str, str] | None = None, shim: bool = True):
    """A made-up game with sound library units, a runtime folder with three files, a PsyZ folder and the shim folder."""
    config = run_tree(root, tag, units=units, nonmatching=SOUND_NONMATCHING)
    base = root / tag
    for rel in ("sdk/libsnd/sn1.c", "sdk/libsnd/sn2.c", "sdk/libspu/sp1.c", "sdk/other/o.c", "sdk/libsnd/sa.s"):
        write(base / "src" / rel, "int x;\n")
    write(base / "runtime" / "gpu.c", "int gpu;\n")
    for rel, text in (prefix or {}).items():
        write(base / "prefix" / rel, text)
    if shim:
        write(base / "sdkshim" / "common.h", "/* made up */\n")
    folder = base / "psyzdir"
    (folder / "inc").mkdir(parents=True)
    lib = write(folder / "libpsyz.a", "")
    write(folder / "psyz.json", json.dumps({"include": str(folder / "inc"), "define": ["__psyz", "EXTRA=1"], "link": [str(lib), "-lm"], "commit": "abc123"}))
    return config, folder


def sound_calls(root: Path, tag: str):
    """The commands the stand-in compiler saw: the unit compiles by source file name, the runtime compiles by source file name."""
    class Seen(dict):
        def __missing__(self, key):   # a command that was not made is an empty one, so that its case fails and the group goes on
            return []

    calls = [x.split() for x in read(root / f"{tag}.log").splitlines()]
    units = Seen({Path(c[-1]).name: c for c in calls if "-S" in c and c[-1].endswith(".c") and not c[-1].endswith("underscore.c")})
    rt = Seen({Path(c[-1]).name: c for c in calls if c[:4] == ["-O1", "-Wall", "-Wextra", "-c"]})
    return units, rt


def sound_cases(root: Path):
    """--sound-library and the prefix folder."""
    flags = hb.COMPILE_FLAGS

    # The selection, called directly: the units of sdk/libsnd and sdk/libspu whose source is .c, and the library folder.
    config, path = config_tree(root, "snd-sel", SOUND_UNITS, "", SOUND_NONMATCHING)
    for rel in ("sdk/libsnd/sn1.c", "sdk/libsnd/sn2.c", "sdk/libspu/sp1.c", "sdk/other/o.c"):
        write(path.parent / rel, "int x;\n")
    sel = hb.select_units(config, path, True)
    yield "sound-select-with-the-option", same(sorted((j.name, j.library) for j in sel.jobs), [
        ("func_80100300", True), ("func_80100310", False), ("sn1", True), ("sn2", True), ("sp1", True), ("ua", False), ("ub", False)])
    yield "sound-select-counts", same(sel.sound_counts, (2, 1, 1))
    sel = hb.select_units(config, path)
    yield "sound-select-without-the-option", same(sorted((j.name, j.library) for j in sel.jobs), [("func_80100310", False), ("ua", False), ("ub", False)])
    yield "sound-select-without-the-option-counts-nothing", same(sel.sound_counts, (0, 0, 0))
    config, path = config_tree(root, "snd-sel2", unit("ua", "a.c", [("fa", 0x80100000)]) + unit("so", "sdk/other/o.c", [("of", 0x80100130)]) + unit("sa", "sdk/libsnd/sa.s", kind="asm"))
    yield "sound-select-no-library-unit-is-refused", raises(lambda: hb.select_units(config, path, True), "--sound-library", "sdk/libsnd")
    yield "sound-select-no-library-unit-is-fine-without-the-option", same([j.name for j in hb.select_units(config, path).jobs], ["ua"])

    # A whole run with the option.
    config, folder = sound_tree(root, "snd")
    cc, nm = fake(root, "snd", defs=SOUND_DEFS)
    proc = run(root, "snd", config, cc, nm, "--sound-library", "--psyz", folder)
    out = proc.stdout.splitlines()
    exe = root / "snd" / "build" / "sfa2.exe"
    yield "sound-run-ends-verified", same((proc.returncode, proc.stderr, out[-1]), (0, "", f"linked: {exe}, verified"))
    yield "sound-line-after-the-psyz-line", same(out[:3], ["compiler: fakecc 1.0", "psyz: abc123", "sound library: 2 units of sdk/libsnd, 1 of sdk/libspu, 1 of library_nonmatching"])
    yield "sound-units-line-counts-library-units-and-files", same(out[3], "units: 7 compiled, 2 of them nonmatching, 0 failed")
    units, rt = sound_calls(root, "snd")
    yield "sound-compiled-sources", same(sorted(units), ["a.c", "b.c", "func_80100300.c", "func_80100310.c", "sn1.c", "sn2.c", "sp1.c"])
    gen = str(root / "snd" / "build" / "gen")
    shim, inc = str(root / "snd" / "sdkshim"), str(folder / "inc")
    asm = lambda n: str(root / "snd" / "build" / "asm" / f"{n}.s")
    src = lambda n, rel: str(root / "snd" / "src" / rel)
    yield "sound-library-unit-gets-the-shim-then-psyz-include", same(units["sn1.c"], [*flags, "-S", "-I", gen, "-I", shim, "-I", inc, "-o", asm("sn1"), src("sn1", "sdk/libsnd/sn1.c")])
    yield "sound-libspu-unit-gets-them-too", same(units["sp1.c"], [*flags, "-S", "-I", gen, "-I", shim, "-I", inc, "-o", asm("sp1"), src("sp1", "sdk/libspu/sp1.c")])
    yield "sound-library-nonmatching-file-gets-them", same(units["func_80100300.c"][:-1], [*flags, "-S", "-I", gen, "-I", shim, "-I", inc, "-o", asm("func_80100300")])
    yield "sound-game-unit-does-not", same(units["a.c"], [*flags, "-S", "-I", gen, "-o", asm("ua"), src("ua", "a.c")])
    yield "sound-other-nonmatching-file-does-not", same(units["func_80100310.c"][:-1], [*flags, "-S", "-I", gen, "-o", asm("func_80100310")])
    yield "sound-no-private-include-path-reaches-the-compiler", same(any("sdk/include" in x for x in read(root / "snd.log").splitlines()), False)
    yield "sound-library-units-have-no-defines", same([a for a in units["sn1.c"] + units["func_80100300.c"] if a.startswith("-D")], [])
    tables = read(root / "snd" / "build" / "gen" / "port_tables.c")
    yield "sound-library-functions-have-c-in-the-tables", same(
        [l.strip() for l in tables.splitlines() if l.strip().startswith("{ 0x801") and ("impl_s" in l or "impl_func_80100300" in l)],
        ['{ 0x80100100u, (void *)impl_snf, "snf", -1, 0 },', '{ 0x80100110u, (void *)impl_snf2, "snf2", -1, 0 },', '{ 0x80100120u, (void *)impl_spf, "spf", -1, 0 },',
         '{ 0x80100300u, (void *)impl_func_80100300, "func_80100300", -1, 0 },'])
    rsp = [Path(json.loads(x)).name for x in read(root / "snd" / "build" / "link.rsp").splitlines()]
    yield "sound-library-objects-are-linked", same([n for n in rsp if n.startswith(("sn", "sp", "func_"))], ["func_80100300.o", "func_80100310.o", "sn1.o", "sn2.o", "sp1.o"])

    # The define reaches the runtime's files with the option, port_tables.c never.
    d = "-DPORT_SOUND_LIBRARY_C"
    rtd = str(root / "snd" / "runtime")
    obj = lambda n: str(root / "snd" / "build" / "rt" / f"{n}.o")
    yield "sound-define-reaches-main-c", same(rt["main.c"], ["-O1", "-Wall", "-Wextra", "-c", d, "-I", rtd, "-o", obj("main"), str(root / "snd" / "runtime" / "main.c")])
    yield "sound-define-follows-psyz-flags-on-gpu-c", same(rt["gpu.c"][:10], ["-O1", "-Wall", "-Wextra", "-c", "-DPORT_HAVE_PSYZ", "-D__psyz", "-DEXTRA=1", "-isystem", inc, d])
    yield "sound-define-not-on-the-tables", same(rt["port_tables.c"], ["-O1", "-Wall", "-Wextra", "-c", "-I", rtd, "-o", obj("port_tables"), str(root / "snd" / "build" / "gen" / "port_tables.c")])

    # The same tree without the option.
    config, folder = sound_tree(root, "snd0")
    cc, nm = fake(root, "snd0", defs=SOUND_DEFS)
    proc = run(root, "snd0", config, cc, nm, "--psyz", folder)
    out = proc.stdout.splitlines()
    units, rt = sound_calls(root, "snd0")
    yield "sound-without-the-option-no-added-line", same((proc.returncode, out[:3]), (0, ["compiler: fakecc 1.0", "psyz: abc123", "units: 3 compiled, 1 of them nonmatching, 0 failed"]))
    yield "sound-without-the-option-sources", same(sorted(units), ["a.c", "b.c", "func_80100310.c"])
    yield "sound-without-the-option-library-folder-left-out", same(("func_80100300" in read(root / "snd0" / "build" / "gen" / "port_tables.c"), "func_80100300.c" in units), (False, False))
    yield "sound-without-the-option-no-define-no-shim", same(("-DPORT_SOUND_LIBRARY_C" in " ".join(sum(rt.values(), [])), "sdkshim" in read(root / "snd0.log")), (False, False))
    yield "sound-without-the-option-main-c-as-before", same(rt["main.c"], ["-O1", "-Wall", "-Wextra", "-c", "-I", str(root / "snd0" / "runtime"), "-o", str(root / "snd0" / "build" / "rt" / "main.o"), str(root / "snd0" / "runtime" / "main.c")])

    # The refusals of the option.
    config, folder = sound_tree(root, "snd1")
    cc, nm = fake(root, "snd1", defs=SOUND_DEFS)
    proc = run(root, "snd1", config, cc, nm, "--sound-library")
    yield "sound-needs-psyz", same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), "--sound-library" in proc.stderr and "--psyz" in proc.stderr, read(root / "snd1.log")), (2, "", 1, True, "(missing)"))
    config, folder = sound_tree(root, "snd2", units=unit("ua", "a.c", [("fa", 0x80100000)]) + unit("so", "sdk/other/o.c", [("of", 0x80100130)]) + unit("ux", "asm/x.s", [("callee", 0x80100210)], kind="asm"))
    cc, nm = fake(root, "snd2", defs=SOUND_DEFS)
    proc = run(root, "snd2", config, cc, nm, "--sound-library", "--psyz", folder)
    yield "sound-no-library-unit-is-refused", same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), "sdk/libsnd" in proc.stderr, read(root / "snd2.log")), (2, "", 1, True, "(missing)"))

    # The prefix folder.
    config, folder = sound_tree(root, "snd3", prefix={"sn1.h": "int p1;\n", "ua.h": "int p2;\n", "so.h": "int p3;\n"})
    cc, nm = fake(root, "snd3", defs=SOUND_DEFS)
    proc = run(root, "snd3", config, cc, nm, "--sound-library", "--psyz", folder)
    units, _ = sound_calls(root, "snd3")
    pre = str(root / "snd3" / "prefix")
    gen = str(root / "snd3" / "build" / "gen")
    yield "prefix-file-reaches-its-library-unit-after-the-includes", same(units["sn1.c"][len(flags):-1], [
        "-S", "-I", gen, "-I", str(root / "snd3" / "sdkshim"), "-I", str(folder / "inc"), "-include", f"{pre}/sn1.h", "-o", str(root / "snd3" / "build" / "asm" / "sn1.s")])
    yield "prefix-file-reaches-a-game-unit-too", same(units["a.c"][len(flags):-1], ["-S", "-I", gen, "-include", f"{pre}/ua.h", "-o", str(root / "snd3" / "build" / "asm" / "ua.s")])
    yield "prefix-file-reaches-no-other-unit", same([sorted(k for k, c in units.items() if "-include" in c)], [["a.c", "sn1.c"]])
    yield "prefix-file-of-an-uncompiled-unit-is-no-error", same((proc.returncode, proc.stderr, "so.h" in read(root / "snd3.log")), (0, "", False))
    config, folder = sound_tree(root, "snd4", prefix={"sn1.h": "int p1;\n"})
    cc, nm = fake(root, "snd4", defs=SOUND_DEFS)
    proc = run(root, "snd4", config, cc, nm, "--psyz", folder)
    yield "prefix-file-of-a-unit-not-compiled-in-this-run-is-not-used", same((proc.returncode, proc.stderr, "-include" in read(root / "snd4.log")), (0, "", False))
    for tag, rel, text, named in (("snd5", "zz.h", "int z;\n", "zz.h"), ("snd6", "sn1.txt", "int z;\n", "sn1.txt"), ("snd7", "sn1.h/inner.h", "int z;\n", "sn1.h")):
        config, folder = sound_tree(root, tag, prefix={rel: text})
        cc, nm = fake(root, tag, defs=SOUND_DEFS)
        proc = run(root, tag, config, cc, nm, "--sound-library", "--psyz", folder)
        yield f"prefix-refusal-{named}", same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), named in proc.stderr, read(root / f"{tag}.log")), (2, "", 1, True, "(missing)"))
    config, folder = sound_tree(root, "snd8", prefix={"zz.h": "int z;\n"})
    cc, nm = fake(root, "snd8", defs=SOUND_DEFS)
    proc = run(root, "snd8", config, cc, nm, "--psyz", folder)
    yield "prefix-refusal-also-without-the-option", same((proc.returncode, proc.stdout, "zz.h" in proc.stderr), (2, "", True))


IMAGES_SYM = IMAGES + "\n[image.symbols]\nsym_own = 0x801f0200\n"
LIKE_SYMBOLS = "sym_out = 0x80190000;\nsym_own = 0x80190010;\nsym_in = 0x801e0100;\n"
LIKE_UNITS = (
    unit("ur", "a.c", [("r1", 0x80100000)])
    + unit("um", "d.c", [("mf", 0x801e0000), ("mg", 0x801e0010)], image="mod")
)
LIKE_CALLS = {"um": ["mg", "r1", "sym_in", "sym_out", "sym_own", "hv"]}


def sweep_flow_cases(root: Path):
    units = (unit("ur", "a.c", [("r1", 0x80100000)]) + unit("um", "d.c", [("mg", 0x801e0010), ("mh", 0x801e0018)], image="mod")
             + unit("uk", "b.c", [("mk", 0x801e0110)], image="mod"))
    entries = (
        '[[row]]\nimage = "mod"\naddress = 0x801e0008\nfunction = "mg"\nfunction_address = 0x801e0010\ndata_bytes = 8\nevidence = "a table"\n'
        '[[row]]\nimage = "mod2"\naddress = 0x801f0008\nfunction = "mg__mod2"\nfunction_address = 0x801f0010\ndata_bytes = 8\nevidence = "a table, second side"\n'
    )
    rows = ("{a}\t0x{s}\t{base}0000\t8\taaaa\n{a}\t0x{s}\t{base}0008\t24\tbbbb\n{a}\t0x{s}\t{base}001c\t4\tcccc\n"
            "{a}\t0x{s}\t{base}0100\t32\tdddd\n{a}\t0x{s}\t{base}0118\t8\teeee\n")
    modules = rows.format(a="A.PAC", s="5", base="801e") + rows.format(a="B.PAC", s="6", base="801f")

    def setup(tag, table, units=units, modules=modules, defs=None, fail=()):
        config = run_tree(root, tag, units=units, symbols=LIKE_SYMBOLS, images=IMAGES_SYM)
        inv = root / tag / "inventory"
        write(inv / "modules.tsv", modules)
        write(inv / "game.tsv", "80100000\t16\tx\n80100040\t16\tx\n")
        write(inv / "library.tsv", "")
        cc, nm = fake(root, tag, defs=defs or {"ur": ["r1"], "um": ["mg", "mh"], "uk": ["mk"]}, call="r1", calls={"um": ["mg"]}, fail=list(fail))
        extra = ["--sweep-rows", write(root / tag / "sweep.toml", table)] if table is not None else []
        return run(root, tag, config, cc, nm, "--list", *extra)

    proc = setup("sweep", entries)
    out = proc.stdout.splitlines()
    absent = [l.split()[1] for l in out if l.startswith("absent:")]
    yield "sweepflow-runs", same((proc.returncode, proc.stderr), (0, ""))
    yield "sweepflow-count-line", same([l for l in out if l.startswith("sweep")], ["sweep rows that are not functions: 4"])
    yield "sweepflow-table-and-tail-rows-lose-their-stop", same(sorted(a for a in absent if a.startswith(("func_801e0008", "func_801f0008", "func_801e001c", "func_801f001c"))), [])
    yield "sweepflow-unlisted-rows-keep-theirs", same(sorted(absent), sorted([
        "func_80100040", "func_801e0000_mod", "func_801e0100_mod", "func_801e0118_mod", "func_801f0000_mod2", "func_801f0100_mod2", "func_801f0118_mod2"]))
    yield "sweepflow-list-lines", same([l for l in out if l.startswith("data-row:")], [
        "data-row: func_801e0008_mod at 0x801e0008, mod, table, function mg at 0x801e0010: a table",
        "data-row: func_801f0008_mod2 at 0x801f0008, mod2, table, function mg__mod2 at 0x801f0010: a table, second side",
        "data-row: func_801e001c_mod at 0x801e001c, mod, rule 2: inside the unit um (0x801e0010-0x801e0020)",
        "data-row: func_801f001c_mod2 at 0x801f001c, mod2, rule 2: inside the unit um (0x801f0010-0x801f0020)"])
    tables = read(root / "sweep" / "build" / "gen" / "port_tables.c")
    yield "sweepflow-no-stop-in-the-listed-rows", same(any(a in tables for a in ("0x801e0008u", "0x801f0008u", "0x801e001cu", "0x801f001cu")), False)
    yield "sweepflow-unknown-prefix-before-a-later-c-function-keeps-its-stop", same(
        ("func_801e0100_mod" in absent, "func_801f0100_mod2" in absent, "mk" in tables and "0x801e0110u" in tables), (True, True, True))
    proc = setup("sweep-none", None)
    out = proc.stdout.splitlines()
    yield "sweepflow-without-a-table-only-rule-2-applies", same(
        (proc.returncode, [l for l in out if l.startswith("sweep")], len([l for l in out if l.startswith("absent:")])), (0, ["sweep rows that are not functions: 2"], 9))
    proc = setup("sweep-bad", entries.replace("data_bytes = 8", "data_bytes = 4", 1))
    yield "sweepflow-a-miss-ends-the-build-with-status-1", same((proc.returncode, "mod 0x801e0008" in proc.stderr, "linked:" in proc.stdout), (1, True, False))
    proc = setup("sweep-bad2", entries.replace("evidence", "proof", 1))
    yield "sweepflow-a-malformed-table-is-status-2", same((proc.returncode, "evidence" in proc.stderr, proc.stdout), (2, True, ""))
    # A unit that failed to compile is not built: its rows keep their stop.
    proc = setup("sweep-failed", None, fail=["um"])
    yield "sweepflow-failed-unit-rows-keep-their-stop", same(
        ("func_801e001c_mod" in [l.split()[1] for l in proc.stdout.splitlines() if l.startswith("absent:")], [l for l in proc.stdout.splitlines() if l.startswith("sweep")]),
        (True, ["sweep rows that are not functions: 0"]))
    # A unit whose functions are not contiguous gets no rule 2.
    gappy = unit("ur", "a.c", [("r1", 0x80100000)]) + unit("um", "d.c", [("mg", 0x801e0010, 8), ("mh", 0x801e0028, 8)], image="mod")
    gap_modules = "A.PAC\t0x5\t801e0000\t8\taaaa\nA.PAC\t0x5\t801e0020\t4\tbbbb\nB.PAC\t0x6\t801f0000\t8\taaaa\nB.PAC\t0x6\t801f0020\t4\tbbbb\n"
    proc = setup("sweep-gap", None, units=gappy, modules=gap_modules, defs={"ur": ["r1"], "um": ["mg", "mh"]})
    out = proc.stdout.splitlines()
    yield "sweepflow-non-contiguous-unit-keeps-the-row-in-its-gap", same(
        (proc.returncode, "func_801e0020_mod" in [l.split()[1] for l in out if l.startswith("absent:")], "func_801f0020_mod2" in [l.split()[1] for l in out if l.startswith("absent:")]), (0, True, True))
    yield "sweepflow-non-contiguous-unit-is-named", same(
        ([l for l in out if l.startswith("not-contiguous:")], "unit um: its functions are not contiguous" in proc.stderr),
        (["not-contiguous: unit um: its functions are not contiguous; rows inside its range keep their stop"], True))


def like_cases(root: Path):
    config = run_tree(root, "like", units=LIKE_UNITS, symbols=LIKE_SYMBOLS, images=IMAGES_SYM)
    cc, nm = fake(root, "like", defs={"ur": ["r1"], "um": ["mf", "mg", "hv"]}, call="r1", calls=LIKE_CALLS)
    proc = run(root, "like", config, cc, nm, "--list")
    build = root / "like" / "build"
    out = proc.stdout.splitlines()
    yield "like-run-verifies", same((proc.returncode, proc.stderr, out[2], out[-1] if False else out[10]), (0, "", "like images built: 1", f"linked: {build / 'sfa2.exe'}, verified"))
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
        '{ 0x801f0000u, (void *)impl_mf__mod2, "mf__mod2", 1, 0 },', '{ 0x801f0010u, (void *)impl_mg__mod2, "mg__mod2", 1, 0 },'])
    yield "like-redefine-file-moves-only-moved", same(
        sorted(l.split()[0] for l in read(build / "gen" / "redefine__mod2.txt").splitlines() if "__mod2" in l),
        ["_hv", "_mf", "_mg", "_sym_in", "_sym_own"])
    yield "like-listing-names-the-image", same([l for l in out if l.startswith("like:")], ["like: mod2 of mod, shift +0x10000, 4 names move, units left out: -"])
    yield "like-response-file-has-both-objects", same(
        [Path(json.loads(x)).name for x in read(build / "link.rsp").splitlines()][:4], ["hole.o", "port_game_text_begin.o", "um.o", "um__mod2.o"])
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


# The overrides: a second C file of the port for a function that a unit has C for.

def override_files(root: Path, tag: str, *names: str, py: bool = True) -> Path:
    """The folder `overrides` beside the tag's runtime folder, with a made-up NAME.c (and NAME.py) for each name."""
    folder = root / tag / "overrides"
    for name in names:
        write(folder / f"{name}.c", "int x;\n")
        if py:
            write(folder / f"{name}.py", "# the contract of the differential test, made up\n")
    return folder


def override_rename_cases():
    both = func("f") + func("g", "\tcall\t_f\n\tret\n")
    text, defined = hb.rename_definitions(both, True, replaced={"_f"})
    yield "override-rename-replaced-definition", same((text, defined), (func("replaced_f") + func("impl_g", "\tcall\t_f\n\tret\n"), {"_f", "_g"}))
    text, _ = hb.rename_definitions(both, True, "__mod2", {"_f"})
    yield "override-rename-replaced-keeps-the-suffix", same(text, func("replaced_f__mod2") + func("impl_g__mod2", "\tcall\t_f\n\tret\n"))
    elf = "\t.globl\tf\n\t.type\tf, @function\nf:\n\tcall\tf\n\tret\n\t.size\tf, .-f\n"
    text, _ = hb.rename_definitions(elf, False, replaced={"f"})
    yield "override-rename-replaced-elf-type-and-size", same(text, "\t.globl\treplaced_f\n\t.type\treplaced_f, @function\nreplaced_f:\n\tcall\tf\n\tret\n\t.size\treplaced_f, .-replaced_f\n")
    text, defined = hb.rename_definitions("\t.globl\t_only\n" + func("g"), True, replaced={"_only", "_nothere"})
    yield "override-rename-a-replaced-name-that-is-not-defined-changes-nothing", same((text, defined), ("\t.globl\t_only\n" + func("impl_g"), {"_g"}))
    yield "override-rename-without-replaced-is-the-old-rename", same(hb.rename_definitions(both, True), hb.rename_definitions(both, True, "", frozenset()))


def override_check_cases():
    yield "override-rows-pointing-at-the-units-c-are-named", same(
        hb.verify_rows([(-1, 0x80100000, "impl_fa", "fa"), (-1, 0x80100010, "replaced_fb", "fb")]),
        ["fb: its row of port_functions points at replaced_fb, the C that an override stands in for"])
    yield "override-rows-all-good", same(hb.verify_rows([(-1, 0x80100000, "impl_fa", "fa")]), [])
    nm = "00401000 T _port_game_text_begin\n00401100 T _port_game_text_end\n00401010 T _impl_fa\n00401020 T _replaced_fa\n"
    yield "override-markers-replaced-between", same(hb.verify_markers(nm, ["fa"], [], True, ["fa"]), [])
    yield "override-markers-replaced-outside", same(
        hb.verify_markers(nm.replace("00401020 T _replaced_fa", "00402000 T _replaced_fa"), ["fa"], [], True, ["fa"]),
        ["replaced_fa: at 0x402000, outside the game's code 0x401000-0x401100"])
    link = GOOD_NM + "00401020 T _replaced_fa\n"
    yield "override-link-replaced-present", same(hb.verify_link(link, dict(NAMES), ["host"], ["fa"], True, ["fa"]), [])
    yield "override-link-replaced-missing", same(hb.verify_link(GOOD_NM, dict(NAMES), ["host"], ["fa"], True, ["fa"]), ["replaced_fa: not in the linked file"])
    yield "override-link-replaced-at-a-ps1-address", same(
        hb.verify_link(GOOD_NM + "80100040 T _replaced_fa\n", dict(NAMES), ["host"], ["fa"], True, ["fa"]), ["replaced_fa: at a PS1 address 0x80100040"])
    text = hb.render_tables([], [(-1, 0x80100000, "impl_fa", "fa"), (-1, 0x80100010, "impl_fb", "fb")], [], SHA, None, {"fb"})
    yield "override-render-flags-exactly-the-overridden-rows", same(
        [l.strip() for l in text.splitlines() if l.strip().startswith("{ 0x8")], ['{ 0x80100000u, (void *)impl_fa, "fa", -1, 0 },', '{ 0x80100010u, (void *)impl_fb, "fb", -1, 1 },'])


UNITS_OV = RUN_UNITS


def override_flow_cases(root: Path):
    def start(tag, over=(), units=RUN_UNITS, py=True, nonmatching=None, **spec):
        config = run_tree(root, tag, units=units, nonmatching=nonmatching)
        override_files(root, tag, *over, py=py)
        cc, nm = fake(root, tag, **spec)
        return config, cc, nm

    def stage(tag, over, *more, **spec):
        config, cc, nm = start(tag, over, **spec)
        return run(root, tag, config, cc, nm, *more), root / tag / "build"

    # An override for fa, which unit ua defines together with fb; ub calls it; its own unit calls it too.
    defs = {**DEFS, "ovr_fa": ["fa"]}
    calls = {"ua": ["fa", "fc"], "ub": ["fa"], "ovr_fa": ["fc"]}
    proc, b = stage("ov1", ["fa"], "--list", defs=defs, calls=calls)
    out = proc.stdout.splitlines()
    exe = b / "sfa2.exe"
    yield "override-run-output-has-the-new-line-with-the-count", same(
        (proc.returncode, proc.stderr, out[3:5]),
        (0, "", ["functions with C: 6", "functions overridden in C: 1"]))
    yield "override-run-lists-the-override-with-its-file-and-unit", same(
        [l for l in out if l.startswith("override:")], [f"override: fa at {root / 'ov1' / 'overrides' / 'fa.c'}, in place of the C of unit ua"])
    ua, ovr = read(b / "obj" / "ua.o"), read(b / "obj" / "ovr_fa.o")
    yield "override-the-units-definition-is-replaced-and-its-others-stay-impl", same(
        sorted(l for l in ua.splitlines() if l.startswith("def ")), ["def _impl_fb", "def _replaced_fa"])
    yield "override-the-override-defines-impl-of-the-name", same(sorted(l for l in ovr.splitlines() if l.startswith("def ")), ["def _impl_fa"])
    yield "override-references-are-untouched-so-every-caller-reaches-the-address", same(
        (sorted(l for l in ua.splitlines() if l.startswith("ref ")), sorted(l for l in read(b / "obj" / "ub.o").splitlines() if l.startswith("ref ")),
         [l for l in ovr.splitlines() if l.startswith("ref ")]),
        (sorted(["ref _ps1_callee", "ref _ps1_callee", "ref _ps1_fa", "ref _ps1_fc"]), ["ref _ps1_callee", "ref _ps1_fa"], ["ref _ps1_callee", "ref _ps1_fc"]))
    asm = read(b / "asm" / "ua.s")
    yield "override-the-units-assembly-says-replaced", same(("_replaced_fa:" in asm, "_impl_fa:" in asm, "_impl_fb:" in asm), (True, False, True))
    tables = read(b / "gen" / "port_tables.c")
    yield "override-the-row-points-at-the-override-and-is-flagged", same(
        [l.strip() for l in tables.splitlines() if l.strip().startswith("{ 0x8010")][:4], [
            '{ 0x80100000u, (void *)impl_fa, "fa", -1, 1 },', '{ 0x80100010u, (void *)impl_fb, "fb", -1, 0 },',
            '{ 0x80100020u, (void *)impl_fc, "fc", -1, 0 },', '{ 0x80100030u, (void *)impl_fd, "fd", -1, 0 },'])
    yield "override-no-row-names-a-replaced-symbol", same("replaced_" in tables, False)
    rsp = [Path(json.loads(x)).name for x in read(b / "link.rsp").splitlines()]
    yield "override-its-object-is-linked-between-the-markers", same(rsp[:7], ["hole.o", "port_game_text_begin.o", "ovr_fa.o", "ua.o", "ub.o", "uc.o", "um.o"])
    yield "override-the-output-without-list-has-no-extra-line", same(
        len(run(root, "ov1", *start("ov1", ["fa"], defs=defs, calls=calls)).stdout.splitlines()), 11)

    # A folder that does not exist, or one that holds nothing that is a C file, is no override.
    proc, b = stage("ov0", [], defs=DEFS)
    folder = root / "ov0" / "overrides"
    write(folder / "README.txt", "not an override\n")
    write(folder / "sub" / "fa.c", "int x;\n")
    config, cc, nm = start("ov0b", [], defs=DEFS)
    write(root / "ov0b" / "overrides" / "README.txt", "x\n")
    write(root / "ov0b" / "overrides" / "sub" / "fa.c", "int x;\n")
    proc2 = run(root, "ov0b", config, cc, nm)
    yield "override-no-folder-is-no-override-and-the-line-says-0", same((proc.returncode, "functions overridden in C: 0" in proc.stdout), (0, True))
    yield "override-a-file-that-is-not-c-and-a-subfolder-are-ignored", same((proc2.returncode, "functions overridden in C: 0" in proc2.stdout, (root / "ov0b" / "build" / "obj" / "ovr_fa.o").exists()), (0, True, False))
    # The option must name a folder: a path that is none is refused, not taken for "no override".
    config, cc, nm = start("ov0c", [], defs=DEFS)
    proc = run(root, "ov0c", config, cc, nm, "--overrides", root / "ov0c" / "no-such-folder")
    yield "override-the-option-naming-no-folder-is-refused", same(
        (proc.returncode, "no-such-folder" in proc.stderr and "--overrides" in proc.stderr, (root / "ov0c" / "build" / "sfa2.exe").exists()), (2, True, False))
    write(root / "ov0c" / "a-file", "x\n")
    proc = run(root, "ov0c", config, cc, nm, "--overrides", root / "ov0c" / "a-file")
    yield "override-the-option-naming-a-file-is-refused", same((proc.returncode, "a-file" in proc.stderr), (2, True))
    # The option names another folder.
    config, cc, nm = start("ov2", [], defs={**DEFS, "ovr_fb": ["fb"]})
    elsewhere = override_files(root, "elsewhere", "fb")
    proc = run(root, "ov2", config, cc, nm, "--overrides", elsewhere)
    yield "override-the-option-names-the-folder", same((proc.returncode, "functions overridden in C: 1" in proc.stdout, "def _replaced_fb" in read(root / "ov2" / "build" / "obj" / "ua.o")), (0, True, True))

    # Second placements.
    like_defs = {"ur": ["r1"], "um": ["mf", "mg", "hv"], "ovr_mf": ["mf"]}
    like_calls = {**LIKE_CALLS, "ovr_mf": ["mg", "sym_in"]}

    def like_run(tag, images, defs=like_defs, calls=like_calls):
        config = run_tree(root, tag, units=LIKE_UNITS, symbols=LIKE_SYMBOLS, images=images)
        override_files(root, tag, "mf")
        cc, nm = fake(root, tag, defs=defs, call="r1", calls=calls)
        return run(root, tag, config, cc, nm, "--list"), root / tag / "build"

    proc, b = like_run("ovlike", IMAGES_SYM)
    out = proc.stdout.splitlines()
    first, second = read(b / "obj" / "ovr_mf.o"), read(b / "obj" / "ovr_mf__mod2.o")
    yield "override-second-placement-runs", same((proc.returncode, proc.stderr), (0, ""))
    yield "override-second-placement-definitions", same(
        (sorted(l for l in first.splitlines() if l.startswith("def ")), sorted(l for l in second.splitlines() if l.startswith("def "))), (["def _impl_mf"], ["def _impl_mf__mod2"]))
    yield "override-second-placement-references-follow-the-redefine-file", same(
        (sorted(l for l in first.splitlines() if l.startswith("ref ")), sorted(l for l in second.splitlines() if l.startswith("ref "))),
        (sorted(["ref _ps1_mg", "ref _ps1_sym_in", "ref _ps1_r1"] if False else ["ref _ps1_mg", "ref _ps1_sym_in", "ref _ps1_r1"]),
         sorted(["ref _ps1_mg__mod2", "ref _ps1_sym_in__mod2", "ref _ps1_r1"])))
    yield "override-second-placement-of-the-units-definition-is-replaced", same(
        (sorted(l for l in read(b / "obj" / "um.o").splitlines() if l.startswith("def ")), sorted(l for l in read(b / "obj" / "um__mod2.o").splitlines() if l.startswith("def "))),
        (["def _impl_hv", "def _impl_mg", "def _replaced_mf"], ["def _impl_hv__mod2", "def _impl_mg__mod2", "def _replaced_mf__mod2"]))
    tables = read(b / "gen" / "port_tables.c")
    yield "override-second-placement-rows-flagged", same(
        [l.strip() for l in tables.splitlines() if "mf" in l and l.strip().startswith("{ 0x")], [
            '{ 0x801e0000u, (void *)impl_mf, "mf", 0, 1 },', '{ 0x801f0000u, (void *)impl_mf__mod2, "mf__mod2", 1, 1 },'])
    yield "override-second-placements-count-in-the-line", same([l for l in out if l.startswith("functions overridden")], ["functions overridden in C: 2"])
    yield "override-second-placement-objects-are-linked", same(
        [Path(json.loads(x)).name for x in read(b / "link.rsp").splitlines()][:6], ["hole.o", "port_game_text_begin.o", "ovr_mf.o", "ovr_mf__mod2.o", "um.o", "um__mod2.o"])
    yield "override-second-placement-compiles-the-override-once", same(
        len([x for x in read(root / "ovlike.log").splitlines() if " -S " in f" {x} " and x.endswith("mf.c")]), 1)
    # The unit is left out of the second image: no second object, the name is absent there.
    left = IMAGES_SYM.replace('archive = "../x/B.PAC"', 'archive = "../x/B.PAC"\nleave_out = ["um"]')
    proc, b = like_run("ovlike2", left, {"ur": ["r1"], "um": ["mf", "mg"], "ovr_mf": ["mf"]},
                      {"um": ["mg", "r1", "sym_in", "sym_out", "sym_own"], "ovr_mf": ["mg", "sym_in"]})
    out = proc.stdout.splitlines()
    yield "override-left-out-unit-has-no-second-object", same(
        (proc.returncode, (b / "obj" / "ovr_mf__mod2.o").exists(), (b / "obj" / "um__mod2.o").exists(), [l for l in out if l.startswith("functions overridden")]),
        (0, False, False, ["functions overridden in C: 1"]))
    yield "override-left-out-unit-name-absent-there", same(
        ("absent: mf__mod2 0x801f0000 mod2" in out, "impl_mf__mod2" in read(b / "gen" / "port_tables.c")), (True, False))

    # Refusals before any compilation: status 2, one line, the file and the reason.
    def refused(tag, over, needles, py=True, units=RUN_UNITS, nonmatching=None):
        config, cc, nm = start(tag, over, units=units, py=py, nonmatching=nonmatching, defs=DEFS)
        proc = run(root, tag, config, cc, nm)
        file = root / tag / "overrides" / f"{over[0]}.c"
        return same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), all(n in proc.stderr for n in (str(file), *needles)), (root / f"{tag}.log").exists()),
                    (2, "", 1, True, False))

    yield "override-refused-unknown-name", refused("ovr1", ["nothere"], ["nothere", "not a function that a unit of the configuration declares"])
    yield "override-refused-nonmatching-function", refused(
        "ovr2", ["func_80100040"], ["func_80100040", "nonmatching function"], nonmatching={"n_nonmatching/func_80100040.c": "int z;\n"})
    yield "override-refused-without-its-contract-file", refused("ovr3", ["fa"], ["fa", "no fa.py", "differential test"], py=False)
    yield "override-refused-a-unit-hostcheck-does-not-select", refused("ovr4", ["asmf"], ["asmf", "unit ux", "does not select"])
    yield "override-refused-a-library-unit-hostcheck-does-not-select", refused("ovr5", ["libf"], ["libf", "unit us", "does not select"])
    config, cc, nm = start("ovr6", ["fa", "nothere"], defs=DEFS)
    proc = run(root, "ovr6", config, cc, nm)
    yield "override-refused-names-the-file-that-is-wrong", same((proc.returncode, "nothere.c" in proc.stderr, "fa.c" in proc.stderr), (2, True, False))
    # The nonmatching message differs from the unknown one, and the contract check does not hide the name checks.
    config, cc, nm = start("ovr7", ["func_80100040"], py=False, nonmatching={"n_nonmatching/func_80100040.c": "int z;\n"}, defs=DEFS)
    proc = run(root, "ovr7", config, cc, nm)
    yield "override-refused-the-name-check-comes-before-the-contract-check", same((proc.returncode, "nonmatching" in proc.stderr, "differential" in proc.stderr), (2, True, False))

    # Build stops, status 1, no fall back to the unit's C.
    def stops(tag, spec, over=("fa",), needles=(), no_units_line=True, **more):
        config, cc, nm = start(tag, list(over), defs=spec.pop("defs", defs), **spec)
        proc = run(root, tag, config, cc, nm)
        return same((proc.returncode, all(n in proc.stderr for n in needles), "linked:" in proc.stdout, "units:" in proc.stdout), (1, True, False, not no_units_line))

    yield "override-that-does-not-compile-stops-the-build-with-its-first-error", stops("ovb1", dict(fail=["ovr_fa"]), needles=["fa.c", "b.c:3:5: error: boom in ovr_fa".replace("b.c", "fa.c")])
    yield "override-that-defines-another-global-symbol-is-an-error-naming-it", stops(
        "ovb2", dict(defs={**DEFS, "ovr_fa": ["fa", "extra_global"]}), needles=["fa.c", "extra_global"])
    yield "override-that-does-not-define-its-name-is-an-error", stops("ovb3", dict(defs={**DEFS, "ovr_fa": ["other"]}), needles=["fa.c", "other"])
    yield "override-that-defines_nothing-is-an-error", stops("ovb4", dict(defs={**DEFS, "ovr_fa": []}), needles=["fa.c", "does not define fa"])
    yield "override-whose-unit-does-not-define-the-name-is-an-error", stops(
        "ovb5", dict(defs={"ua": ["fb"], "ub": ["fc"], "uc": ["fd"], "um": ["mf"], "ovr_fa": ["fa"]}), needles=["fa.c", "unit ua", "fa"], no_units_line=False)
    yield "override-whose-unit-does-not-build-is-an-error", stops("ovb6", dict(fail=["ua"]), needles=["fa.c", "unit ua"], no_units_line=False)
    # A failing override compiled alone is not a failed unit: nothing in failed.tsv and the build did not go on to link.
    config, cc, nm = start("ovb7", ["fa"], defs=defs, fail=["ovr_fa"])
    proc = run(root, "ovb7", config, cc, nm)
    yield "override-that-fails-is-not-a-failed-unit", same((proc.returncode, proc.stdout, (root / "ovb7" / "build" / "failed.tsv").exists(), (root / "ovb7" / "build" / "sfa2.exe").exists()), (1, "compiler: fakecc 1.0\n", False, False))
    # The link check covers the new symbol: a linker whose output lacks replaced_fa is caught.
    cc, nm = fake(root, "ovb8", defs=defs, nm_extra=["80100040 T _replaced_fa"])
    config = run_tree(root, "ovb8")
    override_files(root, "ovb8", "fa")
    proc = run(root, "ovb8", config, cc, nm)
    yield "override-replaced-symbol-at-a-ps1-address-fails-the-link-check", same((proc.returncode, "replaced_fa: at a PS1 address 0x80100040" in proc.stderr, "linked:" in proc.stdout), (1, True, False))


# The programs that the port builds in.

PG_SHA = bytes(range(32, 64)).hex()


def declaration(*programs: tuple[str, str, list[tuple[str, int]], dict[str, int]]) -> str:
    """A made-up programs.toml: (name, disc path, functions with addresses, symbols)."""
    text = ""
    for name, disc, functions, symbols in programs:
        text += (f'\n[[program]]\nname = "{name}"\nfile = "../../extract/{name}.exe"\ndisc = "{disc}"\nsha256 = "{PG_SHA}"\naddress = 0x80010000\nfunctions = [\n'
                 + "".join(f'  {{ name = "func_{a:08x}_{name}", address = {a:#x}, size = 16 }},\n' for _, a in functions) + "]\n"
                 + "[program.symbols]\n" + "".join(f"{k} = {v:#x}\n" for k, v in symbols.items()))
    return text


PG_SYMBOLS = {"data_80020000_pg": 0x80020000, "sym_a": 0x80012345, "LibRoutine": 0x80011000}
PG_FUNCTIONS = [("main", 0x80010000), ("helper", 0x80010010)]
PG_LIST = """[[program]]
name = "pg"
main = "func_80010000_pg"
setup = [ { call = "port_a", symbol = "data_80020000_pg" }, { call = "port_b" } ]
teardown = [ { call = "port_c" } ]

[program.library]
LibRoutine = "port_lib_routine"
"""
PG_FILES = {"pg/func_80010000_pg.c": "int x;\n", "pg/func_80010010_pg.c": "int x;\n"}


def program_tree(root: Path, tag: str, list_text: str = PG_LIST, decl: str | None = None, files: dict[str, str] | None = None, units: str = RUN_UNITS) -> Path:
    """A made-up game with a made-up declaration, program folder and port list; the list is root/TAG/programs-list.toml."""
    config = run_tree(root, tag, units=units)
    base = root / tag / "src" / "programs"
    write(base / "programs.toml", decl if decl is not None else declaration(("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS)))
    for rel, text in (files or PG_FILES).items():
        write(base / rel, text)
    write(root / tag / "programs-list.toml", list_text)
    return config


PG_DEFS = {**DEFS, "func_80010000_pg": ["func_80010000_pg"], "func_80010010_pg": ["func_80010010_pg"]}
PG_CALLS = {"func_80010000_pg": ["func_80010010_pg", "data_80020000_pg", "sym_a", "LibRoutine"]}
PG_RT = {"main": ["_port_lib_routine"]}


def program_run(root: Path, tag: str, list_text: str = PG_LIST, decl: str | None = None, files: dict[str, str] | None = None, defs=None, calls=None, units: str = RUN_UNITS, **spec):
    config = program_tree(root, tag, list_text, decl, files, units)
    cc, nm = fake(root, tag, defs=defs or PG_DEFS, calls=calls or PG_CALLS, rt_only=spec.pop("rt_only", PG_RT), **spec)
    return run(root, tag, config, cc, nm, "--programs", root / tag / "programs-list.toml"), root / tag / "build"


def program_read_cases(root: Path):
    """The port's list: every refusal names the file or the program."""
    decl = write(root / "rd" / "programs.toml", declaration(("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS), ("qq", "PAC/QQ.EXE", [("main", 0x80010020)], {"data_80020010_qq": 0x80020010})))

    def read_list(text: str, name: str = "list.toml"):
        return hb.read_programs_list(write(root / "rd" / name, text), decl)

    good = read_list(PG_LIST)
    yield "programs-list-read", same(
        [(x.program.name, x.main, [(c.routine, c.symbol) for c in x.setup], [(c.routine, c.symbol) for c in x.teardown], x.library) for x in good],
        [("pg", "func_80010000_pg", [("port_a", "data_80020000_pg"), ("port_b", None)], [("port_c", None)], {"LibRoutine": "port_lib_routine"})])
    yield "programs-list-without-setup-or-library", same(
        [(x.setup, x.teardown, x.library) for x in read_list('[[program]]\nname = "qq"\nmain = "func_80010020_qq"\n')], [([], [], {})])
    yield "programs-list-missing-file-is-no-program", same(hb.read_programs_list(root / "rd" / "nowhere.toml", decl), [])
    yield "programs-list-without-programs-does-not-read-the-declaration", same(hb.read_programs_list(write(root / "rd" / "none.toml", "# none\n"), root / "rd" / "nodecl.toml"), [])
    two = PG_LIST + '\n[[program]]\nname = "qq"\nmain = "func_80010020_qq"\n'
    yield "programs-list-two-programs", same([x.program.name for x in read_list(two)], ["pg", "qq"])
    yield "programs-list-unknown-top-level-key", raises(lambda: read_list("extra = 1\n" + PG_LIST), "list.toml", "extra")
    yield "programs-list-unknown-program-key", raises(lambda: read_list(PG_LIST.replace('name = "pg"', 'name = "pg"\nextra = 1')), "pg", "extra")
    yield "programs-list-unknown-call-key", raises(lambda: read_list(PG_LIST.replace('{ call = "port_b" }', '{ call = "port_b", arg = 1 }')), "setup", "arg")
    yield "programs-list-unknown-program", raises(lambda: read_list(PG_LIST.replace('"pg"', '"zz"', 1)), "zz", "declaration")
    yield "programs-list-name-twice", raises(lambda: read_list(PG_LIST + PG_LIST), "pg", "twice")
    yield "programs-list-name-missing", raises(lambda: read_list('[[program]]\nmain = "x"\n'), "name")
    yield "programs-list-main-missing", raises(lambda: read_list('[[program]]\nname = "pg"\n'), "pg", "main")
    yield "programs-list-main-not-a-function-of-the-program", raises(lambda: read_list(PG_LIST.replace("func_80010000_pg", "func_80010020_qq")), "pg", "main", "func_80010020_qq")
    yield "programs-list-main-by-a-short-name", raises(lambda: read_list(PG_LIST.replace("func_80010000_pg", "main")), "pg", "main")
    yield "programs-list-call-symbol-not-a-symbol", raises(lambda: read_list(PG_LIST.replace('symbol = "data_80020000_pg"', 'symbol = "nothere"')), "setup", "nothere")
    yield "programs-list-call-without-routine", raises(lambda: read_list(PG_LIST.replace('{ call = "port_b" }', '{ symbol = "sym_a" }')), "setup", "call")
    yield "programs-list-call-not-a-name", raises(lambda: read_list(PG_LIST.replace('"port_c"', '"port c"')), "teardown", "call")
    yield "programs-list-setup-not-an-array", raises(lambda: read_list(PG_LIST.replace("setup = [", "setup = 3 #[")), "setup")
    yield "programs-list-library-name-not-a-symbol", raises(lambda: read_list(PG_LIST.replace("LibRoutine =", "Other =")), "pg", "Other")
    yield "programs-list-library-routine-not-a-name", raises(lambda: read_list(PG_LIST.replace('"port_lib_routine"', "3")), "pg", "LibRoutine")
    yield "programs-list-not-toml", raises(lambda: read_list("[[program\n"), "list.toml")
    yield "programs-list-declaration-with-a-fault", raises(lambda: hb.read_programs_list(write(root / "rd" / "l2.toml", PG_LIST), write(root / "rd" / "bad.toml", "[[program]]\nname = 3\n")), "bad.toml")
    yield "programs-list-declaration-missing", raises(lambda: hb.read_programs_list(write(root / "rd" / "l3.toml", PG_LIST), root / "rd" / "absent.toml"), "absent.toml")


def program_plan_cases(root: Path):
    decl = write(root / "pl" / "programs.toml", declaration(
        ("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS),
        ("qq", "PAC/QQ.EXE", [("main", 0x80010020)], {"data_80020000_pg": 0x80020000, "LibRoutine": 0x80011004, "sym_a": 0x80099999})))
    one = hb.read_programs_list(write(root / "pl" / "one.toml", PG_LIST), decl)
    game = {"sym_a": 0x80190000, "other": 1}
    names = hb.plan_program_names(one, game)
    yield "programs-names-the-four-kinds", same((names.functions, names.data, names.bound),
                                              (["func_80010000_pg", "func_80010010_pg"], {"data_80020000_pg": 0x80020000}, {"LibRoutine": "port_lib_routine"}))
    yield "programs-names-a-game-name-is-skipped-whatever-its-address", same("sym_a" in names.all(), False)
    yield "programs-names-a-symbol-nothing-binds-is-refused", raises(lambda: hb.plan_program_names(
        hb.read_programs_list(write(root / "pl" / "u.toml", PG_LIST.replace('LibRoutine = "port_lib_routine"', "")), decl), game), "pg", "LibRoutine", "library")
    yield "programs-names-a-binding-for-a-game-name-is-refused", raises(lambda: hb.plan_program_names(
        hb.read_programs_list(write(root / "pl" / "g.toml", PG_LIST + 'sym_a = "port_sym"\n'), decl), game), "sym_a", "game")
    yield "programs-names-a-binding-for-a-data-name-is-refused", raises(lambda: hb.plan_program_names(
        hb.read_programs_list(write(root / "pl" / "d.toml", PG_LIST + 'data_80020000_pg = "port_d"\n'), decl), game), "data_80020000_pg")
    yield "programs-names-a-function-with-the-name-of-a-game-name-is-refused", raises(lambda: hb.plan_program_names(one, {**game, "func_80010010_pg": 5}), "func_80010010_pg")
    second = '\n[[program]]\nname = "qq"\nmain = "func_80010020_qq"\n[program.library]\nLibRoutine = "@@"\n'
    same_target = hb.plan_program_names(hb.read_programs_list(write(root / "pl" / "s.toml", PG_LIST + second.replace("@@", "port_lib_routine")), decl), game)
    yield "programs-names-a-name-in-two-programs-with-the-same-target-is-fine", same((same_target.bound, same_target.data), ({"LibRoutine": "port_lib_routine"}, {"data_80020000_pg": 0x80020000}))
    yield "programs-names-a-name-in-two-programs-leading-to-two-things-is-refused", raises(lambda: hb.plan_program_names(
        hb.read_programs_list(write(root / "pl" / "t.toml", PG_LIST + second.replace("@@", "port_other")), decl), game), "LibRoutine", "port_lib_routine", "port_other", "pg", "qq")
    clash = declaration(("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS), ("qq", "PAC/QQ.EXE", [("main", 0x80010020)], {"data_80020000_pg": 0x80020004, "LibRoutine": 0x80011004}))
    yield "programs-names-a-data-name-at-two-addresses-is-refused", raises(lambda: hb.plan_program_names(
        hb.read_programs_list(write(root / "pl" / "v.toml", PG_LIST + second.replace("@@", "port_lib_routine")), write(root / "pl" / "clash.toml", clash)), game),
        "data_80020000_pg", str(0x80020000), str(0x80020004))
    yield "programs-names-a-data-name-the-game-also-has-is-its-data-name", same(hb.plan_program_names(one, {"data_80020000_pg": 0x80020000, "sym_a": 1}).data, {"data_80020000_pg": 0x80020000})
    yield "programs-names-none-listed", same((hb.plan_program_names([], game).functions, hb.plan_program_names([], game).all()), ([], set()))


def program_unit_cases(root: Path):
    decl = write(root / "pu" / "programs" / "programs.toml", declaration(("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS)))
    listed = hb.read_programs_list(write(root / "pu" / "l.toml", PG_LIST), decl)[0]
    folder = root / "pu" / "programs"
    for stem in ("func_80010000_pg", "func_80010010_pg"):
        write(folder / "pg" / f"{stem}.c", "int x;\n")
    write(folder / "pg" / "pg.h", "/* not a unit */\n")
    jobs = hb.program_jobs(folder, listed)
    yield "programs-units-are-the-func-files", same([(j.name, j.source.name, j.functions[0].address, j.image) for j in jobs],
                                                  [("func_80010000_pg", "func_80010000_pg.c", 0x80010000, None), ("func_80010010_pg", "func_80010010_pg.c", 0x80010010, None)])
    write(folder / "pg" / "func_80010020_pg.c", "int x;\n")
    yield "programs-units-a-stem-not-in-the-declaration", raises(lambda: hb.program_jobs(folder, listed), "program pg", "func_80010020_pg.c")
    (folder / "pg" / "func_80010020_pg.c").unlink()
    (folder / "pg" / "func_80010010_pg.c").unlink()
    yield "programs-units-a-declared-function-without-a-file", raises(lambda: hb.program_jobs(folder, listed), "program pg", "func_80010010_pg", "no file")
    write(folder / "pg" / "func_80010030_pg.c", "int x;\n")
    yield "programs-units-both-ways-are-named", raises(lambda: hb.program_jobs(folder, listed), "func_80010030_pg.c", "func_80010010_pg")
    yield "programs-units-no-folder", raises(lambda: hb.program_jobs(root / "pu" / "nowhere", listed), "func_80010000_pg", "no file")


def program_option_cases():
    yield "programs-option-default-is-the-ports-list", same(
        (hb.make_parser().parse_args([]).programs, hb.DEFAULT_PROGRAMS, hb.DEFAULT_PROGRAMS.is_file(), hb.DEFAULT_PROGRAMS.parent.name, hb.DEFAULT_PROGRAMS.name),
        (hb.DEFAULT_PROGRAMS, hb.REPO / "port" / "programs.toml", True, "port", "programs.toml"))
    yield "programs-option-the-ports-list-reads-against-the-declaration", same(
        isinstance(hb.read_programs_list(hb.DEFAULT_PROGRAMS, hb.REPO / "ps1" / "src" / "programs" / "programs.toml"), list), True)
    yield "programs-option-given", same(hb.make_parser().parse_args(["--programs", "x.toml"]).programs, Path("x.toml"))


def program_pure_cases():
    yield "programs-undefined-names-only-ps1", same(hb.undefined_names("         U _ps1_a\n00000000 T _impl_f\n         U _ps1_b\n", True), [])
    yield "programs-undefined-names-a-plain-name", same(hb.undefined_names("         U _ps1_a\n         U _DecDCTout\n         U _memcpy\n", True), ["DecDCTout", "memcpy"])
    yield "programs-undefined-names-no-underscore", same(hb.undefined_names("         U ps1_a\n         U memcpy\n", False), ["memcpy"])
    yield "programs-undefined-names-a-defined-plain-name-is-no-undefined-name", same(hb.undefined_names("00000000 T _f\n00000000 D _v\n", True), [])
    yield "programs-undefined-names-ps1-inside-a-name-is-plain", same(hb.undefined_names("         U _xps1_a\n         U ps1_b\n", True), ["ps1_b", "xps1_a"])
    yield "programs-names-file-binds-a-routine", same(hb.render_names({"a": 1}, ["f"], True, {"Lib": "port_x"}), "_ps1_a = 0x00000001;\n_ps1_f = _impl_f;\n_ps1_Lib = _port_x;\n")
    yield "programs-names-file-binds-routines-in-order-of-name", same(
        hb.render_names({}, [], False, {"Mid": "port_m", "Abc": "port_y", "Zed": "port_z"}), "ps1_Abc = port_y;\nps1_Mid = port_m;\nps1_Zed = port_z;\n")
    yield "programs-names-file-without-bound-is-as-before", same(hb.render_names({"a": 1}, [], True, None), hb.render_names({"a": 1}, [], True))
    nm = GOOD_NM + "00403100 T _port_x\n00403100 A _ps1_Lib\n"
    yield "programs-link-bound-name-at-the-routine", same(hb.verify_link(nm, dict(NAMES), ["host"], ["fa"], True, [], {"Lib": "port_x"}), [])
    yield "programs-link-bound-name-elsewhere", same(hb.verify_link(nm.replace("00403100 A _ps1_Lib", "00403104 A _ps1_Lib"), dict(NAMES), ["host"], ["fa"], True, [], {"Lib": "port_x"}),
                                                    ["Lib: ps1_Lib is at 0x403104, the routine port_x at 0x403100"])
    yield "programs-link-bound-name-listed-twice-one-wrong", same(
        hb.verify_link(nm + "00403104 A _ps1_Lib\n", dict(NAMES), ["host"], ["fa"], True, [], {"Lib": "port_x"}), ["Lib: ps1_Lib is at 0x403100, 0x403104, the routine port_x at 0x403100"])
    yield "programs-link-bound-name-missing", same(hb.verify_link(GOOD_NM + "00403100 T _port_x\n", dict(NAMES), ["host"], ["fa"], True, [], {"Lib": "port_x"}), ["Lib: ps1_Lib is not in the linked file"])
    yield "programs-link-bound-routine-missing", same(hb.verify_link(GOOD_NM + "00403100 A _ps1_Lib\n", dict(NAMES), ["host"], ["fa"], True, [], {"Lib": "port_x"}), ["Lib: port_x is not in the linked file"])
    rows = [hb.ProgramRow("pg", "pg.exe", PG_SHA, "func_80010000_pg", [hb.Call("port_a", "data_80020000_pg"), hb.Call("port_b")], [hb.Call("port_c")]),
            hb.ProgramRow("qq", "qq.exe", PG_SHA, "func_80010020_qq"),
            hb.ProgramRow("rr", "rr.exe", PG_SHA, "func_80010030_rr", [hb.Call("port_d")]),
            hb.ProgramRow("ss", "ss.exe", PG_SHA, "func_80010040_ss", [], [hb.Call("port_e")])]
    text = hb.render_tables([], [], [], SHA, None, frozenset(), rows)
    sha = ", ".join("0x%02x" % b for b in range(32, 64))
    middle = text.split("const struct port_absent port_absents[]")[0].split('#include "port_tables.h"\n\n')[1]
    yield "programs-table-externs-and-routines", same(middle, f"""
extern void impl_func_80010000_pg(void);
extern void impl_func_80010020_qq(void);
extern void impl_func_80010030_rr(void);
extern void impl_func_80010040_ss(void);

static const unsigned char port_programs_sha256_0[32] = {{ {sha} }};
static const unsigned char port_programs_sha256_1[32] = {{ {sha} }};
static const unsigned char port_programs_sha256_2[32] = {{ {sha} }};
static const unsigned char port_programs_sha256_3[32] = {{ {sha} }};

extern void port_a(unsigned);
extern void port_b(unsigned);
extern void port_c(unsigned);
extern void port_d(unsigned);
extern void port_e(unsigned);
extern unsigned char ps1_data_80020000_pg[];

static void port_program_setup_0(void)
{{
    port_a((unsigned)(unsigned long)ps1_data_80020000_pg);
    port_b(0);
}}

static void port_program_teardown_0(void)
{{
    port_c(0);
}}

static void port_program_setup_2(void)
{{
    port_d(0);
}}

static void port_program_teardown_3(void)
{{
    port_e(0);
}}

const struct port_image port_images[] = {{
    {{ 0, 0, 0, 0, 0, 0 }},
}};
const unsigned port_image_count = 0;

const struct port_function port_functions[] = {{
    {{ 0, 0, 0, 0, 0 }},
}};
const unsigned port_function_count = 0;

""")
    yield "programs-table-rows", same(text.split("const struct port_builtin_program port_programs[] = {\n")[1].split("const unsigned port_program_count")[0], """    { "pg", "pg.exe", port_programs_sha256_0, (void *)impl_func_80010000_pg, port_program_setup_0, port_program_teardown_0 },
    { "qq", "qq.exe", port_programs_sha256_1, (void *)impl_func_80010020_qq, 0, 0 },
    { "rr", "rr.exe", port_programs_sha256_2, (void *)impl_func_80010030_rr, port_program_setup_2, 0 },
    { "ss", "ss.exe", port_programs_sha256_3, (void *)impl_func_80010040_ss, 0, port_program_teardown_3 },
};
""")
    yield "programs-table-count", same("const unsigned port_program_count = 4;\n" in text, True)
    empty = hb.render_tables([], [], [], SHA, None, frozenset(), [])
    yield "programs-table-empty-has-count-zero", same(("const unsigned port_program_count = 0;" in empty, "{ 0, 0, 0, 0, 0, 0 },\n};\nconst unsigned port_program_count" in empty, "port_programs_sha256" in empty), (True, True, False))
    yield "programs-table-absent-without-programs-argument", same("port_programs" in hb.render_tables([], [], [], SHA), False)
    program = type("P", (), {"name": "pg", "disc": "PAC/SUB/LOGO.EXE", "sha256": PG_SHA})()
    yield "programs-table-row-from-listing", same(hb.program_row(hb.Listed(program, "m")), hb.ProgramRow("pg", "logo.exe", PG_SHA, "m"))
    one = hb.ProgramRow("a", "a.exe", PG_SHA, "m", [hb.Call("r"), hb.Call("r")], [hb.Call("r")])
    yield "programs-table-a-routine-is-declared-once", same(hb.render_tables([], [], [], SHA, None, frozenset(), [one]).count("extern void r(unsigned);"), 1)


def program_flow_cases(root: Path):
    proc, build = program_run(root, "pg1")
    exe = build / "sfa2.exe"
    want = (
        "compiler: fakecc 1.0\n"
        "units: 4 compiled, 0 of them nonmatching, 0 failed\n"
        "like images built: 1\n"
        "functions with C: 6\n"
        "functions overridden in C: 0\n"
        "functions without C: 5, library 2, game and modules 3\n"
        "sweep rows that are not functions: 0\n"
        "names at PS1 addresses: 11\n"
        "data defined in C, at host addresses: 0\n"
        "programs built in: 1\n"
        "program pg: 2 functions\n"
        f"linked: {exe}, verified\n"
    )
    yield "programs-flow-stdout-exact", same((proc.returncode, proc.stdout, proc.stderr), (0, want, ""))
    names = read(build / "gen" / "names.ld").splitlines()
    yield "programs-flow-names-file-holds-the-four-kinds", same([l for l in names if any(k in l for k in ("_pg", "LibRoutine"))], [
        "_ps1_data_80020000_pg = 0x80020000;", "_ps1_func_80010000_pg = _impl_func_80010000_pg;", "_ps1_func_80010010_pg = _impl_func_80010010_pg;", "_ps1_LibRoutine = _port_lib_routine;"])
    yield "programs-flow-a-name-the-game-has-keeps-the-games-address", same(
        ([l for l in names if l.startswith("_ps1_sym_a ")], "0x80012345" in read(build / "gen" / "names.ld")), (["_ps1_sym_a = 0x80190000;"], False))
    yield "programs-flow-functions-have-no-address", same([l for l in names if "func_8001" in l and "0x" in l], [])
    tables = read(build / "gen" / "port_tables.c")
    yield "programs-flow-table-holds-the-program", same(
        [l.strip() for l in tables.split("const struct port_builtin_program port_programs[] = {")[1].splitlines()[1:3]],
        ['{ "pg", "pg.exe", port_programs_sha256_0, (void *)impl_func_80010000_pg, port_program_setup_0, port_program_teardown_0 },', "};"])
    yield "programs-flow-table-carries-the-hash-bytes", same(
        ("static const unsigned char port_programs_sha256_0[32] = { " + ", ".join("0x%02x" % b for b in range(32, 64)) + " };") in tables, True)
    yield "programs-flow-setup-and-teardown-call-in-order", same(
        tables.split("static void port_program_setup_0(void)")[1].split("}")[0].split() + tables.split("static void port_program_teardown_0(void)")[1].split("}")[0].split(),
        ["{", "port_a((unsigned)(unsigned", "long)ps1_data_80020000_pg);", "port_b(0);", "{", "port_c(0);"])
    yield "programs-flow-functions-are-not-rows-of-the-function-table", same(
        [l for l in tables.splitlines() if l.strip().startswith("{ 0x") and "_pg" in l], [])
    rsp = [Path(json.loads(x)).name for x in read(build / "link.rsp").splitlines()]
    first, last = rsp.index("port_game_text_begin.o"), rsp.index("port_game_text_end.o")
    yield "programs-flow-objects-lie-between-the-markers", same(
        ([n for n in rsp[first:last] if n.endswith("_pg.o")], [n for n in rsp[:first] + rsp[last:] if n.endswith("_pg.o")]), (["func_80010000_pg.o", "func_80010010_pg.o"], []))
    obj = read(build / "obj" / "func_80010000_pg.o")
    yield "programs-flow-references-are-renamed", same(sorted(l for l in obj.splitlines() if l.startswith("ref ")), sorted([
        "ref _ps1_callee", "ref _ps1_func_80010010_pg", "ref _ps1_data_80020000_pg", "ref _ps1_sym_a", "ref _ps1_LibRoutine"]))
    yield "programs-flow-definitions-are-impl", same([l for l in obj.splitlines() if l.startswith("def ")], ["def _impl_func_80010000_pg"])
    yield "programs-flow-games-redefine-file-is-as-before", same(
        read(build / "gen" / "redefine.txt"), "".join(f"_{n} _ps1_{n}\n" for n in sorted(["sym_a", "sym_b", "fa", "fb", "fc", "fd", "mf", "libf", "asmf", "callee"])))
    yield "programs-flow-programs-redefine-file-adds-their-names", same(
        [l for l in read(build / "gen" / "redefine_programs.txt").splitlines() if "pg" in l or "Lib" in l],
        ["_LibRoutine _ps1_LibRoutine", "_data_80020000_pg _ps1_data_80020000_pg", "_func_80010000_pg _ps1_func_80010000_pg", "_func_80010010_pg _ps1_func_80010010_pg"])
    flags = hb.COMPILE_FLAGS
    compiled = [x.split() for x in read(root / "pg1.log").splitlines() if " -S " in f" {x} " and x.split()[-1].endswith(("func_80010000_pg.c", "func_80010010_pg.c"))]
    yield "programs-flow-the-programs-units-are-compiled-like-nonmatching-ones", same(
        (len(compiled), all(c[:len(flags)] == flags and c[len(flags):len(flags) + 3] == ["-S", "-I", str(build / "gen")] for c in compiled)), (2, True))

    # The same tree without a list: nothing of the programs is built.
    config = program_tree(root, "pg0")
    cc, nm = fake(root, "pg0", defs=PG_DEFS, calls=PG_CALLS)
    plain = run(root, "pg0", config, cc, nm)
    yield "programs-flow-no-program-listed-builds-none", same(
        (plain.returncode, plain.stdout.splitlines()[-2], "func_80010000_pg" in read(root / "pg0" / "build" / "gen" / "names.ld")), (0, "programs built in: 0", False))
    yield "programs-flow-the-empty-table-is-there", same(
        "const struct port_builtin_program port_programs[] = {\n    { 0, 0, 0, 0, 0, 0 },\n};\nconst unsigned port_program_count = 0;" in read(root / "pg0" / "build" / "gen" / "port_tables.c"), True)
    yield "programs-flow-the-link-response-is-as-without-programs", same(
        [Path(json.loads(x)).name for x in read(root / "pg0" / "build" / "link.rsp").splitlines()],
        ["hole.o", "port_game_text_begin.o", "ua.o", "ub.o", "uc.o", "um.o", "um__mod2.o", "port_game_text_end.o", "main.o", "port_tables.o", "names.ld"])
    yield "programs-flow-the-programs-redefine-file-without-programs-is-the-games", same(
        read(root / "pg0" / "build" / "gen" / "redefine_programs.txt"), read(root / "pg0" / "build" / "gen" / "redefine.txt"))

    # Refusals before any compilation: status 2, one line.
    def refused(tag, needles, **more):
        proc, _ = program_run(root, tag, **more)
        return same((proc.returncode, proc.stdout, len(proc.stderr.splitlines()), all(n in proc.stderr for n in needles), (root / f"{tag}.log").exists()), (2, "", 1, True, False))

    yield "programs-run-unknown-key-refused", refused("pgr1", ["programs-list.toml", "extra"], list_text="extra = 1\n" + PG_LIST)
    yield "programs-run-unknown-program-refused", refused("pgr2", ["zz", "declaration"], list_text=PG_LIST.replace('"pg"', '"zz"', 1))
    yield "programs-run-main-not-a-function-refused", refused("pgr3", ["main", "func_80010099_pg"], list_text=PG_LIST.replace("func_80010000_pg", "func_80010099_pg"))
    yield "programs-run-name-twice-refused", refused("pgr4", ["pg", "twice"], list_text=PG_LIST + PG_LIST)
    yield "programs-run-stem-not-in-the-declaration-refused", refused(
        "pgr5", ["program pg", "func_80010020_pg.c", "no declared function"], files={**PG_FILES, "pg/func_80010020_pg.c": "int x;\n"})
    yield "programs-run-declared-function-without-file-refused", refused(
        "pgr6", ["program pg", "func_80010010_pg", "no file"], files={"pg/func_80010000_pg.c": "int x;\n"})
    yield "programs-run-library-name-not-bound-refused", refused("pgr7", ["pg", "LibRoutine", "library"], list_text=PG_LIST.replace('LibRoutine = "port_lib_routine"', ""))
    yield "programs-run-declaration-with-a-fault-refused", refused("pgr8", ["programs.toml"], decl="[[program]]\nname = 3\n")

    proc, _ = program_run(root, "pgl1", rt_only={})
    yield "programs-run-a-bound-routine-that-the-runtime-lacks-is-a-link-error-that-names-it", same(
        (proc.returncode, "undefined symbol `_port_lib_routine' referenced in expression" in proc.stderr, "linked:" in proc.stdout), (1, True, False))
    yield "programs-link-errors-name-a-name-the-names-file-uses", same(
        hb.link_errors("ld: n.ld:5: undefined symbol `_a' referenced in expression\nld: n.ld:5: undefined symbol `_a' referenced in expression\ncollect2: error: ld returned 1 exit status\n"),
        "undefined symbol `_a' referenced in expression")

    proc, _ = program_run(root, "pgr10", units=RUN_UNITS + unit("func_80010000_pg", "a.c", [("zz", 0x80100300)]), defs={**PG_DEFS, "func_80010000_pg": ["zz"]})
    yield "programs-run-a-program-unit-with-the-name-of-a-unit-of-the-configuration-refused", same(
        (proc.returncode, proc.stdout, len(proc.stderr.splitlines()), "func_80010000_pg" in proc.stderr and "unit" in proc.stderr), (2, "", 1, True))

    # Stops after compilation, status 1.
    def stops(tag, needles, **more):
        proc, _ = program_run(root, tag, **more)
        return same((proc.returncode, all(n in proc.stderr for n in needles), "linked:" in proc.stdout), (1, True, False))

    yield "programs-run-a-plain-undefined-name-in-a-program-object-stops-the-build", stops(
        "pgs1", ["func_80010000_pg", "stray_plain", "ps1_"], calls={"func_80010000_pg": ["stray_plain"]})
    yield "programs-run-a-call-to-a-function-of-the-program-without-c-stops-the-build-naming-it", stops(
        "pgs2", ["func_80010000_pg", "func_80010020_pg"], calls={"func_80010000_pg": ["func_80010010_pg", "func_80010020_pg"]})
    yield "programs-run-the-plain-name-of-a-library-routine-stops-the-build", stops(
        "pgs3", ["func_80010010_pg", "DecDCTout"], calls={"func_80010010_pg": ["DecDCTout"]})
    proc, _ = program_run(root, "pgs4", calls={"func_80010000_pg": ["a_plain_one", "a_plain_one"], "func_80010010_pg": ["another_plain"]})
    yield "programs-run-every-plain-name-of-every-object-is-named-once", same(
        (proc.returncode, proc.stderr.count("a_plain_one"), proc.stderr.count("another_plain"), proc.stderr.count("func_80010010_pg: ")), (1, 1, 1, 1))
    proc, _ = program_run(root, "pgs4b", nm_fail=["func_80010010_pg"])
    yield "programs-run-nm-failing-on-a-program-object-stops-the-build", same((proc.returncode, "nm did not run" in proc.stderr and "func_80010010_pg" in proc.stderr, "linked:" in proc.stdout), (1, True, False))
    yield "programs-run-a-unit-of-a-program-that-defines-another-global-stops-the-build", stops(
        "pgs5", ["func_80010010_pg", "extra_global"], defs={**PG_DEFS, "func_80010010_pg": ["func_80010010_pg", "extra_global"]})
    yield "programs-run-a-unit-that-does-not-define-its-function-stops-the-build", stops(
        "pgs6", ["func_80010010_pg", "does not define"], defs={**PG_DEFS, "func_80010010_pg": []})
    yield "programs-run-a-unit-that-does-not-compile-stops-the-build-with-its-first-error", stops(
        "pgs7", ["func_80010010_pg", "boom in func_80010010_pg"], fail=["func_80010010_pg"])
    yield "programs-run-a-unit-that-does-not-assemble-stops-the-build", stops("pgs8", ["func_80010010_pg", "assembler"], asm_fail=["func_80010010_pg"])
    yield "programs-run-a-bound-name-at-another-address-fails-the-link-check", stops(
        "pgs9", ["LibRoutine", "port_lib_routine"], nm_extra=["00500000 A _ps1_LibRoutine"])
    yield "programs-run-a-program-function-alias-at-another-address-fails-the-link-check", stops(
        "pgs11", ["func_80010000_pg", "host data"], nm_extra=["00700000 B _ps1_func_80010000_pg"])
    yield "programs-run-a-program-function-outside-the-markers-fails-the-check", stops(
        "pgs10", ["impl_func_80010000_pg", "outside the game's code"], nm_extra=["00900000 T _impl_func_80010000_pg"])

    # Two programs: a name in both with one target builds; with two targets it is refused.
    decl = declaration(("pg", "PAC/PG.EXE", PG_FUNCTIONS, PG_SYMBOLS),
                       ("qq", "PAC/QQ.EXE", [("main", 0x80010020)], {"LibRoutine": 0x80011004, "data_80020000_pg": 0x80020000}))
    both = PG_LIST + '\n[[program]]\nname = "qq"\nmain = "func_80010020_qq"\n[program.library]\nLibRoutine = "@@"\n'
    files = {**PG_FILES, "qq/func_80010020_qq.c": "int x;\n"}
    defs = {**PG_DEFS, "func_80010020_qq": ["func_80010020_qq"]}
    proc, build = program_run(root, "pgt1", list_text=both.replace("@@", "port_lib_routine"), decl=decl, files=files, defs=defs)
    yield "programs-run-two-programs-with-a-shared-name-build", same(
        (proc.returncode, proc.stdout.splitlines()[-4:-1], proc.stderr), (0, ["programs built in: 2", "program pg: 2 functions", "program qq: 1 functions"], ""))
    yield "programs-run-the-shared-name-is-defined-once", same(read(build / "gen" / "names.ld").count("_ps1_LibRoutine ="), 1)
    proc, _ = program_run(root, "pgt2", list_text=both.replace("@@", "port_other"), decl=decl, files=files, defs=defs)
    yield "programs-run-a-name-leading-to-two-things-in-two-programs-refused", same(
        (proc.returncode, proc.stdout, len(proc.stderr.splitlines()), all(n in proc.stderr for n in ("LibRoutine", "port_lib_routine", "port_other"))), (2, "", 1, True))


# The overrides built and run for real (needs --cc).

E2E_UNITS = (
    unit("ua", "a.c", [("fa", 0x80100000), ("fb", 0x80100010)])
    + unit("ub", "b.c", [("fc", 0x80100020), ("game_a", 0x80100030), ("game_c", 0x80100040)])
    + unit("uc", "c.c", [("fe", 0x80100050)])
    + unit("um", "d.c", [("mf", 0x801e0000), ("mg", 0x801e0010)], image="mod")
)
E2E_SYMBOLS = "sym_in = 0x801e0100;\n"
E2E_HEADER = """/* made up */
int printf(const char *, ...);
extern int sym_in;
extern void (*cb)(void);
int fa(int x); int fb(int x); int fc(int v); void fe(int v); int mf(void); int mg(void);
void game_a(void); void game_c(void);
"""
E2E_SOURCES = {
    "a.c": """#include "game.h"
int fa(int x) { printf("fa: the unit's own C, x=%d\\n", x); return x + 1; }
int fb(int x) { printf("fb: the unit's own C\\n"); return fa(x) * 10; }
""",
    "b.c": """#include "game.h"
void (*cb)(void);
/* the call through a pointer declared without parameters: on the console the value is still in the argument register */
int fc(int v) { printf("fc: the unit's own C\\n"); cb(); return v; }
void game_a(void)
{
    int r = fa(1);
    printf("main: fa -> %d\\n", r);
    r = fb(2);
    printf("main: fb -> %d\\n", r);
}
void game_c(void)
{
    cb = (void (*)(void))fe;
    fc(41);
}
""",
    "c.c": """#include "game.h"
void fe(int v) { printf("fe: v=%d\\n", v); }
""",
    "d.c": """#include "game.h"
int mf(void) { printf("mf: the unit's own C, sym_in at 0x%08x\\n", (unsigned)&sym_in); return 1; }
int mg(void) { printf("mg: the unit's own C, sym_in at 0x%08x\\n", (unsigned)&sym_in); return 2; }
""",
}
E2E_OVERRIDES = {
    "fa": """#include "../src/game.h"
int fa(int x) { printf("fa: the override, x=%d\\n", x); return x + 2; }
""",
    "fc": """#include "../src/game.h"
int fc(int v) { printf("fc: the override, v=%d\\n", v); ((void (*)(int))cb)(v); return v; }
""",
    "mf": """#include "../src/game.h"
static unsigned where(void) { return (unsigned)&sym_in; }
int mf(void) { printf("mf: the override, sym_in at 0x%08x\\n", where()); return 1; }
""",
}
E2E_MAIN = r"""
#include "port_tables.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

extern void ps1_game_a(void), ps1_game_c(void), ps1_mf(void), ps1_mf__mod2(void), ps1_mg(void), ps1_mg__mod2(void);

int main(int argc, char **argv)
{
    unsigned i, flagged = 0;
    unsigned char *ram = VirtualAlloc((void *)0x80100000u, 0x100000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!ram) { printf("no memory\n"); return 3; }
    for (i = 0; i < port_function_count; i++) {
        unsigned char *site = (unsigned char *)(size_t)port_functions[i].address;
        unsigned rel = (unsigned)((size_t)port_functions[i].impl - ((size_t)site + 5));
        site[0] = 0xe9;
        memcpy(site + 1, &rel, 4);
        flagged += port_functions[i].overridden != 0;
    }
    printf("overrides in C: %u\n", flagged);
    fflush(stdout);
    if (argc < 2) return 4;
    if (strcmp(argv[1], "a") == 0) ps1_game_a();
    else if (strcmp(argv[1], "c") == 0) ps1_game_c();
    else if (strcmp(argv[1], "mf") == 0) ps1_mf();
    else if (strcmp(argv[1], "mf2") == 0) ps1_mf__mod2();
    else if (strcmp(argv[1], "mg") == 0) ps1_mg();
    else if (strcmp(argv[1], "mg2") == 0) ps1_mg__mod2();
    else return 5;
    printf("end\n");
    fflush(stdout);
    return 0;
}
"""


E2E_PROGRAM_DECLARATION = declaration(("pg", "PAC/PG.EXE", [("main", 0x80010000), ("helper", 0x80010010)],
                                      {"data_80100800_pg": 0x80100800, "LibHello": 0x80011000, "sym_in": 0x801e0104}))
E2E_PROGRAM_LIST = """[[program]]
name = "pg"
main = "func_80010000_pg"
setup = [ { call = "port_setup_hook", symbol = "data_80100800_pg" }, { call = "port_setup_hook" } ]
teardown = [ { call = "port_teardown_hook", symbol = "data_80100800_pg" } ]

[program.library]
LibHello = "port_lib_hello"
"""
E2E_PROGRAM_FILES = {
    "pg/func_80010000_pg.c": """extern int data_80100800_pg;
extern void func_80010010_pg(int v);
extern int sym_in;
void func_80010000_pg(void)
{
    func_80010010_pg(1);
    func_80010010_pg(data_80100800_pg);
}
""",
    "pg/func_80010010_pg.c": """extern void LibHello(int v);
void func_80010010_pg(int v) { LibHello(v + 100); }
""",
}
E2E_PROGRAM_MAIN = r"""
#include "port_tables.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void port_lib_hello(int v) { printf("lib: %d\n", v); }
void port_setup_hook(unsigned address) { printf("setup: 0x%08x\n", address); }
void port_teardown_hook(unsigned address) { printf("teardown: 0x%08x\n", address); }

int main(void)
{
    unsigned i;
    unsigned char *ram = VirtualAlloc((void *)0x80100000u, 0x100000u, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!ram) { printf("no memory\n"); return 3; }
    *(int *)(size_t)0x80100800u = 40;
    printf("programs: %u, functions with jumps: %u\n", port_program_count, port_function_count);
    for (i = 0; i < port_program_count; i++) {
        const struct port_builtin_program *p = &port_programs[i];
        printf("program %s %s sha %02x%02x\n", p->name, p->file, p->sha256[0], p->sha256[31]);
        if (p->setup) p->setup();
        ((void (*)(void))p->main)();
        if (p->teardown) p->teardown();
    }
    printf("end\n");
    fflush(stdout);
    return 0;
}
"""


class Rig:
    """Builds the made-up game with hostbuild.py and the real compiler and starts the Windows programs."""

    def __init__(self, cc: str, prefix: list[str], work: Path):
        self.cc, self.prefix, self.work = cc, prefix, work
        self.silent: list[str] = []   # the standard error of each run that ended with status 1 and printed nothing

    def start_check(self) -> str | None:
        probe = self.work / "probe.c"
        probe.write_text("int main(void) { return 7; }\n")
        out = self.work / "probe.exe"
        proc = subprocess.run([self.cc, "-o", str(out), str(probe)], capture_output=True, text=True, timeout=120)
        if proc.returncode != 0:
            return "the compiler cannot build a program: " + (proc.stderr.strip().splitlines() or ["?"])[0]
        try:
            ran = subprocess.run([*self.prefix, str(out)], capture_output=True, timeout=60)
        except (OSError, subprocess.TimeoutExpired) as err:
            return f"cannot start a Windows program: {err}"
        return None if ran.returncode == 7 else f"cannot start a Windows program (status {ran.returncode})"

    def build(self, tag: str, overrides: tuple[str, ...]) -> tuple[subprocess.CompletedProcess, Path]:
        """hostbuild.py on the made-up game with the given overrides; (the process, the program's path)."""
        root = self.work
        run_tree(root, tag, units=E2E_UNITS, symbols=E2E_SYMBOLS)
        base = root / tag / "src"
        write(base / "game.h", E2E_HEADER)
        for name, text in E2E_SOURCES.items():
            write(base / name, text)
        runtime = root / tag / "runtime"
        write(runtime / "port_tables.h", (TOOLS.parent / "src" / "port_tables.h").read_text())
        write(runtime / "main.c", E2E_MAIN)
        for name in overrides:
            write(root / tag / "overrides" / f"{name}.c", E2E_OVERRIDES[name])
            write(root / tag / "overrides" / f"{name}.py", "# the contract of the differential test, made up\n")
        proc = subprocess.run(
            [sys.executable, str(SCRIPT), "--config", str(base / "build.toml"), "--cc", self.cc, "--build", str(root / tag / "build"),
             "--runtime", str(runtime), "--jobs", "2", "--sweep-rows", str(root / "no-sweep-rows.toml")],
            capture_output=True, text=True, timeout=900)
        return proc, root / tag / "build" / "sfa2.exe"

    def build_program(self, tag: str, list_text: str, files: dict[str, str]) -> tuple[subprocess.CompletedProcess, Path]:
        """hostbuild.py on the made-up game with a made-up program built in, with the real compiler."""
        root = self.work
        run_tree(root, tag, units=E2E_UNITS, symbols=E2E_SYMBOLS)
        base = root / tag / "src"
        write(base / "game.h", E2E_HEADER)
        for name, text in E2E_SOURCES.items():
            write(base / name, text)
        write(base / "programs" / "programs.toml", E2E_PROGRAM_DECLARATION)
        for rel, text in files.items():
            write(base / "programs" / rel, text)
        write(root / tag / "programs-list.toml", list_text)
        runtime = root / tag / "runtime"
        write(runtime / "port_tables.h", (TOOLS.parent / "src" / "port_tables.h").read_text())
        write(runtime / "main.c", E2E_PROGRAM_MAIN)
        proc = subprocess.run(
            [sys.executable, str(SCRIPT), "--config", str(base / "build.toml"), "--cc", self.cc, "--build", str(root / tag / "build"),
             "--runtime", str(runtime), "--jobs", "2", "--sweep-rows", str(root / "no-sweep-rows.toml"), "--programs", str(root / tag / "programs-list.toml")],
            capture_output=True, text=True, timeout=900)
        return proc, root / tag / "build" / "sfa2.exe"

    def run(self, exe: Path, *args: str) -> tuple[int, list[str]]:
        """One run of the program: its status and the lines it printed. Nothing is run a second time. A run that
        ends with status 1 and prints nothing fails its case like any other wrong result; its standard error is
        kept for the note at the end (a program started from a loaded WSL shell can end so before it runs)."""
        proc = subprocess.run([*self.prefix, str(exe), *args], capture_output=True, text=True, timeout=120)
        lines = proc.stdout.replace("\r\n", "\n").splitlines()
        if proc.returncode == 1 and not lines:
            self.silent.append(proc.stderr.strip())
        return proc.returncode, lines


def e2e_cases(rig: Rig):
    proc, exe = rig.build("e2e-over", ("fa", "fc", "mf"))
    yield "e2e-build-with-overrides-verifies", same((proc.returncode, proc.stderr, proc.stdout.splitlines()[3:5]), (0, "", ["functions with C: 10", "functions overridden in C: 4"]))
    if proc.returncode != 0:
        return
    status, lines = rig.run(exe, "a")
    yield "e2e-the-override-runs-for-a-call-from-another-unit-and-from-the-unit-that-defines-it", same((status, lines), (0, [
        "overrides in C: 4", "fa: the override, x=1", "main: fa -> 3", "fb: the unit's own C", "fa: the override, x=2", "main: fb -> 40", "end"]))
    status, lines = rig.run(exe, "c")
    yield "e2e-a-call-through-a-pointer-without-parameters-reaches-a-callee-that-reads-its-parameter", same((status, lines), (0, [
        "overrides in C: 4", "fc: the override, v=41", "fe: v=41", "end"]))
    status, lines = rig.run(exe, "mf")
    yield "e2e-first-placement-runs-the-override-with-its-own-name-of-the-data", same((status, lines), (0, ["overrides in C: 4", "mf: the override, sym_in at 0x801e0100", "end"]))
    status, lines = rig.run(exe, "mf2")
    yield "e2e-second-placement-runs-the-override-with-the-moved-name-of-the-data", same((status, lines), (0, ["overrides in C: 4", "mf: the override, sym_in at 0x801f0100", "end"]))
    status, lines = rig.run(exe, "mg")
    status2, lines2 = rig.run(exe, "mg2")
    yield "e2e-the-units-other-functions-keep-their-own-c-in-both-placements", same(
        ((status, lines[1:]), (status2, lines2[1:])),
        ((0, ["mg: the unit's own C, sym_in at 0x801e0100", "end"]), (0, ["mg: the unit's own C, sym_in at 0x801f0100", "end"])))

    # The same game without any override: the unit's C runs, the count is 0.
    proc, exe0 = rig.build("e2e-plain", ())
    yield "e2e-build-without-overrides-says-0", same((proc.returncode, proc.stdout.splitlines()[3:5]), (0, ["functions with C: 10", "functions overridden in C: 0"]))
    if proc.returncode != 0:
        return
    status, lines = rig.run(exe0, "a")
    yield "e2e-without-the-override-the-units-c-runs", same((status, lines), (0, [
        "overrides in C: 0", "fa: the unit's own C, x=1", "main: fa -> 2", "fb: the unit's own C", "fa: the unit's own C, x=2", "main: fb -> 30", "end"]))
    status, lines = rig.run(exe0, "mf2")
    yield "e2e-without-the-override-the-second-placement-runs-the-units-c", same((status, lines), (0, ["overrides in C: 0", "mf: the unit's own C, sym_in at 0x801f0100", "end"]))
    # Not asserted: what fe prints when the unit's own fc calls it through cb() with no argument. The value it reads is
    # whatever lies in the stack slot, which this file does not control, so no case states it.


def e2e_program_cases(rig: Rig):
    """A made-up program built in with the real compiler and linker; the program's main runs its helper, reads its
    data by the absolute address, calls a bound routine of the runtime, and the generated setup and teardown run."""
    proc, exe = rig.build_program("e2e-program", E2E_PROGRAM_LIST, E2E_PROGRAM_FILES)
    yield "e2e-program-build-verifies", same((proc.returncode, proc.stderr, proc.stdout.splitlines()[-3:-1]), (0, "", ["programs built in: 1", "program pg: 2 functions"]))
    if proc.returncode != 0:
        return
    status, lines = rig.run(exe)
    yield "e2e-program-runs-with-setup-main-and-teardown", same((status, lines), (0, [
        "programs: 1, functions with jumps: 10", "program pg pg.exe sha 203f", "setup: 0x80100800", "setup: 0x00000000", "lib: 101", "lib: 140", "teardown: 0x80100800", "end"]))
    proc, _ = rig.build_program("e2e-program-stray", E2E_PROGRAM_LIST, {**E2E_PROGRAM_FILES, "pg/func_80010010_pg.c": "extern int printf(const char *, ...);\nvoid func_80010010_pg(int v) { printf(\"x\"); }\n"})
    yield "e2e-program-a-plain-library-call-stops-the-build-naming-it", same((proc.returncode, "func_80010010_pg" in proc.stderr and "printf" in proc.stderr, "linked:" in proc.stdout), (1, True, False))


def groups(root: Path, rig: "Rig | None" = None):
    yield rename_cases()
    yield sweep_table_cases(root)
    yield placement_cases()
    yield names_cases()
    yield selection_cases(root)
    yield table_cases(root)
    yield verify_cases()
    yield marker_cases()
    yield image_cases()
    yield flow_cases(root)
    yield like_cases(root)
    yield sweep_flow_cases(root)
    yield marker_flow_cases(root)
    yield image_flow_cases(root)
    yield psyz_cases(root)
    yield sound_cases(root)
    yield override_rename_cases()
    yield override_check_cases()
    yield override_flow_cases(root)
    yield program_read_cases(root)
    yield program_plan_cases(root)
    yield program_unit_cases(root)
    yield program_option_cases()
    yield program_pure_cases()
    yield program_flow_cases(root)
    if rig:
        yield e2e_cases(rig)
        yield e2e_program_cases(rig)


def main() -> int:
    parser = argparse.ArgumentParser(description="Controls for hostbuild.py.")
    parser.add_argument("--cc", default=None, help="cross compiler: also build and run the overrides for real")
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    rig = None
    work = None
    if args.cc:
        if not shutil.which(args.cc):
            print(f"the cross compiler {args.cc} is missing; these controls need it")
            return 2
        build = TOOLS.parent / "build"
        build.mkdir(parents=True, exist_ok=True)
        work = Path(tempfile.mkdtemp(prefix="hostbuild-e2e-", dir=build))
        rig = Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            shutil.rmtree(work, ignore_errors=True)
            return 2
    else:
        print("note: without --cc the overrides are not built and run for real (the e2e- cases are not run)")
    try:
        failed = run_all(rig)
    finally:
        if work:
            shutil.rmtree(work, ignore_errors=True)
    if rig and rig.silent:
        print(f"note: {len(rig.silent)} program run(s) ended with status 1 and printed nothing; their cases failed. "
              f"Standard error of the last: {rig.silent[-1] or '(empty)'}")
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


def run_all(rig: "Rig | None") -> int:
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostbuild-test-") as tmp:
        for produced in groups(Path(tmp), rig):
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
    return failed


if __name__ == "__main__":
    sys.exit(main())
