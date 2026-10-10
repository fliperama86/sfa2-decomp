#!/usr/bin/env python3
"""Controls for programs.py, with made-up declarations and made-up program files.

    python test_programs.py

No private input and no toolchain; it runs in well under a second. Each refusal of the reader has
a case that shows it and a declaration that is accepted next to it (the first case). The expected
results are written out here, not computed by the reader. Prints one line per case, then
`all cases behaved as required` (status 0) or a count of failures (status 1).
"""

from __future__ import annotations

import hashlib
import struct
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import programs as P  # noqa: E402

HASH = "ab" * 32
FUNCTION = '{ name = "func_80012010_zz", address = 0x80012010, size = 24 }'


def declaration(name="zz", file="ZZ.EXE", disc="PAC/ZZ.EXE", sha=HASH, address="0x80012000", functions=FUNCTION,
                symbols="tbl = 0x80012080\n", extra_key="", more=""):
    return (f'[[program]]\nname = "{name}"\nfile = "{file}"\ndisc = "{disc}"\nsha256 = "{sha}"\naddress = {address}\n'
            f'{extra_key}functions = [ {functions} ]\n[program.symbols]\n{symbols}{more}')


def loaded(text: str):
    with tempfile.TemporaryDirectory() as tmp:
        path = Path(tmp) / "programs.toml"
        path.write_text(text)
        return P.load(path), Path(tmp).resolve()


def refused(text: str, *needles: str) -> str | None:
    """None when the reader refuses `text` naming every needle."""
    try:
        loaded(text)
    except P.ProgramError as exc:
        missing = [n for n in needles if n not in str(exc)]
        return f"the message {str(exc)!r} lacks {missing}" if missing else None
    return "nothing was refused"


def case_accepted_declaration():
    (program,), tmp = loaded(declaration())
    want = P.Program("zz", tmp / "ZZ.EXE", "PAC/ZZ.EXE", HASH, 0x80012000,
                     (P.ProgramFunction("func_80012010_zz", 0x80012010, 24),), {"tbl": 0x80012080})
    return None if program == want else f"got {program}"


def case_two_programs_and_no_symbols_table():
    text = (declaration(functions="").replace("[program.symbols]\ntbl = 0x80012080\n", "") +
            declaration(name="yy", functions='{ name = "func_80020000_yy", address = 0x80020000, size = 4 }', address="0x80020000"))
    programs, _ = loaded(text)
    ok = [p.name for p in programs] == ["zz", "yy"] and programs[0].functions == () and programs[0].symbols == {} and programs[1].symbols == {"tbl": 0x80012080}
    return None if ok else f"got {programs}"


def case_empty_declaration_has_no_programs():
    programs, _ = loaded("")
    return None if programs == () else f"got {programs}"


def case_program_name_used_twice():
    return refused(declaration() + declaration(), "program zz is declared twice")


def case_function_name_used_twice():
    return refused(declaration(functions=FUNCTION + ", " + FUNCTION), "function func_80012010_zz is declared twice")


def case_function_below_the_body():
    return refused(declaration(functions='{ name = "func_80011ff0_zz", address = 0x80011ff0, size = 8 }'), "func_80011ff0_zz", "outside the body")


def case_function_past_ram():
    return refused(declaration(functions='{ name = "func_801ffffc_zz", address = 0x801ffffc, size = 8 }'), "func_801ffffc_zz", "outside the body")


def case_function_just_inside_ram_is_accepted():
    loaded(declaration(functions='{ name = "func_801ffff8_zz", address = 0x801ffff8, size = 8 }'))
    return None


def case_overlap_of_two_functions():
    two = FUNCTION + ', { name = "func_80012020_zz", address = 0x80012020, size = 8 }'
    return refused(declaration(functions=two), "func_80012010_zz and func_80012020_zz overlap")


def case_touching_functions_are_accepted():
    two = FUNCTION + ', { name = "func_80012028_zz", address = 0x80012028, size = 8 }'
    loaded(declaration(functions=two))
    return None


def case_name_not_of_the_form():
    for bad in ("func_80012010", "func_80012010_yy", "func_80012010_ZZ", "func_8001201_zz", "func_80012010A_zz", "func_8001201G_zz", "fn_80012010_zz"):
        why = refused(declaration(functions='{ name = "%s", address = 0x80012010, size = 24 }' % bad), "is not func_<8 hex digits>_zz")
        if why:
            return f"{bad}: {why}"
    return refused(declaration(functions='{ name = "func_80012010_zz", address = 0x80012014, size = 24 }'), "is not the one in the name")


def case_function_integers_and_alignment():
    for bad in ('address = 0x80012010, size = 0', 'address = 0x80012010, size = 6', 'address = 0x80012010, size = "24"'):
        text = declaration(functions='{ name = "func_80012010_zz", %s }' % bad)
        why = refused(text, "func_80012010_zz")
        if why:
            return f"{bad}: {why}"
    return refused(declaration(functions='{ name = "func_80012012_zz", address = 0x80012012, size = 8 }'), "word aligned")


def case_program_name_form():
    for bad in ("Zz", "z_z", "", "z z", "zz-"):
        why = refused(declaration(name=bad), "lower case letters and digits")
        if why:
            return f"{bad!r}: {why}"
    loaded(declaration(name="z9").replace("func_80012010_zz", "func_80012010_z9"))
    return None


def case_hash_of_the_wrong_form():
    for bad in ("ab" * 31 + "a", "AB" * 32, "ab" * 33, "xy" * 32, ""):
        why = refused(declaration(sha=bad), "sha256 is 64 lower case hex digits")
        if why:
            return f"{bad!r}: {why}"
    return None


def case_unknown_keys():
    why = refused(declaration(extra_key='colour = "red"\n'), "unknown key colour")
    if why:
        return why
    why = refused(declaration(functions='{ name = "func_80012010_zz", address = 0x80012010, size = 24, note = "x" }'), "unknown key note")
    if why:
        return why
    return refused("title = 1\n" + declaration(), "unknown top-level key title")


def case_missing_keys():
    return refused(declaration().replace('disc = "PAC/ZZ.EXE"\n', ""), "missing key disc") or \
        refused(declaration().replace("functions = [ " + FUNCTION + " ]\n", ""), "missing key functions") or \
        refused(declaration(functions='{ name = "func_80012010_zz", size = 24 }'), "missing key address")


def case_file_and_disc_forms():
    return (refused(declaration(file="/abs/ZZ.EXE"), "file is a path relative to the toml")
            or refused(declaration(file=""), "file is a path relative to the toml")
            or refused(declaration(disc="/PAC/ZZ.EXE"), "disc is a path of the disc")
            or refused(declaration(disc="PAC/../ZZ.EXE"), "disc is a path of the disc")
            or refused(declaration(disc="PAC\\\\ZZ.EXE"), "disc is a path of the disc"))


def case_address_forms():
    return (refused(declaration(address="0x80012002"), "word aligned address in RAM")
            or refused(declaration(address="0x80200000"), "word aligned address in RAM")
            or refused(declaration(address="0x00012000"), "word aligned address in RAM")
            or refused(declaration(address='"0x80012000"'), "word aligned address in RAM")
            or refused(declaration(address="true"), "word aligned address in RAM"))


def case_symbols():
    return (refused(declaration(symbols="1bad = 0x80012080\n"), "symbol '1bad' is not a C name")
            or refused(declaration(symbols="tbl = 5\n"), "symbol tbl is not an address in RAM")
            or refused(declaration(symbols='tbl = "x"\n'), "symbol tbl is not an address in RAM")
            or refused(declaration(symbols="func_80012010_zz = 0x80012080\n"), "symbol func_80012010_zz is also a function")
            or refused(declaration(symbols="").replace("[program.symbols]", "symbols = 3"), "symbols is a table"))


def case_every_finding_is_named():
    text = declaration(sha="nope", address="0x80012000").replace("PAC/ZZ.EXE", "/PAC/ZZ.EXE") + declaration(name="yy", functions="")
    return refused(text, "disc is a path of the disc", "sha256 is 64 lower case hex digits")


def case_not_toml_and_not_readable():
    return (refused("this is = = not toml", "not valid TOML")
            or (None if _unreadable() else "an unreadable file was accepted"))


def _unreadable() -> bool:
    try:
        P.load(HERE / "no-such-file.toml")
    except P.ProgramError as exc:
        return "no-such-file.toml cannot be read" in str(exc)
    return False


def case_not_an_array_of_tables():
    return refused('program = "zz"\n', "program is an array of tables")


def case_the_declaration_of_the_tree_is_read():
    programs = P.load()
    names = [p.name for p in programs]
    return None if "logo" in names else f"names {names}"


def body_file(tmp, load=0x80012000, size=0x100, magic=b"PS-X EXE", body=b"\xBB" * 0x100, sha=None, name="ZZ.EXE", functions=FUNCTION):
    header = (magic + bytes(16 - len(magic)) + bytes(8) + struct.pack("<II", load, size)).ljust(2048, b"\0")
    data = header + body
    (Path(tmp) / name).write_bytes(data)
    (Path(tmp) / "programs.toml").write_text(declaration(sha=sha or hashlib.sha256(data).hexdigest(), functions=functions))
    return P.load(Path(tmp) / "programs.toml")[0]


def body_refused(needles, **kinds):
    with tempfile.TemporaryDirectory() as tmp:
        try:
            P.read_body(body_file(tmp, **kinds))
        except P.ProgramError as exc:
            missing = [n for n in needles if n not in str(exc)]
            return f"the message {str(exc)!r} lacks {missing}" if missing else None
    return "nothing was refused"


def case_body_of_a_good_file():
    with tempfile.TemporaryDirectory() as tmp:
        got = P.read_body(body_file(tmp))
    return None if got == b"\xBB" * 0x100 else f"got {len(got)} bytes"


def case_body_wrong_hash():
    return body_refused(["sha256"], sha="cd" * 32)


def case_body_header_checks():
    return (body_refused(["PS-X EXE header"], magic=b"PS-X EXF")
            or body_refused(["0x80013000", "0x80012000"], load=0x80013000)
            or body_refused(["512 bytes", "256"], size=0x200)
            or body_refused(["PS-X EXE header"], body=b"", size=0))


def case_body_missing_file():
    with tempfile.TemporaryDirectory() as tmp:
        program = body_file(tmp)
        (Path(tmp) / "ZZ.EXE").unlink()
        try:
            P.read_body(program)
        except P.ProgramError as exc:
            return None if "ZZ.EXE" in str(exc) and "discfile.py" in str(exc) else f"message {exc}"
    return "nothing was refused"


def case_body_function_past_its_end():
    far = '{ name = "func_800120f8_zz", address = 0x800120f8, size = 16 }'
    ends = '{ name = "func_800120f8_zz", address = 0x800120f8, size = 8 }'
    four = '{ name = "func_800120f8_zz", address = 0x800120f8, size = 12 }'
    with tempfile.TemporaryDirectory() as tmp:
        P.read_body(body_file(tmp, functions=ends))  # ends exactly at the body's end: accepted
    return body_refused(["func_800120f8_zz", "past the body"], functions=far) or \
        body_refused(["func_800120f8_zz", "past the body"], functions=four)  # four bytes past


def case_program_of():
    (program,), _ = loaded(declaration())
    found = [P.program_of((program,), n) for n in ("func_80012010_zz", "func_80012010_yy", "func_80012010", "x")]
    return None if found == [program, None, None, None] else f"got {found}"


CASES = [(name[5:].replace("_", "-"), fn) for name, fn in sorted(globals().items()) if name.startswith("case_")]


def main() -> int:
    failed = 0
    for name, case in CASES:
        try:
            why = case()
        except Exception as exc:  # a case that raises is a case that fails
            why = f"raised {type(exc).__name__}: {exc}"
        if why:
            failed += 1
            print(f"FAIL {name}: {why}")
        else:
            print(f"ok {name}")
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
