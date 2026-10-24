#!/usr/bin/env python3
"""Controls for the programs of the disc that are built into the PC program (port/src/programs.c, programs.h) and for
Exec's use of them (port_h_Exec in port/src/overrides.c).

    python3 test_hostprograms.py --cc CROSS_CC [--src DIR] [--only SUBSTRING]

The test compiles programs.c, overrides.c, disc.c and sha256.c with a driver of its own, which maps the game's RAM at its own
address, has a made-up table port_programs (programs whose setup, main and teardown print a line each, and rows with a null setup
and teardown) and stands in for what these files take from the rest of the runtime: port_halt (prints `stop: ...` and ends the
driver with the status it was given; each case that stops runs in a driver of its own), port_trace_line (prints `trace: ...`),
port_cd_source_at (the sector the test says; the disc layer's own record of where a word came from is tested in test_hostcd.py),
port_disc_handle and port_skip_programs, port_callbacks_program_start (prints `callbacks fresh`) and port_press_report (prints
`decoder report NAME`). The disc is a made-up ISO 9660 image (test_hostrun.py's Image) of made-up program files in
the root and in a folder; the driver is told, by script lines, to do what the game does before Exec: the header's sector to
0x801e0000 and the body at t_addr, taken from the file's bytes (a case may then change bytes of the RAM, or of the disc's file).
NOTHING IS COMPARED WITH THE CONSOLE: the contract is the text of programs.h, and the cases check that the code does what it says.

Groups: the run (setup, main, teardown in order and the result 1, the right one of two programs, rows without setup and teardown,
the trace lines, the fresh callbacks before and the decoder's report after), the name (case, version suffix, folder, near misses that name no row), the option --skip-programs, a file that
is not in the table, a changed byte of the disc's file (header sector, body, last byte), a changed header in RAM (each field), a
changed body in RAM (first and last byte), and the guards (t_size beyond the file, a span that leaves the RAM, a header outside
the RAM, and the edges of each that are still valid).

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import hashlib
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostrun as HR  # noqa: E402  (the made-up ISO image)

SRC = HERE.parent / "src"
LIMIT = 60
LINK_FLAGS = ["-static", "-Wl,--large-address-aware", "-Wl,--disable-dynamicbase"]
RAM, RAM_END = 0x80000000, 0x80200000
EXIT_PROGRAM = 13
HDR = 0x801e0000   # where the game's read puts the header's sector; Exec's argument is HDR + 0x10

# The made-up programs. file: the row's file field; where: the path on the disc; t_addr and t_size are the header's fields;
# body: the bytes the file holds after the first 0x800 (default t_size); setup/teardown: whether the row has them.
PROGRAMS = {
    "alpha": dict(file="alpha.exe", where="PAC/ALPHA.EXE;1", t_addr=0x80010000, t_size=0x1000, setup=True, teardown=True),
    "beta": dict(file="beta.exe", where="BETA.EXE;1", t_addr=0x80020000, t_size=0x900, setup=True, teardown=True),
    "gamma": dict(file="GAMMA.ZXE", where="GAMMA.ZXE;1", t_addr=0x80030000, t_size=0x800, setup=False, teardown=False),
    "delta": dict(file="delta.exe", where="DELTA.EXE;1", t_addr=0x80040000, t_size=0x400, body=0x100, setup=True, teardown=True),
    "theta": dict(file="theta.exe", where="THETA.EXE;1", t_addr=0x80050000, t_size=0xffffffff, body=0x10, setup=True, teardown=True),
    "eps": dict(file="eps.exe", where="EPS.EXE;1", t_addr=0x801ff800, t_size=0x1000, setup=True, teardown=True, fits=False),
    "zeta": dict(file="zeta.exe", where="ZETA.EXE;1", t_addr=0x7ffff000, t_size=0x1000, setup=True, teardown=True, fits=False),
    "iota": dict(file="iota.exe", where="IOTA.EXE;1", t_addr=0xfffff000, t_size=0x2000, setup=True, teardown=True, fits=False),
    "kappa": dict(file="kappa.exe", where="KAPPA.EXE;1", t_addr=0x801ff000, t_size=0x1000, setup=True, teardown=True),
    "short1": dict(file="short1.exe", where="SHORT1.EXE;1", t_addr=0x80070000, t_size=0x400, body=0x3ff, setup=True, teardown=True),
    "over1": dict(file="over1.exe", where="OVER1.EXE;1", t_addr=0x801ff001, t_size=0x1000, setup=True, teardown=True, fits=False),
    "sig0": dict(file="sig0.exe", where="SIG0.EXE;1", t_addr=0x80080000, t_size=0x800, setup=True, teardown=True, flip=0),
    "sig16": dict(file="sig16.exe", where="SIG16.EXE;1", t_addr=0x80081000, t_size=0x800, setup=True, teardown=True, flip=16),
    "sig31": dict(file="sig31.exe", where="SIG31.EXE;1", t_addr=0x80082000, t_size=0x800, setup=True, teardown=True, flip=31),
    "tiny": dict(file="tiny.exe", where="TINY.EXE;1", t_addr=0x80090000, t_size=0, setup=True, teardown=True, raw=0x10),
    "mini": dict(file="mini.exe", where="MINI.EXE;1", t_addr=0x80091000, t_size=0, setup=True, teardown=True, raw=0x20),
    "lambda": dict(file="lambda.exe", where="LAMBDA.EXE;1", t_addr=RAM, t_size=0x800, setup=True, teardown=True),
}
OTHER = ("OTHER.EXE;1", bytes((i * 5 + 3) & 0xff for i in range(0x1000)))


def fill(seed: int, n: int) -> bytes:
    out = bytearray()
    x = seed * 2654435761 + 12345
    for _ in range(n):
        x = (x * 1103515245 + 12345) & 0x7fffffff
        out.append((x >> 16) & 0xff)
    return bytes(out)


def make_file(index: int, p: dict) -> bytes:
    if "raw" in p:   # a file shorter than the header's sector: the first `raw` bytes of a header
        head = bytearray(fill(index * 3 + 1, p["raw"]))
        if p["raw"] >= 0x20:
            head[0:8] = b"PS-X EXE"
            struct.pack_into("<IIII", head, 0x10, p["t_addr"], 0x80000000 + index, p["t_addr"], p["t_size"])
        return bytes(head)
    head = bytearray(fill(index * 3 + 1, 0x800))
    head[0:8] = b"PS-X EXE"
    struct.pack_into("<IIII", head, 0x10, (p["t_addr"] + 0x40) & 0xffffffff, 0x80000000 + index, p["t_addr"], p["t_size"])
    return bytes(head) + fill(index * 3 + 2, p.get("body", p["t_size"]) if p["t_size"] < 0x10000000 else p.get("body", 0))


FILES = {name: make_file(i, p) for i, (name, p) in enumerate(PROGRAMS.items())}
for name, p in PROGRAMS.items():
    p["data"] = FILES[name]
    p["sha"] = hashlib.sha256(FILES[name]).digest()
    p["rowsha"] = bytes(b ^ (1 if k == p.get("flip", -1) else 0) for k, b in enumerate(p["sha"]))   # the row's hash: the file's, with one byte changed
    p["fits"] = p.get("fits", True) and p["t_size"] < 0x10000000 and p.get("body", p["t_size"]) >= p["t_size"]


def driver_c() -> str:
    out = ['#include "port.h"', '#include "port_tables.h"', '#include "cd.h"', "#include <stdarg.h>", "#include <stdio.h>", "#include <stdlib.h>",
           "#include <string.h>", "#include <windows.h>", ""]
    rows = []
    for n, (name, p) in enumerate(PROGRAMS.items()):
        out.append("static const unsigned char sha_%d[32] = { %s };" % (n, ", ".join("0x%02x" % b for b in p["rowsha"])))
        out.append('static void main_%d(void) { printf("main %s\\n"); }' % (n, name))
        if p["setup"]:
            out.append('static void setup_%d(void) { printf("setup %s\\n"); }' % (n, name))
        if p["teardown"]:
            out.append('static void teardown_%d(void) { printf("teardown %s\\n"); }' % (n, name))
        rows.append('    { "%s", "%s", sha_%d, (void *)main_%d, %s, %s },' % (name, p["file"], n, n, "setup_%d" % n if p["setup"] else "0",
                                                                          "teardown_%d" % n if p["teardown"] else "0"))
    out.append("const struct port_builtin_program port_programs[] = {\n" + "\n".join(rows) + "\n};")
    out.append("const unsigned port_program_count = %d;" % len(PROGRAMS))
    out.append(r'''
const unsigned char port_program_sha256[32] = {0};
struct port_disc *port_disc_handle;
int port_skip_programs;
static int source_sector = -1;

int port_cd_source_at(unsigned address) { (void)address; return source_sector; }
static int intro_enabled;
int port_intro_program(const char *name) {
    if (!intro_enabled) return 0;
    printf("native intro %s\n", name); return 1;
}
int port_intro_available(void) { return intro_enabled; }
int port_arcade_enabled(void) { return 0; }
void port_callbacks_program_start(void) { printf("callbacks fresh\n"); }
void port_press_report(const char *program) { printf("decoder report %s\n", program); }
void port_trace_line(const char *fmt, ...)
{
    va_list ap;
    printf("trace: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}
void port_halt(int status, const char *fmt, ...)
{
    va_list ap;
    printf("stop: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    exit(status);
}

int port_program_exec(const char *disc_name, unsigned sector, unsigned size, void *header);
int port_h_Exec(void *header, int argc, char **argv);

static int hexv(int c) { return c <= '9' ? c - '0' : (c | 32) - 'a' + 10; }

int main(int argc, char **argv)
{
    static char line[70000];
    char err[PORT_ERR];
    struct port_disc d;
    void *ram = VirtualAlloc((void *)(size_t)PORT_RAM_BASE, PORT_RAM_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (ram != (void *)(size_t)PORT_RAM_BASE) { printf("no RAM\n"); return 3; }
    if (argc < 2) return 64;
    if (port_disc_open(&d, argv[1], err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
    port_disc_handle = &d;
    setvbuf(stdout, NULL, _IONBF, 0);
    while (fgets(line, sizeof line, stdin)) {
        char cmd[32], name[256];
        char *rest = line;
        int n = 0;
        if (sscanf(line, "%31s %n", cmd, &n) < 1) continue;
        rest += n;
        if (!strcmp(cmd, "src")) source_sector = atoi(rest);
        else if (!strcmp(cmd, "skip")) port_skip_programs = atoi(rest);
        else if (!strcmp(cmd, "intro")) intro_enabled = atoi(rest);
        else if (!strcmp(cmd, "poke")) {   /* ADDRESS HEXBYTES */
            unsigned a; int k = 0;
            char *h;
            if (sscanf(rest, "%x %n", &a, &k) < 1) return 64;
            for (h = rest + k; isxdigit((unsigned char)h[0]) && isxdigit((unsigned char)h[1]); h += 2) *(unsigned char *)(size_t)a++ = (unsigned char)(hexv(h[0]) * 16 + hexv(h[1]));
        }
        else if (!strcmp(cmd, "exec")) { unsigned a = (unsigned)strtoul(rest, 0, 16); printf("R %d\n", port_h_Exec((void *)(size_t)a, 1, 0)); }
        else if (!strcmp(cmd, "nodisc")) port_disc_handle = 0;
        else if (!strcmp(cmd, "direct")) {   /* NAME ADDRESS SECTOR SIZE: port_program_exec itself */
            unsigned a, s, z;
            if (sscanf(rest, "%255s %x %u %u", name, &a, &s, &z) != 4) return 64;
            printf("D %d\n", port_program_exec(name, s, z, (void *)(size_t)a));
        }
        else { printf("unknown script command %s\n", cmd); return 64; }
    }
    return 0;
}
''')
    return "\n".join(out).replace("#include <windows.h>", "#include <windows.h>\n#include <ctype.h>", 1)


class Ctx:
    tmp: Path
    exe: Path
    image: Path
    sector: dict
    size: dict


def hexs(b: bytes) -> str:
    return b.hex()


def make_image(path: Path, data: dict[str, bytes], other=OTHER) -> HR.Image:
    root: dict = {}
    for name, p in PROGRAMS.items():
        d = data[name]
        if "/" in p["where"]:
            folder, inner = p["where"].split("/")
            root.setdefault(folder, {})[inner] = d
        else:
            root[p["where"]] = d
    root[other[0]] = other[1]
    img = HR.Image(root)
    img.write(path)
    return img


def prep(name: str, header_sector: bytes | None = None, body: bool | None = None) -> list[str]:
    """The script lines that do what the game does before Exec for the program `name`, with the file's own bytes (or those of
    `header_sector` for the first 0x800 bytes), and that point the disc layer's record at the file's first sector."""
    p = PROGRAMS[name]
    data = p["data"]
    lines = [f"src {Ctx.sector[p['where']]}", f"poke {HDR:x} {hexs(header_sector if header_sector is not None else data[:0x800])}"]
    if p["t_size"] and (p["fits"] if body is None else body):
        lines.append(f"poke {p['t_addr']:x} {hexs(data[0x800:0x800 + p['t_size']])}")
    return lines


def run(script: list[str], image: Path | None = None) -> tuple[int, list[str]]:
    proc = subprocess.run([str(Ctx.exe), str(image or Ctx.image)], input=("\n".join(script) + "\n").encode(), capture_output=True, timeout=LIMIT)
    return proc.returncode, proc.stdout.decode(errors="replace").splitlines()


def exec_lines(name: str, *more: str, skip: bool = False) -> list[str]:
    return ([f"skip 1"] if skip else []) + prep(name) + list(more) + [f"exec {HDR + 0x10:x}"]


def ran(name: str, setup=True, teardown=True) -> list[str]:
    return (["callbacks fresh", f"trace: program {name}: start"] + ([f"setup {name}"] if setup else []) + [f"main {name}"] + ([f"teardown {name}"] if teardown else [])
            + [f"trace: program {name}: returned", f"decoder report {name}", "R 1"])


def expect(rc: int, lines: list[str], want_rc: int, want: list[str]):
    if rc != want_rc or lines != want:
        return f"status {rc} (wanted {want_rc}), lines {lines}, wanted {want}"


# ---- the run ----------------------------------------------------------------

def setup_main_teardown_in_order():
    rc, l = run(exec_lines("alpha"))
    return expect(rc, l, 0, ran("alpha"))


def intro_replaces_only_after_validation():
    rc, lines = run(["intro 1"] + exec_lines("alpha"))
    bad = expect(rc, lines, 0, ["callbacks fresh", "trace: program alpha: start",
        "native intro alpha", "trace: program alpha: native intro returned", "R 1"])
    if bad: return bad
    path, _ = changed_file("alpha", 0x800 + 5, "intro")
    rc, lines = run(["intro 1"] + exec_lines("alpha"), path)
    if rc != EXIT_PROGRAM or any("native intro" in line for line in lines):
        return f"intro bypassed file validation: {rc}, {lines}"


def callbacks_start_fresh_once_after_the_checks_and_before_setup():
    # the stand-in prints `callbacks fresh`: once, after the checks (so a failed check, a file not in the table and a skipped
    # file print no such line: see the cases of those below) and before setup
    rc, l = run(exec_lines("alpha"))
    if rc != 0 or l.count("callbacks fresh") != 1 or l.index("callbacks fresh") >= l.index("setup alpha"):
        return f"status {rc}, lines {l}"
    path, _ = changed_file("alpha", 0x800 + 5, "fresh")
    rc, l = run(exec_lines("alpha"), path)
    if rc != EXIT_PROGRAM or "callbacks fresh" in l:
        return f"a failed check: status {rc}, lines {l}"
    rc, l = run(["skip 1", f"src {Ctx.sector[OTHER[0]]}", f"exec {HDR + 0x10:x}"])
    if "callbacks fresh" in l:
        return f"a file not in the table: {l}"


def decoder_report_once_with_the_programs_name_after_everything_else():
    # the stand-in prints `decoder report NAME`: once, after teardown and the trace line of the return (the whole order is in
    # setup_main_teardown_in_order), with the name of the program that ran; not after a failed check, and not for a file that is
    # not in the table
    rc, l = run(exec_lines("beta"))
    reports = [x for x in l if x.startswith("decoder report")]
    if rc != 0 or reports != ["decoder report beta"] or l.index("decoder report beta") < l.index("trace: program beta: returned"):
        return f"status {rc}, lines {l}"
    path, _ = changed_file("alpha", 0x800 + 5, "report")
    rc, l = run(exec_lines("alpha"), path)
    if rc != EXIT_PROGRAM or any(x.startswith("decoder report") for x in l):
        return f"a failed check: status {rc}, lines {l}"
    rc, l = run(["skip 1", f"src {Ctx.sector[OTHER[0]]}", f"exec {HDR + 0x10:x}"])
    if any(x.startswith("decoder report") for x in l):
        return f"a file not in the table: {l}"


def the_right_one_of_two():
    rc, l = run(exec_lines("beta"))
    return expect(rc, l, 0, ran("beta"))


def the_first_row_of_two_is_not_taken_for_the_second():
    rc, l = run(exec_lines("alpha") + exec_lines("beta"))
    return expect(rc, l, 0, ran("alpha") + ran("beta"))


def a_row_without_setup_and_teardown():
    rc, l = run(exec_lines("gamma"))
    return expect(rc, l, 0, ran("gamma", False, False))


def a_second_exec_runs_again():
    rc, l = run(exec_lines("alpha") + [f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ran("alpha") + ran("alpha"))


def body_ending_at_the_rams_end_runs():
    rc, l = run(exec_lines("kappa"))
    return expect(rc, l, 0, ran("kappa"))


def body_starting_at_the_rams_start_runs():
    rc, l = run(exec_lines("lambda"))
    return expect(rc, l, 0, ran("lambda"))


def header_in_the_last_16_bytes_of_the_ram_runs():
    p = PROGRAMS["alpha"]
    top = RAM_END - 16
    rc, l = run(prep("alpha") + [f"poke {top:x} {hexs(p['data'][0x10:0x20])}", f"exec {top:x}"])
    return expect(rc, l, 0, ran("alpha"))


def a_later_sector_of_the_file_finds_and_reads_the_whole_file():
    # the disc layer's record says the header word came from the file's second sector: the file is still read from its first
    first = Ctx.sector[PROGRAMS["alpha"]["where"]]
    rc, l = run(prep("alpha")[1:] + [f"src {first + 1}", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ran("alpha"))


def header_at_the_rams_first_bytes_runs():
    p = PROGRAMS["alpha"]
    rc, l = run(prep("alpha") + [f"poke {RAM:x} {hexs(p['data'][0x10:0x20])}", f"exec {RAM:x}"])
    return expect(rc, l, 0, ran("alpha"))


def file_of_exactly_the_headers_fields_with_no_body_runs():
    rc, l = run(prep("mini") + [f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ran("mini"))


# ---- the name ---------------------------------------------------------------

def name_in_a_folder_found_through_the_listing():
    # alpha is in the folder PAC on the disc, spelled in capitals with a version suffix
    rc, l = run(exec_lines("alpha"))
    return expect(rc, l, 0, ran("alpha"))


def name_matched_without_regard_to_case():
    # the row says GAMMA.ZXE, the listing says gamma.zxe
    rc, l = run(exec_lines("gamma"))
    return expect(rc, l, 0, ran("gamma", False, False))


def direct_names(names: list[str], want_results: list[str], program: str = "alpha"):
    p = PROGRAMS[program]
    z = len(p["data"])
    script = prep(program) + [f"direct {n} {HDR + 0x10:x} {Ctx.sector[p['where']]} {z}" for n in names]
    rc, l = run(script)
    want = []
    for r in want_results:
        want += (ran(program, p["setup"], p["teardown"])[:-1] if r == "1" else []) + [f"D {r}"]
    return expect(rc, l, 0, want)


def direct_name_with_a_version_suffix():
    return direct_names(["ALPHA.EXE;1", "alpha.exe;12", "Alpha.Exe;0"], ["1"] * 3)


def direct_name_in_a_folder():
    return direct_names(["PAC/ALPHA.EXE;1", "pac\\alpha.exe", "a/b\\c/ALPHA.EXE", "/alpha.exe", "\\ALPHA.EXE;1"], ["1"] * 5)


def direct_name_in_a_mixed_case():
    return direct_names(["aLpHa.ExE"], ["1"])


def direct_name_with_the_first_and_last_letters_in_both_cases():
    # the row is GAMMA.ZXE: the edges of the letters A and Z, in the row and in the name
    return direct_names(["gamma.zxe", "GAMMA.ZXE", "Gamma.zXe;1"], ["1"] * 3, "gamma")


def direct_names_that_are_not_the_row():
    return direct_names(["xalpha.exe", "alpha.exe.bak", "alpha.ex", "alpha.exee", "lpha.exe", "alpha", "alpha.exe/", ";1", "alpha.exe\\x;1"], ["0"] * 9)


def direct_name_cut_at_the_first_semicolon_only():
    return direct_names(["alpha.exe;1;2", "alpha.exe;"], ["1", "1"])


def no_disc_open_stops():
    p = PROGRAMS["alpha"]
    rc, l = run(prep("alpha") + ["nodisc", f"direct alpha.exe {HDR + 0x10:x} {Ctx.sector[p['where']]} {len(p['data'])}"])
    return expect(rc, l, EXIT_PROGRAM, ["stop: program alpha (alpha.exe): no disc is open"])


def file_that_cannot_be_read_stops():
    rc, l = run(prep("alpha") + [f"direct alpha.exe {HDR + 0x10:x} 99999 100"])
    if rc != EXIT_PROGRAM or len(l) != 1 or not l[0].startswith("stop: program alpha (alpha.exe): the file cannot be read: disc: "):
        return f"status {rc}, lines {l}"


def no_row_does_nothing():
    # nothing of the program is touched: a name without a row with a bad hash, a header that is not the file's and a span outside the RAM
    script = [f"src {Ctx.sector[PROGRAMS['alpha']['where']]}", f"direct nothing.exe 0 {Ctx.sector[PROGRAMS['alpha']['where']]} 7"]
    rc, l = run(script)
    return expect(rc, l, 0, ["D 0"])


# ---- the option, and files that are not in the table ------------------------

def skip_option_a_built_in_program_still_runs():
    rc, l = run(exec_lines("alpha", skip=True))
    return expect(rc, l, 0, ran("alpha"))


def skip_option_off_then_on_the_same_program():
    rc, l = run(exec_lines("beta") + ["skip 1"] + [f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ran("beta") + ran("beta"))


def file_not_in_the_table_stops_with_todays_line():
    sector = Ctx.sector[OTHER[0]]
    rc, l = run([f"src {sector}", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: no C yet for the program other.exe (disc sector {sector})"])


def file_not_in_the_table_with_the_option_skips():
    sector = Ctx.sector[OTHER[0]]
    rc, l = run(["skip 1", f"src {sector}", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, [f"skipped: Exec of other.exe (disc sector {sector}): a program of its own for which no C exists yet; it returns at once", "R 1"])


def a_sector_inside_the_file_not_only_its_first_finds_it():
    sector = Ctx.sector[OTHER[0]] + 1
    rc, l = run(["skip 1", f"src {sector}", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, [f"skipped: Exec of other.exe (disc sector {sector}): a program of its own for which no C exists yet; it returns at once", "R 1"])


def a_sector_no_file_has_stops_with_todays_line():
    rc, l = run(["src 3", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, EXIT_PROGRAM, ["stop: no C yet for the program ? (disc sector 3)"])


def no_sector_known_stops_with_todays_line():
    rc, l = run(["src -1", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, EXIT_PROGRAM, ["stop: no C yet for the program ? (disc sector -1)"])


def no_sector_known_with_the_option_skips():
    rc, l = run(["skip 1", "src -1", f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ["skipped: Exec of ? (disc sector -1): a program of its own for which no C exists yet; it returns at once", "R 1"])


# ---- the file on the disc ---------------------------------------------------

def changed_file(name: str, at: int, tag: str) -> tuple[Path, bytes]:
    data = dict((n, p["data"]) for n, p in PROGRAMS.items())
    d = bytearray(data[name])
    d[at] ^= 0x01
    data[name] = bytes(d)
    path = Ctx.tmp / f"changed_{tag}.bin"
    make_image(path, data)
    return path, bytes(d)


def disc_byte_changed(name: str, at: int, tag: str):
    path, changed = changed_file(name, at, tag)
    rc, l = run(exec_lines(name), path)
    p = PROGRAMS[name]
    got, want = hashlib.sha256(changed).hexdigest(), p["sha"].hex()
    if rc != EXIT_PROGRAM or len(l) != 1 or not l[0].startswith("stop: "):
        return f"status {rc}, lines {l}"
    line = l[0]
    for piece in (f"program {name}:", got, want):
        if piece not in line:
            return f"the line lacks {piece!r}: {line}"


def disc_byte_changed_in_the_headers_sector():
    return disc_byte_changed("alpha", 0x300, "hdr")


def disc_byte_changed_in_the_body():
    return disc_byte_changed("alpha", 0x800 + 0x400, "body")


def disc_last_byte_changed():
    return disc_byte_changed("alpha", len(PROGRAMS["alpha"]["data"]) - 1, "last")


def disc_first_byte_changed():
    return disc_byte_changed("beta", 0, "first")


def disc_changed_byte_in_a_file_that_ends_inside_a_sector():
    # beta's file is 0x800 + 0x900 bytes: its last sector holds 0x100 of them
    return disc_byte_changed("beta", len(PROGRAMS["beta"]["data"]) - 1, "betalast")


def disc_changed_file_stops_before_the_header_is_compared():
    # the RAM holds a header that is not the file's; the hash line comes first
    path, changed = changed_file("alpha", 0x300, "hdr2")
    bad = bytearray(PROGRAMS["alpha"]["data"][:0x800])
    bad[0x10] ^= 0xff
    rc, l = run(prep("alpha", bytes(bad)) + [f"exec {HDR + 0x10:x}"], path)
    if rc != EXIT_PROGRAM or "SHA-256" not in "".join(l):
        return f"status {rc}, lines {l}"


def hash_line_names_the_file_the_program_and_both_hashes():
    path, changed = changed_file("alpha", 0x800 + 5, "line")
    rc, l = run(exec_lines("alpha"), path)
    p = PROGRAMS["alpha"]
    want = f"stop: program alpha: the file alpha.exe on the disc has SHA-256 {hashlib.sha256(changed).hexdigest()}, the C was written from the file with {p['sha'].hex()}"
    return expect(rc, l, EXIT_PROGRAM, [want])


def row_hash_differs_in_a_byte(name: str):
    p = PROGRAMS[name]
    rc, l = run(exec_lines(name))
    want = f"stop: program {name}: the file {name}.exe on the disc has SHA-256 {p['sha'].hex()}, the C was written from the file with {p['rowsha'].hex()}"
    return expect(rc, l, EXIT_PROGRAM, [want])


def row_hash_differs_in_the_first_byte():
    return row_hash_differs_in_a_byte("sig0")


def row_hash_differs_in_the_byte_after_the_first_16():
    return row_hash_differs_in_a_byte("sig16")


def row_hash_differs_in_the_last_byte():
    return row_hash_differs_in_a_byte("sig31")


def file_shorter_than_a_header_stops():
    rc, l = run(prep("tiny") + [f"exec {HDR + 0x10:x}"])
    return expect(rc, l, EXIT_PROGRAM, ["stop: program tiny (tiny.exe): the file has 16 bytes, it has no header"])


# ---- the header in RAM ------------------------------------------------------

FIELDS = ["pc0", "gp0", "t_addr", "t_size"]


def ram_header_changed(i: int, byte: int):
    p = PROGRAMS["alpha"]
    d = p["data"]
    at = 0x10 + 4 * i + byte
    bad = bytearray(d[:0x800])
    bad[at] ^= 0x80
    rc, l = run(prep("alpha", bytes(bad)) + [f"exec {HDR + 0x10:x}"])
    fw = struct.unpack_from("<I", d, 0x10 + 4 * i)[0]
    rw = struct.unpack_from("<I", bytes(bad), 0x10 + 4 * i)[0]
    want = f"stop: program alpha: the header in RAM differs from the file's at {FIELDS[i]}: RAM 0x{rw:08x}, file 0x{fw:08x}"
    return expect(rc, l, EXIT_PROGRAM, [want])


def ram_header_pc0_first_byte():
    return ram_header_changed(0, 0)


def ram_header_pc0_last_byte():
    return ram_header_changed(0, 3)


def ram_header_gp0():
    return ram_header_changed(1, 1)


def ram_header_t_addr():
    return ram_header_changed(2, 2)


def ram_header_t_size():
    return ram_header_changed(3, 0)


def ram_header_t_size_last_byte():
    return ram_header_changed(3, 3)


def ram_header_two_fields_names_the_first():
    p = PROGRAMS["alpha"]
    bad = bytearray(p["data"][:0x800])
    bad[0x1c] ^= 1
    bad[0x14] ^= 1
    rc, l = run(prep("alpha", bytes(bad)) + [f"exec {HDR + 0x10:x}"])
    if rc != EXIT_PROGRAM or len(l) != 1 or "at gp0:" not in l[0]:
        return f"status {rc}, lines {l}"


def ram_outside_the_headers_16_bytes_is_not_compared():
    p = PROGRAMS["alpha"]
    bad = bytearray(p["data"][:0x800])
    bad[0x0f] ^= 1
    bad[0x20] ^= 1
    bad[0x300] ^= 1
    rc, l = run(prep("alpha", bytes(bad)) + [f"exec {HDR + 0x10:x}"])
    return expect(rc, l, 0, ran("alpha"))


# ---- the body in RAM --------------------------------------------------------

def ram_body_changed(name: str, offset: int):
    p = PROGRAMS[name]
    d = p["data"]
    a = p["t_addr"] + offset
    ram = d[0x800 + offset] ^ 0x40
    rc, l = run(exec_lines(name, f"poke {a:x} {ram:02x}")[:-1] + [f"exec {HDR + 0x10:x}"])
    want = f"stop: program {name}: the body in RAM differs from the file's at 0x{a:08x}: RAM 0x{ram:02x}, file 0x{d[0x800 + offset]:02x}"
    return expect(rc, l, EXIT_PROGRAM, [want])


def ram_body_first_byte():
    return ram_body_changed("alpha", 0)


def ram_body_last_byte():
    return ram_body_changed("alpha", PROGRAMS["alpha"]["t_size"] - 1)


def ram_body_middle_byte():
    return ram_body_changed("beta", 0x123)


def ram_body_first_byte_at_the_rams_start():
    return ram_body_changed("lambda", 0)


def ram_body_last_byte_at_the_rams_end():
    return ram_body_changed("kappa", PROGRAMS["kappa"]["t_size"] - 1)


def ram_body_first_difference_is_named():
    p = PROGRAMS["alpha"]
    a1, a2 = p["t_addr"] + 0x10, p["t_addr"] + 0x20
    d = p["data"]
    rc, l = run(exec_lines("alpha", f"poke {a2:x} {d[0x800 + 0x20] ^ 1:02x}", f"poke {a1:x} {d[0x800 + 0x10] ^ 1:02x}")[:-1] + [f"exec {HDR + 0x10:x}"])
    if rc != EXIT_PROGRAM or len(l) != 1 or f"at 0x{a1:08x}:" not in l[0]:
        return f"status {rc}, lines {l}"


def ram_byte_after_the_body_is_not_compared():
    p = PROGRAMS["alpha"]
    rc, l = run(exec_lines("alpha", f"poke {p['t_addr'] + p['t_size']:x} ff", f"poke {p['t_addr'] - 1:x} ff"))
    return expect(rc, l, 0, ran("alpha"))


def ram_body_not_loaded_at_all_stops():
    rc, l = run(prep("beta", body=False) + [f"exec {HDR + 0x10:x}"])
    d = PROGRAMS["beta"]["data"]
    a = PROGRAMS["beta"]["t_addr"]
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program beta: the body in RAM differs from the file's at 0x{a:08x}: RAM 0x00, file 0x{d[0x800]:02x}"])


# ---- the guards -------------------------------------------------------------

def t_size_beyond_the_file_stops():
    rc, l = run(exec_lines("delta"))
    size = len(PROGRAMS["delta"]["data"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program delta: the header says t_size 0x400, the file's {size} bytes hold only {size - 0x800} after the first 0x800"])


def t_size_of_four_billion_stops_without_wrapping():
    rc, l = run(exec_lines("theta"))
    if rc != EXIT_PROGRAM or len(l) != 1 or "t_size 0xffffffff" not in l[0]:
        return f"status {rc}, lines {l}"


def t_size_one_byte_beyond_the_file_stops():
    size = len(PROGRAMS["short1"]["data"])
    rc, l = run(exec_lines("short1"))
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program short1: the header says t_size 0x400, the file's {size} bytes hold only {size - 0x800} after the first 0x800"])


def span_one_byte_beyond_the_rams_end_stops():
    rc, l = run(exec_lines("over1"))
    return expect(rc, l, EXIT_PROGRAM, ["stop: program over1: the body at 0x801ff001, 0x1000 bytes, is not inside the game's RAM"])


def span_leaving_the_ram_at_its_end_stops():
    rc, l = run(exec_lines("eps"))
    return expect(rc, l, EXIT_PROGRAM, ["stop: program eps: the body at 0x801ff800, 0x1000 bytes, is not inside the game's RAM"])


def span_starting_below_the_ram_stops():
    rc, l = run(exec_lines("zeta"))
    return expect(rc, l, EXIT_PROGRAM, ["stop: program zeta: the body at 0x7ffff000, 0x1000 bytes, is not inside the game's RAM"])


def span_wrapping_around_the_address_space_stops():
    rc, l = run(exec_lines("iota"))
    return expect(rc, l, EXIT_PROGRAM, ["stop: program iota: the body at 0xfffff000, 0x2000 bytes, is not inside the game's RAM"])


def header_beyond_the_rams_end_stops():
    top = RAM_END - 8
    p = PROGRAMS["alpha"]
    rc, l = run(prep("alpha") + [f"exec {top:x}"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program alpha: the header at 0x{top:x}, 16 bytes, is not inside the game's RAM"])


def header_one_byte_beyond_the_rams_end_stops():
    top = RAM_END - 15
    rc, l = run(prep("alpha") + [f"exec {top:x}"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program alpha: the header at 0x{top:x}, 16 bytes, is not inside the game's RAM"])


def header_below_the_ram_stops():
    a = RAM - 16
    rc, l = run(prep("alpha") + [f"exec {a:x}"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program alpha: the header at 0x{a:x}, 16 bytes, is not inside the game's RAM"])


def header_wrapping_the_address_space_stops():
    a = 0xfffffff8
    rc, l = run(prep("alpha") + [f"exec {a:x}"])
    return expect(rc, l, EXIT_PROGRAM, [f"stop: program alpha: the header at 0x{a:x}, 16 bytes, is not inside the game's RAM"])


def header_outside_the_ram_is_checked_after_the_hash():
    path, changed = changed_file("alpha", 0x300, "hdr3")
    rc, l = run(prep("alpha") + [f"exec {RAM_END:x}"], path)
    if rc != EXIT_PROGRAM or "SHA-256" not in "".join(l):
        return f"status {rc}, lines {l}"


def nothing_is_called_before_every_check_passed():
    # a body difference at the very last byte: no setup line was printed
    return ram_body_changed("alpha", PROGRAMS["alpha"]["t_size"] - 1)


NAMES = [intro_replaces_only_after_validation, setup_main_teardown_in_order, callbacks_start_fresh_once_after_the_checks_and_before_setup,
         decoder_report_once_with_the_programs_name_after_everything_else, the_right_one_of_two, the_first_row_of_two_is_not_taken_for_the_second, a_row_without_setup_and_teardown,
         a_later_sector_of_the_file_finds_and_reads_the_whole_file, header_at_the_rams_first_bytes_runs, file_of_exactly_the_headers_fields_with_no_body_runs,
         a_second_exec_runs_again, body_ending_at_the_rams_end_runs, body_starting_at_the_rams_start_runs, header_in_the_last_16_bytes_of_the_ram_runs,
         name_in_a_folder_found_through_the_listing, name_matched_without_regard_to_case, direct_name_with_a_version_suffix, direct_name_in_a_folder,
         direct_name_in_a_mixed_case, direct_name_with_the_first_and_last_letters_in_both_cases, no_disc_open_stops, file_that_cannot_be_read_stops, direct_names_that_are_not_the_row, direct_name_cut_at_the_first_semicolon_only, no_row_does_nothing,
         skip_option_a_built_in_program_still_runs, skip_option_off_then_on_the_same_program, file_not_in_the_table_stops_with_todays_line,
         file_not_in_the_table_with_the_option_skips, a_sector_inside_the_file_not_only_its_first_finds_it, a_sector_no_file_has_stops_with_todays_line,
         no_sector_known_stops_with_todays_line, no_sector_known_with_the_option_skips,
         disc_byte_changed_in_the_headers_sector, disc_byte_changed_in_the_body, disc_last_byte_changed, disc_first_byte_changed,
         disc_changed_byte_in_a_file_that_ends_inside_a_sector, disc_changed_file_stops_before_the_header_is_compared,
         hash_line_names_the_file_the_program_and_both_hashes,
         row_hash_differs_in_the_first_byte, row_hash_differs_in_the_byte_after_the_first_16, row_hash_differs_in_the_last_byte, file_shorter_than_a_header_stops,
         ram_header_pc0_first_byte, ram_header_pc0_last_byte, ram_header_gp0, ram_header_t_addr, ram_header_t_size, ram_header_t_size_last_byte,
         ram_header_two_fields_names_the_first, ram_outside_the_headers_16_bytes_is_not_compared,
         ram_body_first_byte, ram_body_last_byte, ram_body_middle_byte, ram_body_first_byte_at_the_rams_start, ram_body_last_byte_at_the_rams_end,
         ram_body_first_difference_is_named, ram_byte_after_the_body_is_not_compared, ram_body_not_loaded_at_all_stops,
         t_size_beyond_the_file_stops, t_size_one_byte_beyond_the_file_stops, span_one_byte_beyond_the_rams_end_stops, t_size_of_four_billion_stops_without_wrapping,
         span_leaving_the_ram_at_its_end_stops, span_starting_below_the_ram_stops, span_wrapping_around_the_address_space_stops,
         header_beyond_the_rams_end_stops, header_one_byte_beyond_the_rams_end_stops, header_below_the_ram_stops,
         header_wrapping_the_address_space_stops, header_outside_the_ram_is_checked_after_the_hash,
         nothing_is_called_before_every_check_passed]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cc", default=None)
    ap.add_argument("--src", type=Path, default=SRC, help="another copy of the runtime's folder (the mutants run on a scratch copy)")
    ap.add_argument("--only", default="")
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("a C compiler for the host (--cc) is needed; these controls need it")
        return 2
    t0 = time.time()
    failed = count = 0
    with tempfile.TemporaryDirectory(prefix="hostprograms-test-") as tmp:
        root = Path(tmp)
        Ctx.tmp = root
        (root / "progmain.c").write_text(driver_c())
        exe = root / "progtest.exe"
        s = args.src
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(s), "-o", str(exe), str(root / "progmain.c"), str(s / "programs.c"),
                               str(s / "overrides.c"), str(s / "disc.c"), str(s / "sha256.c")] + LINK_FLAGS, capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build and link without the library:\n" + proc.stderr[-2000:])
            return 1
        Ctx.exe = exe
        Ctx.image = root / "image.bin"
        img = make_image(Ctx.image, {n: p["data"] for n, p in PROGRAMS.items()})
        Ctx.sector = dict(img.sector)
        Ctx.size = dict(img.size)
        for fn in NAMES:
            name = fn.__name__
            if args.only not in name:
                continue
            count += 1
            try:
                bad = fn()
            except subprocess.TimeoutExpired:
                bad = "time limit: the driver did not finish"
            except Exception as e:  # the driver ended, or answered short
                bad = f"{type(e).__name__}: {e}"
            if bad is None:
                print(f"ok   {name}")
            else:
                print(f"FAIL {name}: {bad}")
                failed += 1
    print(f"{count} cases, {time.time() - t0:.1f} seconds")
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
