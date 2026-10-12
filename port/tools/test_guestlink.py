#!/usr/bin/env python3
"""Controls for guestlink.py and for the guest tables of hostbuild.py, on made-up inputs. No compiler, no linker, no disc.

    python3 test_guestlink.py

Cost: under a second. Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required`
or the number that did not. The expected values are written out here by hand (the instruction words are those of the
Arm architecture's `hvc #imm`: 0xd4000002 for #0, one more bit for each step of 32).
"""

from __future__ import annotations

import struct
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
sys.path.insert(0, str(TOOLS.parent.parent / "ps1" / "tools"))
sys.path.insert(0, str(TOOLS.parent.parent / "ps1" / "src" / "programs"))

import guestlink as G  # noqa: E402
import hostbuild as H  # noqa: E402


def elf(machine=183, klass=1, data=1, etype=2, loads=((0x80200000, 0x100, 5),), file_size=0x2000) -> bytes:
    """A made-up ELF header with program headers; each load is (vaddr, memsz, flags), its bytes at offset 0x1000."""
    head = bytearray(b"\x7fELF" + bytes([klass, data, 1]) + bytes(9))
    head += struct.pack("<HHIIIIIHHHHHH", etype, machine, 1, 0x80200000, 52, 0, 0, 52, 32, len(loads), 0, 0, 0)
    for vaddr, memsz, flags in loads:
        head += struct.pack("<IIIIIIII", 1, 0x1000, vaddr, vaddr, min(memsz, 0x100), memsz, flags, 0x1000)
    return bytes(head).ljust(file_size, b"\0")


CASES = []


def case(fn):
    CASES.append(fn)
    return fn


@case
def hvc_words_are_the_architectures():
    assert G.hvc_word(0) == 0xD4000002, hex(G.hvc_word(0))
    assert G.hvc_word(1) == 0xD4000022, hex(G.hvc_word(1))
    assert G.hvc_word(0xFFFF) == 0xD41FFFE2, hex(G.hvc_word(0xFFFF))
    assert G.hvc_number(0xD4000022) == 1 and G.hvc_number(0xD41FFFE2) == 0xFFFF and G.hvc_number(0xD65F03C0) is None
    for bad in (-1, 0x10000):
        try:
            G.hvc_word(bad)
        except ValueError:
            continue
        raise AssertionError(f"hvc #{bad} was accepted")


@case
def the_stubs_are_sorted_and_numbered_from_one():
    text = G.render_host_stubs(["port_b", "port_a", "port_b"])
    assert text.index("port_a:") < text.index("port_b:"), text
    assert text.count("hvc\t#1") == 1 and text.count("hvc\t#2") == 1 and "hvc\t#3" not in text, text
    assert text.count("ret") == 2, text
    assert G.render_host_stubs([]) == ""


@case
def the_script_has_three_segments_at_the_base():
    text = G.render_script(0x80300000)
    assert ". = 0x80300000;" in text
    for flags in ("FLAGS(5)", "FLAGS(4)", "FLAGS(6)"):
        assert flags in text, flags
    assert text.index(".text") < text.index(".rodata") < text.index(".data") < text.index(".bss")


@case
def a_good_image_has_no_miss():
    assert G.elf_misses(elf()) == []
    assert G.elf_misses(elf(loads=((0x80200000, 0x100, 5), (0x80204000, 0x20, 4), (0x80208000, 0x40, 6)))) == []


@case
def a_bad_header_is_a_miss():
    assert G.elf_misses(b"MZ" + bytes(100)) == ["not an ELF file"]
    assert G.elf_misses(elf(klass=2)) == ["not a 32-bit ELF file (class 2)"]
    assert G.elf_misses(elf(data=2)) == ["not little-endian"]
    assert any("machine 3" in m for m in G.elf_misses(elf(machine=3)))
    assert any("type 3" in m for m in G.elf_misses(elf(etype=3)))
    assert G.elf_misses(elf(loads=()))[-1] == "program headers missing or cut"


@case
def a_bad_segment_is_a_miss():
    assert any("outside" in m for m in G.elf_misses(elf(loads=((0x80100000, 0x100, 5),))))
    assert any("outside" in m for m in G.elf_misses(elf(loads=((0x88000000 - 0x80, 0x100, 5),))))
    assert any("writable and executable" in m for m in G.elf_misses(elf(loads=((0x80200000, 0x100, 7),))))
    assert any("first segment is at 0x80200010" in m for m in G.elf_misses(elf(loads=((0x80200010, 0x100, 5),))))
    assert any("not code" in m for m in G.elf_misses(elf(loads=((0x80200000, 0x100, 4),))))
    assert any("runs past the file" in m for m in G.elf_misses(elf(file_size=0x1010)))


@case
def nm_text_is_read_by_prefix():
    nm = "80200000 T impl_a\n80200010 T impl_b\n80100000 A ps1_a\n         U ext\n"
    assert G.symbol_addresses(nm, "impl_") == {"a": 0x80200000, "b": 0x80200010}
    try:
        G.symbol_addresses(nm + "80200020 T impl_a\n", "impl_")
    except ValueError:
        return
    raise AssertionError("a name at two addresses was accepted")


@case
def guest_tables_write_addresses_and_declare_nothing():
    images = [{"name": "resident", "address": 0x80010000, "slot": 0, "sha256": "00" * 32}]
    functions = [(-1, 0x80010100, "impl_func_80010100", "func_80010100"), (-1, 0x80010200, "impl_main", "main")]
    absents = [(-1, 0x80157d9c, "DrawSync", 1)]
    text = H.render_tables(images, functions, absents, "11" * 32, impl_at={"impl_func_80010100": 0x80200040, "impl_main": 0x80200080})
    assert "extern" not in text, text
    assert "{ 0x80010100u, (void *)0x80200040u, \"func_80010100\", -1, 0 }" in text, text
    assert "{ 0x80010200u, (void *)0x80200080u, \"main\", -1, 0 }" in text, text
    windows = H.render_tables(images, functions, absents, "11" * 32)
    assert "extern void impl_main(void);" in windows and "(void *)impl_main" in windows, windows


@case
def guest_tables_hold_the_programs_main_calls_and_bound_routines():
    call = H.Call("port_stream_bind", "data_8002edb8_logo")
    row = H.ProgramRow("logo", "logo.exe", "00" * 32, "func_800106fc_logo", [call], [H.Call("port_stream_bind")])
    text = H.render_tables([], [], [], "11" * 32, programs=[row], impl_at={"impl_func_800106fc_logo": 0x80250000}, bound={"DecDCTin": "port_DecDCTin", "DecDCTout": "port_DecDCTout"},
                           data_at={"data_8002edb8_logo": 0x8002edb8})
    assert "(void *)0x80250000u" in text, text
    assert "extern unsigned char ps1_" not in text and "impl_func_800106fc_logo" not in text.replace("0x80250000u", ""), text
    assert "port_stream_bind(0x8002edb8u);" in text and "port_stream_bind(0);" in text, text
    assert '{ "port_DecDCTin", (void *)port_DecDCTin },' in text and '{ "port_DecDCTout", (void *)port_DecDCTout },' in text and "port_bound_count = 2;" in text, text
    empty = H.render_tables([], [], [], "11" * 32, impl_at={})
    assert "port_bound_count = 0;" in empty and "{ 0, 0 }," in empty, empty
    windows = H.render_tables([], [], [], "11" * 32, programs=[row])
    assert "extern unsigned char ps1_data_8002edb8_logo[];" in windows and "port_bounds" not in windows and "impl_func_800106fc_logo" in windows, windows


@case
def host_stubs_can_start_at_another_number():
    text = G.render_host_stubs(["port_b", "port_a"], G.BOUND_BASE)
    assert "hvc\t#%d" % G.BOUND_BASE in text and "hvc\t#%d" % (G.BOUND_BASE + 1) in text, text


def main() -> int:
    failed = 0
    for fn in CASES:
        name = fn.__name__.replace("_", "-")
        try:
            fn()
        except Exception as err:
            failed += 1
            print(f"FAIL {name}: {type(err).__name__}: {err}")
        else:
            print(f"ok   {name}")
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {len(CASES)} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
