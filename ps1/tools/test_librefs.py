#!/usr/bin/env python3
"""Controls for librefs.py using synthetic inputs.

Builds a small fake PS-X executable, a build configuration, a symbol file and
the unit objects of a build, in a temporary directory. The objects are ELF
relocatable files written here by hand, one per unit. No game data is
involved. The expected output is worked out here from the layout, not read
from the tool.
"""

from __future__ import annotations

import collections
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile

from test_families import ONE, OPEN, RETURN, CLOSE, START, executable, lay_out, quiet, verdict

TOOLS = Path(__file__).resolve().parent
OWN, REF, TWO, NONE = "named by its own unit", "named by reference", "named two ways", "without a name"
CALL, HI16, LO16, R32 = 4, 5, 6, 2  # R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_32


def tool(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOLS / "librefs.py"), *map(str, args)], capture_output=True, text=True)


def elf_bytes(sections: list[str], relocs: list[tuple[str, str, list[tuple[str | None, int]]]]) -> bytes:
    """A 32-bit little-endian MIPS relocatable file.

    `sections` are the names of the sections of 16 zero bytes. `relocs` are
    (target section, "rel" or "rela", [(symbol name or None, type)]). A symbol
    without a name is the section symbol of the first section; any other is
    an undefined global.
    """
    names = sorted({name for _, _, entries in relocs for name, _ in entries if name})
    strtab = b"\0"
    offset_of = {}
    for name in names:
        offset_of[name] = len(strtab)
        strtab += name.encode() + b"\0"
    symbols = struct.pack("<IIIBBH", 0, 0, 0, 0, 0, 0) + struct.pack("<IIIBBH", 0, 0, 0, 3, 0, 1)
    symbols += b"".join(struct.pack("<IIIBBH", offset_of[name], 0, 0, 0x10, 0, 0) for name in names)
    index_of = {name: 2 + n for n, name in enumerate(names)}
    index_of[None] = 1

    plan = [("", 0, b"", 0, 0, 0, 0)]  # name, type, body, link, info, align, entsize
    for name in sections:
        plan.append((name, 1, bytes(16), 0, 0, 4, 0))
    symtab = 1 + len(sections) + len(relocs)
    for target, kind, entries in relocs:
        body = b""
        for n, (name, typ) in enumerate(entries):
            info = index_of[name] << 8 | typ
            body += struct.pack("<II", 4 * n, info) + (struct.pack("<i", 0) if kind == "rela" else b"")
        plan.append((f".{kind}{target}", 9 if kind == "rel" else 4, body, symtab, 1 + sections.index(target), 4, 8 if kind == "rel" else 12))
    plan.append((".symtab", 2, symbols, symtab + 1, 2, 4, 16))
    plan.append((".strtab", 3, strtab, 0, 0, 1, 0))
    shstrtab, shname = b"\0", {}
    for name, *_ in plan[1:]:
        shname[name] = len(shstrtab)
        shstrtab += name.encode() + b"\0"
    shname[".shstrtab"] = len(shstrtab)
    shstrtab += b".shstrtab\0"
    plan.append((".shstrtab", 3, shstrtab, 0, 0, 1, 0))
    shname[""] = 0

    body, headers, position = b"", b"", 52
    for name, typ, content, link, info, align, entsize in plan:
        pad = -position % 4
        body += bytes(pad) + content
        position += pad
        headers += struct.pack("<IIIIIIIIII", shname[name], typ, 0, 0, position if typ else 0, len(content), link, info, align, entsize)
        position += len(content)
    pad = -position % 4
    header = b"\x7fELF" + bytes([1, 1, 1, 0]) + bytes(8)
    header += struct.pack("<HHIIIIIHHHHHH", 1, 8, 1, 0, 0, position + pad, 0, 52, 0, 0, 40, len(plan), len(plan) - 1)
    return header + body + bytes(pad) + headers


def writer_cases(root: Path):
    """The writer, read back with the reader of the tool."""
    sections = [".text", ".data"]
    relocs = [(".text", "rel", [("fa", 4), (None, 5), ("fb", 6)]), (".data", "rela", [("fa", 2)])]
    path = root / "writer.o"
    path.write_bytes(elf_bytes(sections, relocs))
    with open(path, "rb") as handle:
        elf = ELFFile(handle)
        found = []
        for section in elf.iter_sections():
            if section.header.sh_type in ("SHT_REL", "SHT_RELA"):
                target = elf.get_section(section.header.sh_info).name
                symbols = elf.get_section(section.header.sh_link)
                found.append((section.name, target, [(symbols.get_symbol(r["r_info_sym"]).name, r["r_info_type"]) for r in section.iter_relocations()]))
        names = [s.name for s in elf.iter_sections()]
        header = (elf.header["e_machine"], elf.header["e_type"], elf.little_endian, elf.elfclass)
    want = [(".rel.text", ".text", [("fa", 4), ("", 5), ("fb", 6)]), (".rela.data", ".data", [("fa", 2)])]
    yield "writer-sections", verdict(names == ["", ".text", ".data", ".rel.text", ".rela.data", ".symtab", ".strtab", ".shstrtab"], f"{names}"), 0, "as required"
    yield "writer-relocations", verdict(found == want, f"{found}\n{want}"), 0, "as required"
    yield "writer-header", verdict(header == ("EM_MIPS", "ET_REL", True, 32), f"{header}"), 0, "as required"


# Every function of the layout, in order: (name, the name its unit declares or None, its class, [(reference name, counts)]).
# Counts: units that call it, form its address in code, hold it in data.
FUNCTIONS = [
    ("g0", "g0", None, []),
    ("g1", None, None, []),
    ("L_own", "own1", OWN, []),
    ("L_ref", None, REF, [("sym_ref", (2, 3, 1))]),
    ("L_ph1", "FUN_1", REF, [("ref_ph", (1, 0, 0))]),
    ("L_ph2", "decl_game", REF, [("decl_game", (1, 0, 0))]),
    ("L_two", None, TWO, [("two_a", (1, 0, 0)), ("two_b", (1, 0, 0))]),
    ("L_ownd", "own_d", TWO, [("alt_d", (1, 0, 0))]),
    ("L_dup1", "dup", REF, [("dup", (1, 0, 0))]),
    ("L_dup2", None, NONE, []),
    ("L_none", None, NONE, []),
    ("L_out", "z_name", NONE, []),
    ("L_img", None, NONE, []),
    ("L_res", None, REF, [("res_name", (1, 0, 0))]),
    ("L_mid", None, NONE, []),
    ("L_data", None, REF, [("data_name", (0, 0, 1))]),
    ("L_rela", None, REF, [("rela_fn", (1, 0, 1))]),
    ("L_tx", None, REF, [("text_x", (1, 1, 0))]),
]
NAMES = [f[0] for f in FUNCTIONS]
SDK_UNITS = 7  # a, b, c, d, e, rela and res; not q (another image), z (sdkz/ is not sdk/), game and m (outside sdk/, m has no object)


def build(root: Path, start: int = START):
    """The executable, its addresses and sizes, the configuration, the symbol file and the build folder."""
    plain = [OPEN, ONE, RETURN, CLOSE]
    exe_bytes, a, size = lay_out([(name, lambda book: plain) for name in NAMES], start)
    end = start + len(exe_bytes) - 0x800
    exe = root / "librefs.exe"
    exe.write_bytes(exe_bytes)

    def functions(*items):
        return "functions = [" + ", ".join("{ " + (f'name = "{n}", ' if n else "") + f"address = {a[f]} }}" for f, n in items) + "]\n\n"

    units = [
        ("a", "sdk/a.c", None, [("L_own", "own1"), ("L_ownd", "own_d")]),
        ("b", "sdk/b.c", None, []),
        ("c", "sdk/c/c.c", None, []),
        ("d", "sdk/d.c", None, []),
        ("e", "sdk/e.c", None, []),
        ("rela", "sdk/rela.c", None, []),
        ("res", "sdk/r.c", "resident", []),
        ("q", "sdk/q.c", "other", [("L_none", "q_name")]),
        ("z", "sdkz/z.c", None, [("L_out", "z_name")]),
        ("m", "game/m.c", None, []),
        ("game", "game/x.c", None, [("g0", "g0"), ("g1", None), ("L_ph1", "FUN_1"), ("L_ph2", "decl_game"), ("L_dup1", "dup")]),
    ]
    text = ""
    for name, source, image, items in units:
        text += f'[[unit]]\nname = "{name}"\nsource = "{source}"\n' + (f'image = "{image}"\n' if image else "") + functions(*items)
    config = root / "librefs.toml"
    config.write_text(text)

    symbols = root / "librefs.ld"
    values = {
        "sym_ref": a["L_ref"], "ref_ph": a["L_ph1"], "two_a": a["L_two"], "two_b": a["L_two"], "alt_d": a["L_ownd"],
        "dup": a["L_dup2"], "to_game": a["g0"], "to_mid": a["L_mid"] + 4, "noref": a["L_out"], "noimg": a["L_img"],
        "res_name": a["L_res"], "data_name": a["L_data"], "text_x": a["L_tx"], "rela_fn": a["L_rela"],
    }  # fmt: skip
    symbols.write_text("/* the symbols */\n" + "".join(f"{name} = {value:#x};\n" for name, value in values.items()))

    objects = {
        "a": ([".text"], [(".text", "rel", [])]),
        "b": (
            [".text", ".data", ".rodata"],
            [
                (".text", "rel", [(n, CALL) for n in ("sym_ref", "own1", "alt_d", "two_b", "to_game", "to_mid", "undefined_x", "ref_ph", "decl_game", "dup")]
                 + [(None, CALL), (None, HI16), ("sym_ref", HI16), ("sym_ref", LO16)]),
                (".data", "rel", [("sym_ref", R32), (None, R32)]),
                (".rodata", "rel", [("data_name", CALL)]),  # type 4 outside code is an address in data
            ],
        ),
        "c": (
            [".text", ".text.hot"],
            [(".text", "rel", [("sym_ref", CALL), ("sym_ref", CALL), ("two_a", CALL)]), (".text.hot", "rel", [("text_x", CALL)])],
        ),
        "d": ([".text", ".text.hot"], [(".text", "rel", [("sym_ref", HI16)]), (".text.hot", "rel", [("text_x", HI16)])]),
        "e": ([".text"], [(".text", "rel", [("sym_ref", LO16)])]),
        "rela": ([".text", ".data"], [(".text", "rela", [("rela_fn", CALL)]), (".data", "rela", [("rela_fn", R32)])]),
        "res": ([".text"], [(".text", "rel", [("res_name", CALL)])]),
        "q": ([".text"], [(".text", "rel", [("noimg", CALL)])]),
        "z": ([".text"], [(".text", "rel", [("noref", CALL)])]),
    }
    folder = root / "objects"
    folder.mkdir()
    for name, (sections, relocs) in objects.items():
        (folder / f"unit-{name}.o").write_bytes(elf_bytes(sections, relocs))
    return exe, a, size, end, config, symbols, folder


def expected(a, size, end, first=0, library=2, last=None):
    """What a run over functions first..last of the layout, library from the function `library`, must print."""
    last = len(FUNCTIONS) if last is None else last
    swept = FUNCTIONS[first:last]
    lib = FUNCTIONS[library:last]
    lib = [f for f in lib if f in swept]
    classes = collections.Counter(f[2] for f in lib)
    start = a[NAMES[first]]
    limit = end if last == len(FUNCTIONS) else a[NAMES[last]]
    lines = [
        f"swept {start:#x} to {limit:#x}: {len(swept)} functions, {len(lib)} library",
        f"units under sdk/: {SDK_UNITS}",
        *[f"{c}: {classes[c]}" for c in (OWN, REF, TWO, NONE)],
    ]
    for wanted in (REF, TWO):
        for name, shown, cls, refs in lib:
            if cls == wanted:
                for ref, (calls, code, data) in refs:
                    lines.append(f"  {a[name]:#x} {shown or '-'}: {ref}, units that call it {calls}, form its address {code}, hold it in data {data}")
    rows = ""
    for name, shown, cls, refs in lib:
        for ref, counts in refs if cls in (REF, TWO) else [("-", (0, 0, 0))]:
            rows += f"{a[name]:08x}\t{size[name]}\t{shown or '-'}\t{cls}\t{ref}\t" + "\t".join(map(str, counts)) + "\n"
    status = 1 if classes[TWO] else 0
    return "".join(line + "\n" for line in lines), rows, status


def main_cases(root: Path):
    exe, a, size, end, config, symbols, folder = build(root)
    base = [exe, "--config", config, "--build", folder, "--symbols", symbols, "--library", f"{a['L_own']:#x}", "--end", f"{end:#x}"]

    def check(label, proc, want, status, out=None):
        ok = proc.returncode == status and proc.stdout == want
        yield label, verdict(ok, f"exit {proc.returncode} wanted {status}\n--- got\n{proc.stdout}--- wanted\n{want}{proc.stderr}"), 0, "as required"

    text, rows, status = expected(a, size, end)
    assert status == 1
    out = root / "deep" / "er" / "librefs.tsv"
    proc = tool(*base, "--out", out)
    yield from check("summary-exact", proc, text, 1)
    got = out.read_text() if out.exists() else None
    yield "out-rows", verdict(got == rows, f"{got!r}\n{rows!r}"), 0, "as required"
    alone = tool(*base)
    yield from check("no-file-without-out", alone, text, 1)
    yield "no-out-file-written", verdict(not (root / "librefs.tsv").exists() and len(list(root.glob("*.tsv"))) == 0, "a file was written"), 0, "as required"

    # The sweep starts at the image start when --start is not given.
    yield from check("start-given-as-default", tool(*base, "--start", f"{START:#x}"), text, 1)
    # From a function inside the image: the earlier ones are not swept.
    first = NAMES.index("L_ref")
    t, r, s = expected(a, size, end, first=first)
    yield from check("start-inside", tool(*base, "--start", f"{a['L_ref']:#x}"), t, s)
    # The limit between game and library functions.
    t, r, s = expected(a, size, end, library=first)
    lib_args = [*base]
    lib_args[lib_args.index("--library") + 1] = f"{a['L_ref']:#x}"
    yield from check("library-limit", tool(*lib_args), t, s)
    # An end inside the image leaves out the functions after it; with no contradiction before it the status is 0.
    last = NAMES.index("L_two")
    t, r, s = expected(a, size, end, last=last)
    assert s == 0
    short = [*base]
    short[short.index("--end") + 1] = f"{a['L_two']:#x}"
    out2 = root / "cut.tsv"
    yield from check("end-inside", tool(*short, "--out", out2), t, 0)
    got = out2.read_text() if out2.exists() else None
    yield "end-inside-rows", verdict(got == r, f"{got!r}\n{r!r}"), 0, "as required"
    # The same run with every function a game function: nothing is library, and the file is empty.
    none_lib = [*base]
    none_lib[none_lib.index("--library") + 1] = f"{end:#x}"
    out3 = root / "nolib.tsv"
    zero = (
        f"swept {START:#x} to {end:#x}: {len(FUNCTIONS)} functions, 0 library\nunits under sdk/: {SDK_UNITS}\n"
        + "".join(f"{c}: 0\n" for c in (OWN, REF, TWO, NONE))
    )
    yield from check("no-library-function", tool(*none_lib, "--out", out3), zero, 0)
    yield "no-library-rows", verdict(out3.exists() and out3.read_text() == "", repr(out3.read_text() if out3.exists() else None)), 0, "as required"

    # Addresses below 0x10000000 still have eight digits in a row of the file.
    low_root = root / "low"
    low_root.mkdir()
    lexe, la, lsize, lend, lconfig, lsymbols, lfolder = build(low_root, 0x00010000)
    lt, lr, ls = expected(la, lsize, lend)
    lout = root / "low.tsv"
    lproc = tool(lexe, "--config", lconfig, "--build", lfolder, "--symbols", lsymbols, "--library", f"{la['L_own']:#x}", "--end", f"{lend:#x}", "--out", lout)
    yield from check("low-addresses-summary", lproc, lt, 1)
    got = lout.read_text() if lout.exists() else None
    yield "low-addresses-rows", verdict(got == lr and got.startswith("00010020\t"), f"{got!r}\n{lr!r}"), 0, "as required"
    # Addresses may be given in decimal.
    decimal = [*base]
    decimal[decimal.index("--library") + 1] = str(a["L_own"])
    decimal[decimal.index("--end") + 1] = str(end)
    yield from check("addresses-in-decimal", tool(*decimal), text, 1)
    # Without a contradiction the status is 0 even when functions have no name.
    after = NAMES.index("L_dup1")
    t, r, s = expected(a, size, end, first=after)
    assert s == 0 and "without a name: 5\n" in t
    yield from check("nameless-is-no-contradiction", tool(*base, "--start", f"{a['L_dup1']:#x}"), t, 0)


def region_cases(root: Path):
    """Which units are read, which relocations count, and what a name stands for, one rule at a time."""
    exe, a, size, end, config, symbols, folder = build(root)
    text, _, _ = expected(a, size, end)
    base = [exe, "--config", config, "--build", folder, "--symbols", symbols, "--library", f"{a['L_own']:#x}", "--end", f"{end:#x}"]
    proc = tool(*base)
    out = proc.stdout

    def line(name, ref):
        return next((l for l in out.splitlines() if l.startswith(f"  {a[name]:#x} ") and f": {ref}," in l), None)

    def has(label, name, ref, counts, shown="-"):
        calls, code, data = counts
        want = f"  {a[name]:#x} {shown}: {ref}, units that call it {calls}, form its address {code}, hold it in data {data}"
        yield label, verdict(line(name, ref) == want, f"{line(name, ref)!r}\n{want!r}"), 0, "as required"

    # Two relocations of one kind in one unit count once; a second unit counts again.
    yield from has("count-units-not-relocations", "L_ref", "sym_ref", (2, 3, 1))
    yield from has("rela-text-and-rela-data-count", "L_rela", "rela_fn", (1, 0, 1))
    yield from has("text-subsection-is-code", "L_tx", "text_x", (1, 1, 0))
    yield from has("type-4-outside-code-is-data", "L_data", "data_name", (0, 0, 1))
    yield from has("explicit-resident-unit-is-read", "L_res", "res_name", (1, 0, 0))
    yield from has("placeholder-function-named-by-reference", "L_ph1", "ref_ph", (1, 0, 0), "FUN_1")
    yield from has("declared-function-by-game-unit", "L_ph2", "decl_game", (1, 0, 0), "decl_game")
    yield from has("declared-function-wins-over-symbol-file", "L_dup1", "dup", (1, 0, 0), "dup")
    mentioned = [name for name in ("L_out", "L_img", "L_dup2", "L_mid", "L_none") if f"  {a[name]:#x} " in out]
    yield "not-named-by-others", verdict(not mentioned, f"{mentioned}\n{out}"), 0, "as required"
    # The unit of another image and the unit outside sdk/ hold relocations but are not read: even a missing object is no error.
    yield "unread-units-no-error", verdict(proc.returncode == 1 and "unit-m.o" not in out, out), 0, "as required"
    yield "no-game-function-listed", verdict(f"  {a['g0']:#x} " not in out and f"  {a['g1']:#x} " not in out, out), 0, "as required"

    # A contradiction: two names by reference, and a name other than the one the sdk/ unit declares.
    yield "two-ways-lines", verdict(
        line("L_two", "two_a") is not None and line("L_two", "two_b") is not None and line("L_ownd", "alt_d") is not None and "own_d: alt_d," in out,
        out,
    ), 0, "as required"
    yield "two-ways-is-status-1", verdict(proc.returncode == 1, str(proc.returncode)), 0, "as required"
    # A function that its own unit declares, referred to under the same name by another sdk/ unit, is not a contradiction.
    t, _, s = expected(a, size, end, last=NAMES.index("L_two"))
    short = [*base]
    short[short.index("--end") + 1] = f"{a['L_two']:#x}"
    own_run = tool(*short)
    yield "own-name-no-status", verdict(own_run.returncode == 0 and "named by its own unit: 1\n" in own_run.stdout and "own1" not in own_run.stdout.split("units under")[1], own_run.stdout), 0, "as required"


def trees(root: Path):
    """Variations of the inputs, each one run with the tool."""
    exe, a, size, end, config, symbols, folder = build(root)
    args = [exe, "--config", config, "--build", folder, "--symbols", symbols, "--library", f"{a['L_own']:#x}", "--end", f"{end:#x}"]

    def with_arg(**changes):
        out = list(args)
        for key, value in changes.items():
            out[out.index(f"--{key}") + 1] = value
        return out

    # An object that does not exist: the path is named and nothing else is read.
    short = root / "objects-short"
    short.mkdir()
    for path in folder.iterdir():
        if path.name != "unit-c.o":
            (short / path.name).write_bytes(path.read_bytes())
    proc = quiet(tool(*with_arg(build=short)))
    yield "object-missing", verdict(proc.returncode == 1 and f"{short / 'unit-c.o'}:" in proc.stdout and "No such file" in proc.stdout, f"{proc.returncode}\n{proc.stdout}{proc.stderr}"), 0, "as required"
    # An object of a unit outside sdk/ may be missing; so may that of another image.
    gone = root / "objects-gone"
    gone.mkdir()
    for path in folder.iterdir():
        if path.name not in ("unit-q.o", "unit-z.o"):
            (gone / path.name).write_bytes(path.read_bytes())
    yield "unread-objects-may-be-missing", quiet(tool(*with_arg(build=gone))), 1, "named two ways: 2"
    # A file that is no ELF.
    junk = root / "objects-junk"
    junk.mkdir()
    for path in folder.iterdir():
        (junk / path.name).write_bytes(b"not an elf file at all" * 10 if path.name == "unit-d.o" else path.read_bytes())
    proc = quiet(tool(*with_arg(build=junk)))
    yield "object-no-elf", verdict(proc.returncode == 1 and f"{junk / 'unit-d.o'}:" in proc.stdout and len(proc.stdout.splitlines()) == 1, f"{proc.returncode}\n{proc.stdout}{proc.stderr}"), 0, "as required"
    empty = root / "objects-empty-file"
    empty.mkdir()
    for path in folder.iterdir():
        (empty / path.name).write_bytes(b"" if path.name == "unit-e.o" else path.read_bytes())
    proc = quiet(tool(*with_arg(build=empty)))
    yield "object-empty", verdict(proc.returncode == 1 and f"{empty / 'unit-e.o'}:" in proc.stdout, f"{proc.returncode}\n{proc.stdout}{proc.stderr}"), 0, "as required"
    yield "build-folder-missing", quiet(tool(*with_arg(build=root / "nothing"))), 1, "unit-a.o"

    # The symbol file.
    def bad_symbols(label, text, message):
        path = root / f"bad-{label}.ld"
        path.write_text(text)
        proc = quiet(tool(*with_arg(symbols=path)))
        yield f"symbols-{label}", verdict(proc.returncode == 1 and message in proc.stdout and len(proc.stdout.splitlines()) == 1, f"{proc.returncode}\n{proc.stdout}{proc.stderr}"), 0, "as required"

    yield from bad_symbols("unsupported", "good = 0x80010000;\nPROVIDE(first = 0x80010010);\nPROVIDE(second = 0x80010020);\n", "unsupported statement 'PROVIDE(first = 0x80010010)'")
    yield from bad_symbols("no-semicolon-value", "good = foo;\n", "unsupported statement")
    yield from bad_symbols("octal-digit", "bad = 09;\n", "'bad' is assigned '09', which is no integer as the linker reads it")
    yield from bad_symbols("octal-eight", "bad = 0128;\n", "'bad' is assigned '0128'")
    yield from bad_symbols("empty-hex", "bad = 0x;\n", "unsupported statement")
    yield "symbols-missing", quiet(tool(*with_arg(symbols=root / "nothing.ld"))), 1, "No such file or directory"
    # Values are read as the linker reads integers.
    spelled = root / "spelled.ld"
    spelled.write_text(
        f"sym_ref = {a['L_ref']};\nref_ph = 0{a['L_ph1']:o};\ntwo_a = 0X{a['L_two']:X};\ntwo_b = {a['L_two']:#x};\n"
        f"alt_d = {a['L_ownd']:#x};\ndup = {a['L_dup2']:#x};\n"
    )
    proc = quiet(tool(*with_arg(symbols=spelled)))
    yield "symbol-integer-forms", verdict(
        proc.returncode == 1 and all(f"{a[n]:#x} " in proc.stdout for n in ("L_ref", "L_ph1", "L_two")) and "sym_ref, units that call it 2, form its address 3, hold it in data 1" in proc.stdout,
        proc.stdout,
    ), 0, "as required"

    # The range.
    def refused(label, start, finish):
        extra = [] if start is None else ["--start", f"{start:#x}"]
        proc = quiet(tool(*with_arg(end=f"{finish:#x}"), *extra))
        shown = START if start is None else start
        yield label, proc, 1, f"the range to sweep, {shown:#x} to {finish:#x}, is not a range of words inside the image"

    yield from refused("range-start-zero", 0, end)
    yield from refused("range-end-zero", None, 0)
    yield from refused("range-start-zero-and-end-zero", 0, 0)
    yield from refused("range-end-past-image", None, end + 4)
    yield from refused("range-start-before-image", START - 4, end)
    yield from refused("range-start-unaligned", START + 2, end)
    yield from refused("range-end-unaligned", None, end - 2)
    yield from refused("range-empty", START + 8, START + 8)
    yield from refused("range-backwards", START + 12, START + 8)
    yield from refused("range-start-at-image-end", end, end + 4)

    # The configuration and the executable.
    def bad_config(label, text):
        path = root / f"bad-{label}.toml"
        path.write_text(text)
        yield f"config-{label}", quiet(tool(*with_arg(config=path))), 1, "(at line" if label == "broken" else "has no integer address"

    yield from bad_config("broken", "[[unit\n")
    yield from bad_config("no-address", '[[unit]]\nname = "a"\nsource = "sdk/a.c"\nfunctions = [{ name = "f" }]\n')
    yield "config-missing", quiet(tool(*with_arg(config=root / "nothing.toml"))), 1, "No such file or directory"
    junk_exe = root / "junk.exe"
    junk_exe.write_bytes(b"not an executable" * 200)
    yield "not-an-executable", quiet(tool(junk_exe, *args[1:])), 1, "not a PS-X executable"
    short_exe = root / "short.exe"
    short_exe.write_bytes(b"PS-X EXE")
    yield "short-executable", quiet(tool(short_exe, *args[1:])), 1, "not a PS-X executable"
    yield "executable-missing", quiet(tool(root / "nothing.exe", *args[1:])), 1, "No such file or directory"

    # Required arguments: each one missing ends the run with the status of a usage error.
    for drop, label in ((0, "executable"), (1, "config"), (3, "build"), (5, "symbols"), (7, "library"), (9, "end")):
        trimmed = args[:drop] + args[drop + (1 if drop == 0 else 2) :]
        proc = tool(*trimmed)
        yield f"missing-{label}", verdict(proc.returncode == 2 and "required" in proc.stderr, f"{proc.returncode} {proc.stderr}"), 0, "as required"


def cases(root: Path):
    for name, build_cases in (("writer", writer_cases), ("main", main_cases), ("region", region_cases), ("trees", trees)):
        sub = root / name
        sub.mkdir()
        yield from build_cases(sub)


def main() -> int:
    failed = 0
    count = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name, proc, want_status, want_text in cases(Path(tmp)):
            output = proc.stdout + proc.stderr
            ok = proc.returncode == want_status and want_text in output
            count += 1
            print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {proc.returncode}, wanted {want_status} with {want_text!r}")
            if not ok:
                print(output)
                failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
