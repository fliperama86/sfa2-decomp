#!/usr/bin/env python3
"""Controls for the modules package (port/src/modules.c): the launch path, run for real.

    python3 test_hostmodules.py --cc CROSS_CC [--run PREFIX]

The test builds the runtime (every port/src/*.c the launch controls build, plus
modules.c) with the cross compiler CC and made-up tables of its own, then runs
the program on disc images made here from invented bytes. The images hold three
made-up archives of the real archive layout (u32 count, 32-byte entries of
u16 slot, u16 table, u32 size and a copy of the first word, data from 0x800 in
whole sectors) in a folder, and the made-up game code of this file loads them
with the real CdIntToPos / CdControlB / CdReady / CdGetSector host routines:

- module A (slot 5) and module B (slot 5, another archive) load at one address
  (0x80150000, two pages). The tables give each image one function with C on the
  first page, one without C, and B a second one with C on the second page;
- load A, call its function: the C of A runs and the `module:` line says so;
- load B over it, call the same address: the C of B runs (a new `module:` line);
  B's function on the second page runs without another line; load A again: A's;
- the data of a loaded module is readable before any call (no fault, no line);
- the function without C: the stop line names it, status 3;
- an address in a loaded page that is no function start of the image: refused with
  the line that names the sector, the file and the images at that address, status 5;
- a module no image of the tables fits (another slot): the same kind of line;
- a `like` image (its C is not built yet): the line that says so and names it, status 3;
- a fault of the kind "read" is not swallowed: the program's own crash line, status 10.

The expected lines are worked out here from the fixtures. Needs the cross compiler
and a way to start a Windows program, like test_hostlaunch.py; without them this
says so and ends with status 2.
"""

from __future__ import annotations

import argparse
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostlaunch as tl  # noqa: E402
from test_hostrun import CNF, Image  # noqa: E402

SRC, BUILD, RAM = tl.SRC, tl.BUILD, tl.RAM
BASE = RAM + 0x150000      # where A and B load
LIKE_BASE = RAM + 0x160000
OTHER_BASE = RAM + 0x170000
SIZE = 0x2000


def archive(slot: int, size: int, seed: int) -> bytes:
    """A made-up archive of one chunk of table 0: the real container layout, invented content."""
    data = bytearray(((i * seed + 1) & 0xFF) for i in range(size))
    header = bytearray(0x800)
    struct.pack_into("<I", header, 0, 1)
    struct.pack_into("<HHI", header, 0x20, slot, 0, size)
    header[0x28:0x2C] = data[0:4]
    return bytes(header + data)


def content(seed: int, size: int) -> bytes:
    return bytes(((i * seed + 1) & 0xFF) for i in range(size))


FILES = {"MODA.PAC;1": archive(5, SIZE, 3), "MODB.PAC;1": archive(5, SIZE, 5), "MODL.PAC;1": archive(7, 0x800, 7), "MODX.PAC;1": archive(9, 0x800, 11)}


def disc_image(entry_program: bytes) -> Image:
    return Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": entry_program, "PAC": dict(FILES)})


PROBE = disc_image(tl.program(tl.ENTRY_C))
FILE_SECTOR = {n.split(";")[0]: PROBE.sector["PAC/" + n] for n in FILES}   # "MODA.PAC" -> first sector

# resident game functions (with C) and the library functions the game code calls (absent, library)
G = {n: RAM + 0x100600 + 0x10 * i for i, n in enumerate(
    ["g_a", "g_ab", "g_noc", "g_nonstart", "g_other", "g_like", "g_read", "g_data"])}
LIB = {n: RAM + 0x100700 + 0x10 * i for i, n in enumerate(["CdControlB", "CdReady", "CdGetSector", "CdIntToPos", "CdInit", "CdSync", "CdControl", "CdControlF", "CdMix", "CdPosToInt"])}
FA, GA, FB2 = BASE + 0x10, BASE + 0x20, BASE + 0x1010   # C, without C, C on the second page
NONSTART = BASE + 0x1018
LIKE_F, OTHER_F = LIKE_BASE + 0x10, OTHER_BASE + 0x10

GAME = tl.PROLOGUE + f"""
extern int ps1_CdControlB(int, unsigned char *, unsigned char *);
extern int ps1_CdReady(int, unsigned char *);
extern int ps1_CdGetSector(void *, int);
extern unsigned char *ps1_CdIntToPos(int, unsigned char *);
static void load(int sector, int sectors, unsigned dest)
{{
    unsigned char pos[4], res[8];
    int i;
    ps1_CdIntToPos(sector, pos);
    ps1_CdControlB(2, pos, res);
    ps1_CdControlB(6, 0, res);
    for (i = 0; i < sectors; i++) {{
        ps1_CdReady(0, res);
        ps1_CdGetSector((void *)(size_t)(dest + 2048u * (unsigned)i), 512);
    }}
    ps1_CdControlB(9, 0, res);
}}
#define LOAD_A load({FILE_SECTOR['MODA.PAC'] + 1}, 4, 0x{BASE:08x}u)
#define LOAD_B load({FILE_SECTOR['MODB.PAC'] + 1}, 4, 0x{BASE:08x}u)
#define CALL(a) ((void (*)(void))(size_t)(a))()
void ma_f(void) {{ SAY("A ran\\n"); }}
void mb_f(void) {{ SAY("B ran\\n"); }}
void mb_g(void) {{ SAY("B second page ran\\n"); }}
void g_a(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u); }}
void g_ab(void)
{{
    LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u);
    LOAD_B; SAY("loaded B\\n"); CALL(0x{FA:08x}u); CALL(0x{FB2:08x}u);
    LOAD_A; SAY("loaded A again\\n"); CALL(0x{FA:08x}u);
}}
void g_noc(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{GA:08x}u); SAY("not reached\\n"); }}
void g_nonstart(void) {{ LOAD_A; SAY("loaded A\\n"); CALL(0x{NONSTART:08x}u); SAY("not reached\\n"); }}
void g_other(void)
{{
    load({FILE_SECTOR['MODX.PAC'] + 1}, 1, 0x{OTHER_BASE:08x}u); SAY("loaded X\\n"); CALL(0x{OTHER_F:08x}u); SAY("not reached\\n");
}}
void g_like(void)
{{
    load({FILE_SECTOR['MODL.PAC'] + 1}, 1, 0x{LIKE_BASE:08x}u); SAY("loaded L\\n"); CALL(0x{LIKE_F:08x}u); SAY("not reached\\n");
}}
void g_read(void)
{{
    volatile unsigned *p = (volatile unsigned *)0x1000;
    LOAD_A; SAY("loaded A\\n"); CALL(0x{FA:08x}u);
    SAY("about to read the mirror\\n");
    SAY("read %u\\n", *p);
}}
void g_data(void)
{{
    volatile unsigned char *p = (volatile unsigned char *)0x{BASE + 4:08x}u;
    LOAD_A; SAY("loaded A\\n");
    SAY("data %u %u\\n", p[0], p[0x1004]);
}}
"""

DOMAINS = r"""
#include "port.h"
const struct port_domain port_domains[] = { { "cd", port_cd_library } };
const unsigned port_domain_count = 1;
static const struct port_override o[] = { { 0, 0, 0 } };
const struct port_override *const port_override_sets[] = { o };
const unsigned port_override_set_count = 1;
"""


def tables_c(pin: bytes) -> str:
    out = ['#include "port_tables.h"']
    for sym in ("g_a", "g_ab", "g_noc", "g_nonstart", "g_other", "g_like", "g_read", "g_data", "ma_f", "mb_f", "mb_g", "c_main", "c_big"):
        out.append(f"extern void {sym}(void);")
    out.append('static const char *const arch_a[] = { "MODA.PAC", "MODA2.PAC", 0 };')
    out.append('static const char *const arch_b[] = { "modb.pac", 0 };')
    out.append('static const char *const arch_l[] = { "MODL.PAC", 0 };')
    out.append("const struct port_image port_images[] = {")
    out.append(f'    {{ "ma", 0x{BASE:08x}u, 0, 5, arch_a }},')
    out.append(f'    {{ "mb", 0x{BASE:08x}u, 0, 5, arch_b }},')
    out.append(f'    {{ "ml", 0x{LIKE_BASE:08x}u, "ma", 7, arch_l }},')
    out.append("};")
    out.append("const unsigned port_image_count = 3;")
    fns = [(tl.ENTRY_C, "c_main", "c_main", -1), (tl.BIG_C, "c_big", "c_big", -1)]
    fns += [(a, n, n, -1) for n, a in G.items()]
    fns += [(FA, "ma_f", "ma_f", 0), (FA, "mb_f", "mb_f", 1), (FB2, "mb_g", "mb_g", 1)]
    fns.sort(key=lambda f: (f[3], f[0]))
    out.append("const struct port_function port_functions[] = {")
    for a, name, sym, image in fns:
        out.append(f'    {{ 0x{a:08x}u, (void *){sym}, "{name}", {image} }},')
    out.append("};")
    out.append(f"const unsigned port_function_count = {len(fns)};")
    abs_ = [(a, n, -1, 1) for n, a in LIB.items()] + [(GA, "func_80150020_ma", 0, 0), (GA, "func_80150020_mb", 1, 0)]
    abs_.sort(key=lambda f: (f[2], f[0]))
    out.append("const struct port_absent port_absents[] = {")
    for a, name, image, lib in abs_:
        out.append(f'    {{ 0x{a:08x}u, "{name}", {image}, {lib} }},')
    out.append("};")
    out.append(f"const unsigned port_absent_count = {len(abs_)};")
    out.append(f"const unsigned char port_program_sha256[32] = {{ {tl.sha_bytes(pin)} }};")
    return "\n".join(out) + "\n"


class ModRig(tl.Rig):
    built_parts: dict = {}

    def runtime_objects(self):
        if not self.objects:
            main = (SRC / "main.c").read_text()
            if "port_modules_init" not in main:   # L1 calls it from main; until then a scratch copy does
                needle = '    printf("start: 0x%08x\\n", entry);'
                if needle not in main:
                    raise RuntimeError("main.c has no start line to put the call of port_modules_init before")
                main = '#include "modules.h"\n' + main.replace(needle, "    port_modules_init();\n" + needle, 1)
            scratch = self.work / "main_scratch.c"
            scratch.write_text(main)
            self.objects = [self.compile(scratch)] + [self.compile(SRC / f"{n}.c") for n in tl.RUNTIME if n != "main"]
            stubs = self.work / "stubs.c"
            stubs.write_text(tl.STUBS)
            self.objects.append(self.compile(stubs))
        return self.objects

    def program(self, pin: bytes) -> Path:
        """The runtime with the made-up tables for the program `pin` (the tables carry its SHA-256)."""
        import hashlib
        key = hashlib.sha256(pin).hexdigest()[:8]
        if key not in self.built:
            if "fixed" not in self.built_parts:
                parts = []
                for tag, text in (("domains", DOMAINS), ("mbegin", tl.MARK_BEGIN), ("game", GAME), ("mend", tl.MARK_END)):
                    path = self.work / f"mod-{tag}.c"
                    path.write_text(text)
                    parts.append(self.compile(path, ["-Wno-unused-function"] if tag == "game" else None))
                self.built_parts["fixed"] = parts
            path = self.work / f"mod-tables-{key}.c"
            path.write_text(tables_c(pin))
            objs = list(self.runtime_objects()) + self.built_parts["fixed"] + [self.compile(path)]
            names = {n: a for n, a in LIB.items()} | G | {"ma_f": FA, "mb_f": FA, "mb_g": FB2}
            defs = [f"-Wl,--defsym,_ps1_{n}=0x{a:08x}" for n, a in names.items()]
            out = self.work / f"sfa2-mod-{key}.exe"
            proc = subprocess.run([self.cc, "-o", str(out), *map(str, objs), *tl.LINK_FLAGS, *defs], capture_output=True, text=True, timeout=300)
            if proc.returncode != 0:
                raise RuntimeError("the runtime did not link:\n" + proc.stderr.strip())
            self.built[key] = out
        return self.built[key]

    def go(self, tag: str, entry: int, timeout: int = 60) -> tuple[int, list[str]]:
        data = tl.program(entry)
        exe = self.program(data)
        img = disc_image(data)
        path = self.work / f"{tag}.bin"
        img.write(path)
        proc = subprocess.run([*self.prefix, str(exe), self.native(path)], capture_output=True, text=True, timeout=timeout)
        lines = proc.stdout.replace("\r\n", "\n").splitlines()
        if f"start: 0x{entry:08x}" in lines:
            lines = lines[lines.index(f"start: 0x{entry:08x}") + 1:]
        return proc.returncode, lines


def verdict(got, want):
    return None if got == want else f"got {got!r}, wanted {want!r}"


def mline(name: str, base: int, c: int, a: int) -> str:
    return f"module: {name} at 0x{base:08x}, {c} C jumps, {a} without C"


def cases(rig: ModRig):
    yield "fixture-the-two-modules-share-an-address-and-an-slot", verdict((BASE, 5), (BASE, 5))
    yield "fixture-archives-are-sector-aligned-and-hold-their-chunk", verdict(
        (len(FILES["MODA.PAC;1"]) % 2048, len(FILES["MODA.PAC;1"]) - 0x800), (0, SIZE))

    status, lines = rig.go("a", G["g_a"])
    yield "load-a-call-its-function-the-c-of-a-runs", verdict((status, lines), (0, ["loaded A", mline("ma", BASE, 1, 1), "A ran", "stop: main returned"]))

    status, lines = rig.go("ab", G["g_ab"])
    want = ["loaded A", mline("ma", BASE, 1, 1), "A ran",
            "loaded B", mline("mb", BASE, 2, 1), "B ran", "B second page ran",
            "loaded A again", mline("ma", BASE, 1, 1), "A ran", "stop: main returned"]
    yield "load-b-over-a-the-c-of-b-runs-then-a-again-each-with-its-line-and-the-second-page-without-one", verdict((status, lines), (0, want))

    status, lines = rig.go("data", G["g_data"])
    want = ["loaded A", f"data {content(3, SIZE)[4]} {content(3, SIZE)[0x1008]}", "stop: main returned"]
    yield "the-data-of-a-loaded-module-is-readable-without-a-module-line", verdict((status, lines), (0, want))

    status, lines = rig.go("noc", G["g_noc"])
    yield "a-function-without-c-stops-with-its-name", verdict(
        (status, lines), (3, ["loaded A", mline("ma", BASE, 1, 1), f"stop: no C yet for func_80150020_ma (0x{GA:08x})"]))

    sector = FILE_SECTOR["MODA.PAC"] + 4   # the last sector written to the second page
    status, lines = rig.go("nonstart", G["g_nonstart"])
    want = ["loaded A", f"stop: call to 0x{NONSTART:08x}, which no module can be placed at: the page was written from sector {sector} of moda.pac "
            f"(image ma is there but 0x{NONSTART:08x} is no function start of it); images the tables have at that address: ma, mb"]
    yield "an-address-in-a-loaded-page-that-is-no-function-start-is-refused-with-sector-file-and-images", verdict((status, lines), (5, want))

    status, lines = rig.go("other", G["g_other"])
    want = ["loaded X", f"stop: call to 0x{OTHER_F:08x}, which no module can be placed at: the page was written from sector {FILE_SECTOR['MODX.PAC'] + 1} of modx.pac "
            "(no image of the tables has that archive, slot and address); images the tables have at that address: none"]
    yield "a-module-no-image-fits-is-refused-with-the-same-kind-of-line", verdict((status, lines), (5, want))

    status, lines = rig.go("like", G["g_like"])
    want = ["loaded L", f"stop: call to 0x{LIKE_F:08x} in image ml, which is a like placement of ma; its C is not built yet"]
    yield "a-like-image-ends-with-a-line-that-names-it-and-says-its-c-is-not-built", verdict((status, lines), (3, want))

    status, lines = rig.go("read", G["g_read"])
    ok = (status == 10 and lines[:4] == ["loaded A", mline("ma", BASE, 1, 1), "A ran", "about to read the mirror"]
          and len(lines) == 5 and lines[4].startswith("stop: crash: the game used the PS1's RAM mirror at 0x00001000 (read)"))
    yield "a-fault-of-the-kind-read-is-not-swallowed", None if ok else f"status {status}, lines {lines!r}"

    shape = [l for l in lines if l.startswith("module:")]
    import re
    yield "the-module-line-has-its-shape", None if shape and all(re.fullmatch(r"module: \w+ at 0x[0-9a-f]{8}, \d+ C jumps, \d+ without C", l) for l in shape) else f"{shape!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostmodules-", dir=BUILD))
    try:
        rig = ModRig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = 0
        try:
            for name, detail in cases(rig):
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            failed += 1
        print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
