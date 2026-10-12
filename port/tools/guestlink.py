#!/usr/bin/env python3
"""The guest image of the macOS build: the game's C as one 32-bit-pointer AArch64 program at the PS1's addresses.

Used by `hostbuild.py --guest` (see `port/mac/README.md`). This module holds
the pieces that need no compiler and no disc, so that their tests are fast
(`test_guestlink.py`): the compiler flags, the linker script, the assembly
of the stubs for routines that the host serves, the check of the linked
ELF file's header and program headers, and the reading of `nm`.

The image is linked with `aarch64-elf-ld -m aarch64elf32` from objects that
`aarch64-elf-gcc -mabi=ilp32` made, so pointers and `long` have four bytes
as on the PS1. Its code, read-only data, data and bss lie from GUEST_BASE up
to GUEST_LIMIT, above the PS1's 2 MB of RAM and within the reach of a branch
from any PS1 address (a branch has 128 MB). Every PS1 name of the game is an
absolute symbol (`names.ld`) as in the Windows program.
"""
from __future__ import annotations

import re
import struct

GUEST_BASE = 0x80200000
GUEST_LIMIT = 0x88000000  # the forward reach (128 MB) of a branch at 0x80000000
TARGET_FLAGS = ["-mabi=ilp32", "-Wno-deprecated"]
EMULATION = "aarch64elf32"
DEFAULT_CC = "aarch64-elf-gcc"
DEFAULT_LD = "aarch64-elf-ld"
EM_AARCH64 = 183
ET_EXEC = 2
PT_LOAD = 1
PF_X, PF_W = 1, 2
HVC_BASE = 0xD4000002  # `hvc #0`; the 16-bit immediate sits at bits 5 to 20
RET = 0xD65F03C0


def render_script(base: int = GUEST_BASE) -> str:
    """The linker script: code, read-only data, data, bss, one after the other from `base`, in three segments (code is read and
    execute, read-only data is read, data and bss are read and write), each on a 16 KB boundary, the size of a page of the virtual
    machine's memory. The compiler's notes and unwind tables are dropped."""
    return (
        "PHDRS {\n"
        "    text PT_LOAD FLAGS(5);\n"
        "    rodata PT_LOAD FLAGS(4);\n"
        "    data PT_LOAD FLAGS(6);\n"
        "}\n"
        "SECTIONS {\n"
        f"    . = {base:#x};\n"
        "    .text : { *(.text .text.*) . = ALIGN(16); } :text\n"
        "    . = ALIGN(0x4000);\n"
        "    .rodata : { *(.rodata .rodata.*) . = ALIGN(16); } :rodata\n"
        "    . = ALIGN(0x4000);\n"
        "    .data : { *(.data .data.*) . = ALIGN(16); } :data\n"
        "    .bss : { *(.bss .bss.* COMMON) . = ALIGN(16); } :data\n"
        "    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) *(.ARM.attributes) }\n"
        "}\n"
    )


def hvc_word(number: int) -> int:
    """The instruction `hvc #number`."""
    if not 0 <= number <= 0xFFFF:
        raise ValueError(f"hvc immediate {number} is outside 0..65535")
    return HVC_BASE | (number << 5)


def hvc_number(word: int) -> int | None:
    """The immediate of `word` if it is an `hvc`, else None."""
    return (word >> 5) & 0xFFFF if (word & 0xFFE0001F) == HVC_BASE else None


def render_host_stubs(routines: list[str]) -> str:
    """Assembly of one stub per routine that the host serves: `NAME: hvc #K; ret`, K from 1 in the order of the sorted names.
    The host finds each routine again from the symbol and the `hvc` it reads there, so no side table exists. No names, no text."""
    names = sorted(set(routines))
    if not names:
        return ""
    out = ["\t.text\n"]
    for k, name in enumerate(names, 1):
        out.append(f"\t.globl\t{name}\n\t.type\t{name}, %function\n\t.p2align\t3\n{name}:\n\thvc\t#{k}\n\tret\n\t.size\t{name}, . - {name}\n")
    return "".join(out)


def elf_misses(data: bytes, base: int = GUEST_BASE, limit: int = GUEST_LIMIT) -> list[str]:
    """The misses of a linked image's headers: a 32-bit little-endian AArch64 executable whose every loaded segment lies
    in [base, limit), whose first segment begins at `base` and is code, and in which no segment is both writable and executable.
    Empty when none."""
    if len(data) < 52 or data[:4] != b"\x7fELF":
        return ["not an ELF file"]
    if data[4] != 1:
        return [f"not a 32-bit ELF file (class {data[4]})"]
    if data[5] != 1:
        return ["not little-endian"]
    etype, machine, _, entry, phoff, _, _, _, phentsize, phnum = struct.unpack_from("<HHIIIIIHHH", data, 16)
    misses = []
    if machine != EM_AARCH64:
        misses.append(f"machine {machine}, wanted {EM_AARCH64} (AArch64)")
    if etype != ET_EXEC:
        misses.append(f"type {etype}, wanted {ET_EXEC} (executable)")
    if phentsize != 32 or phnum == 0 or phoff + phnum * 32 > len(data):
        return misses + ["program headers missing or cut"]
    loads = []
    for i in range(phnum):
        ptype, offset, vaddr, _, filesz, memsz, flags, _ = struct.unpack_from("<IIIIIIII", data, phoff + 32 * i)
        if ptype != PT_LOAD or memsz == 0:
            continue
        loads.append((vaddr, memsz, flags))
        if vaddr < base or vaddr + memsz > limit:
            misses.append(f"segment {vaddr:#x}..{vaddr + memsz:#x} is outside {base:#x}..{limit:#x}")
        if flags & PF_X and flags & PF_W:
            misses.append(f"segment at {vaddr:#x} is writable and executable")
        if offset + filesz > len(data):
            misses.append(f"segment at {vaddr:#x} runs past the file")
    if not loads:
        return misses + ["no loaded segment"]
    first = min(loads)
    if first[0] != base:
        misses.append(f"the first segment is at {first[0]:#x}, wanted {base:#x}")
    if not first[2] & PF_X:
        misses.append("the first segment is not code")
    return misses


NM_LINE = re.compile(r"^([0-9a-fA-F]+)\s+(\S)\s+(\S+)$")


def symbol_addresses(nm_text: str, prefix: str) -> dict[str, int]:
    """Names that start with `prefix` in the text of `nm`, as {name without the prefix: address}. A name that appears twice with two
    addresses is an error."""
    found: dict[str, int] = {}
    for line in nm_text.splitlines():
        match = NM_LINE.match(line.strip())
        if not match or not match.group(3).startswith(prefix):
            continue
        name, address = match.group(3)[len(prefix):], int(match.group(1), 16)
        if found.setdefault(name, address) != address:
            raise ValueError(f"{prefix}{name} is at two addresses")
    return found
