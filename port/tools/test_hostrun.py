#!/usr/bin/env python3
"""Controls for the runtime in port/src/ using disc images that this file makes.

What is tested: disc.c, which is plain stdio, built with the host's own C
compiler (`cc`) around a small test main that this file writes. The images are
a few sectors of bytes this file invents: a volume descriptor, a root
directory, a folder, SYSTEM.CNF and a made-up PS-X EXE. Cases: a file found in
the root and one folder down; version suffix and case; a root directory of two
sectors; a cue sheet and its relative path; each refusal of the program load
(magic, text range outside RAM, short file, bss, entry); a truncated image;
the BOOT line of SYSTEM.CNF; the entry scan on made-up words (the last jal before the first break, a break with no jal before it,
no break, the 64-instruction window, a start that is not at the base); the
file listing (port_disc_list) on directories made to break it: a record that
runs past its sector or is too short, a name that runs past its record, a name
longer than the entry, a directory over the bound or longer than the image, an
extent beyond the image, more files than the caller holds, more folders than the
table holds, a folder inside a folder.

What is NOT tested here: the Windows mapping (memory.c), the jumps (jumps.c),
the stop routine and main.c. Those need the linked Windows program and are
exercised by the real run of the program on a real disc image.

The expected values are worked out here from each fixture, never read back
from the runtime. It needs `cc`; without it this file says so and ends with
status 2.
"""

from __future__ import annotations

import hashlib
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"

SECTOR = 2352
RAM = 0x80000000

TEST_MAIN = r'''
#include "port.h"
#include <stdlib.h>
#include <string.h>
static unsigned char *fakeram(void)
{
    unsigned char *r = malloc(PORT_RAM_SIZE);
    memset(r, 0xaa, PORT_RAM_SIZE);
    return r;
}
int main(int argc, char **argv)
{
    char err[PORT_ERR];
    if (argc >= 3 && strcmp(argv[1], "sha") == 0) {
        FILE *f = fopen(argv[2], "rb");
        unsigned char *buf = malloc(1 << 21), out[32];
        size_t n = f ? fread(buf, 1, 1 << 21, f) : 0, i;
        port_sha256(buf, n, out);
        for (i = 0; i < 32; i++) printf("%02x", out[i]);
        printf("\n");
        return 0;
    }
    if (argc >= 4 && strcmp(argv[1], "load") == 0) {
        struct port_disc d;
        unsigned char sha[32];
        unsigned k;
        for (k = 0; k < 32; k++) { char two[3] = { argv[3][2 * k], argv[3][2 * k + 1], 0 }; sha[k] = (unsigned char)strtoul(two, 0, 16); }
        struct port_program p;
        unsigned char *ram = fakeram();
        if (port_disc_open(&d, argv[2], err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
        printf("disc: %s\n", d.path);
        if (port_program_load(&d, ram, sha, &p, err, sizeof err) != 0) {
            printf("ram: %02x %02x\n", ram[0x10000], ram[0x10000 + 100]);
            printf("refused: %s\n", err);
            return 2;
        }
        printf("program: %s sector %u t_size %u t_addr %08x pc0 %08x\n", p.name, p.sector, p.t_size, p.t_addr, p.pc0);
        printf("bytes: text0 %02x textlast %02x after %02x bss0 %02x bsslast %02x bssafter %02x\n",
               ram[p.t_addr - PORT_RAM_BASE], ram[p.t_addr - PORT_RAM_BASE + p.t_size - 1], ram[p.t_addr - PORT_RAM_BASE + p.t_size],
               p.b_size ? ram[p.b_addr - PORT_RAM_BASE] : 0, p.b_size ? ram[p.b_addr - PORT_RAM_BASE + p.b_size - 1] : 0, p.b_size ? ram[p.b_addr - PORT_RAM_BASE + p.b_size] : 0xaa);
        return 0;
    }
    if (argc >= 4 && strcmp(argv[1], "list") == 0) {   /* list IMAGE MAX: every file of the root and one folder down */
        struct port_disc d;
        static struct port_disc_file files[4096];
        unsigned count = 0, i, max = (unsigned)atoi(argv[3]);
        if (max > 4096) max = 4096;
        if (port_disc_open(&d, argv[2], err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
        if (port_disc_list(&d, files, max, &count, err, sizeof err) != 0) { printf("refused: %s\n", err); return 2; }
        for (i = 0; i < count; i++) printf("file %s %u %u\n", files[i].name, files[i].sector, files[i].size);
        return 0;
    }
    if (argc >= 3 && strcmp(argv[1], "boot") == 0) {
        char name[64];
        if (port_boot_name(argv[2], strlen(argv[2]), name, sizeof name) != 0) { printf("none\n"); return 0; }
        printf("name %s\n", name);
        return 0;
    }
    if (argc >= 3 && strcmp(argv[1], "scan") == 0) {
        unsigned char *ram = fakeram();
        unsigned pc0 = (unsigned)strtoul(argv[2], 0, 16), target, i;
        for (i = 3; pc0 >= PORT_RAM_BASE && i < (unsigned)argc; i++) {
            unsigned w = (unsigned)strtoul(argv[i], 0, 16), at = pc0 - PORT_RAM_BASE + 4 * (i - 3);
            ram[at] = (unsigned char)w; ram[at + 1] = (unsigned char)(w >> 8); ram[at + 2] = (unsigned char)(w >> 16); ram[at + 3] = (unsigned char)(w >> 24);
        }
        if (pc0 >= PORT_RAM_BASE && pc0 - PORT_RAM_BASE + 4 * 70 <= PORT_RAM_SIZE)
            for (i = pc0 - PORT_RAM_BASE; i < pc0 - PORT_RAM_BASE + 4 * 70; i++) if (ram[i] == 0xaa) ram[i] = 0;
        if (port_entry_scan(ram, pc0, &target) != 0) { printf("none\n"); return 0; }
        printf("target %08x\n", target);
        return 0;
    }
    return 64;
}
'''


GATE_MAIN = r"""
#include "port.h"
#include "port_tables.h"
#include <stdlib.h>
static void fa(void) {}
char port_game_text_begin, port_game_text_end;   /* jumps.c checks call targets against these; this test does not call it */
const struct port_image port_images[] = {{ "mod", 0x80180000u, 0, 1, 0, 0 }};
const unsigned port_image_count = 1;
const struct port_function port_functions[] = {{ 0x80100000u, (void *)fa, "fa", -1 }, { 0x80100040u, (void *)fa, "fb", -1 }, { 0x80180000u, (void *)fa, "m", 0 }};
const unsigned port_function_count = 3;
const struct port_absent port_absents[] = {{ 0x80100020u, "x", -1, 0 }, { 0x801fffb0u, "end", -1, 1 }, { 0x80180100u, "my", 0, 0 }
#ifdef DUP
, { 0x80100000u, "dup", -1, 0 }
#endif
};
const unsigned port_absent_count = sizeof port_absents / sizeof port_absents[0];
const unsigned char port_program_sha256[32] = {0};
int main(int argc, char **argv)
{
    char err[PORT_ERR];
    int i;
    if (port_jump_known(0x80100000u)) { printf("known before build\n"); return 1; }
    if (port_jump_set_build(err, sizeof err) != 0) { printf("build: %s\n", err); return 1; }
    for (i = 1; i < argc; i++) printf("%s %d\n", argv[i], port_jump_known((unsigned)strtoul(argv[i], 0, 16)));
    return 0;
}
"""


# ---- making images -------------------------------------------------------

def record(name: bytes, extent: int, size: int, directory: bool) -> bytes:
    n = len(name)
    length = 33 + n + (1 - n % 2)  # padded to an even length
    r = bytearray(length)
    r[0] = length
    r[2:6] = struct.pack("<I", extent)
    r[6:10] = struct.pack(">I", extent)
    r[10:14] = struct.pack("<I", size)
    r[14:18] = struct.pack(">I", size)
    r[25] = 2 if directory else 0
    r[32] = n
    r[33:33 + n] = name
    return bytes(r)


def sectors(data: bytes) -> int:
    return max(1, -(-len(data) // 2048))


class Image:
    """root: dict of name -> bytes (a file) or dict (a folder, files only).
    names are written as given (so the control chooses ;1 and case)."""

    def __init__(self, root: dict, root_sectors: int = 1, first_dir_sector: int = 20):
        self.sector = {}          # path -> first sector
        self.size = {}
        self.data = {}            # sector number -> 2048 bytes
        nxt = first_dir_sector
        root_at = nxt
        nxt += root_sectors
        folder_at = {}
        for name, v in root.items():
            if isinstance(v, dict):
                folder_at[name] = nxt
                nxt += 1
        files = {}
        for name, v in root.items():
            if isinstance(v, dict):
                for inner, content in v.items():
                    files[name + "/" + inner] = content
            else:
                files[name] = content = v
        # files after the directories
        placed = {}
        for path, content in files.items():
            placed[path] = nxt
            nxt += sectors(content)
        self.end = nxt
        for path, content in files.items():
            self.sector[path] = placed[path]
            self.size[path] = len(content)
            for i in range(sectors(content)):
                self.data[placed[path] + i] = content[i * 2048:(i + 1) * 2048].ljust(2048, b"\0")

        def directory(self_extent, parent_extent, entries, nsec):
            recs = [record(b"\0", self_extent, 2048 * nsec, True), record(b"\1", parent_extent, 2048, True)] + entries
            return recs

        def lay(extent, recs, nsec, split_at=None):
            buf = [bytearray(2048) for _ in range(nsec)]
            s, pos = 0, 0
            for k, r in enumerate(recs):
                if split_at is not None and k == split_at:
                    s, pos = 1, 0     # the rest goes to the next sector, the first keeps zero padding
                assert pos + len(r) <= 2048
                buf[s][pos:pos + len(r)] = r
                pos += len(r)
            for i in range(nsec):
                self.data[extent + i] = bytes(buf[i])

        for name, v in root.items():
            if isinstance(v, dict):
                recs = directory(folder_at[name], root_at, [record(inner.encode(), placed[name + "/" + inner], len(c), False) for inner, c in v.items()], 1)
                lay(folder_at[name], recs, 1)
        entries = []
        for name, v in root.items():
            if isinstance(v, dict):
                entries.append(record(name.encode(), folder_at[name], 2048, True))
            else:
                entries.append(record(name.encode(), placed[name], len(v), False))
        recs = directory(root_at, root_at, entries, root_sectors)
        lay(root_at, recs, root_sectors, split_at=2 + len(entries) // 2 if root_sectors == 2 else None)
        pvd = bytearray(2048)
        pvd[0] = 1
        pvd[1:6] = b"CD001"
        pvd[6] = 1
        pvd[156:156 + 34] = record(b"\0", root_at, 2048 * root_sectors, True)[:34]
        pvd[156] = 34
        self.data[16] = bytes(pvd)

    def write(self, path: Path, cut: int | None = None) -> None:
        out = bytearray()
        for s in range(self.end):
            out += bytes(24) + self.data.get(s, bytes(2048)) + bytes(SECTOR - 24 - 2048)
        path.write_bytes(bytes(out[:cut] if cut is not None else out))


def exe(t_addr=RAM + 0x10000, t_size=3000, pc0=None, b_addr=RAM + 0x20000, b_size=64, magic=b"PS-X EXE", file_text=None, header_only=False):
    pc0 = t_addr if pc0 is None else pc0
    h = bytearray(0x800)
    h[0:8] = magic
    struct.pack_into("<I", h, 0x10, pc0)
    struct.pack_into("<I", h, 0x18, t_addr)
    struct.pack_into("<I", h, 0x1c, t_size)
    struct.pack_into("<I", h, 0x28, b_addr)
    struct.pack_into("<I", h, 0x2c, b_size)
    text = bytes((i * 7 + 0x11) & 0xff if (i * 7 + 0x11) & 0xff != 0xaa else 0x55 for i in range(t_size if file_text is None else file_text))
    return bytes(h) if header_only else bytes(h) + text


def text_byte(i: int) -> int:
    v = (i * 7 + 0x11) & 0xff
    return 0x55 if v == 0xaa else v


CNF = b"BOOT = cdrom:\\SLPS_004.15;1\r\nTCB = 4\r\nEVENT = 10\r\nSTACK = 801FFFF0\r\n"


# ---- running -------------------------------------------------------------

def build_test(root: Path, cc: str) -> Path:
    main = root / "test_main.c"
    main.write_text(TEST_MAIN)
    exe_path = root / "testrun"
    proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(exe_path), str(main), str(SRC / "disc.c"), str(SRC / "sha256.c")],
                          capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        print("could not build disc.c with the host compiler:\n" + proc.stderr)
        sys.exit(2)
    return exe_path


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(prog: Path, *args) -> tuple[int, list[str]]:
    proc = subprocess.run([str(prog), *map(str, args)], capture_output=True, text=True, timeout=60)
    return proc.returncode, proc.stdout.splitlines()


def expect_load(prog: Path, img: Path, name: str, sector: int, exe_args: dict | None = None):
    """The success lines for an exe made by exe(**exe_args)."""
    a = dict(t_addr=RAM + 0x10000, t_size=3000, b_addr=RAM + 0x20000, b_size=64)
    a.update(exe_args or {})
    status, lines = run(prog, "load", img, sha(exe(**a)))
    pc0 = a.get("pc0", a["t_addr"])
    want = [f"disc: {img}",
            f"program: {name} sector {sector} t_size {a['t_size']} t_addr {a['t_addr']:08x} pc0 {pc0:08x}",
            f"bytes: text0 {text_byte(0):02x} textlast {text_byte(a['t_size'] - 1):02x} after aa bss0 00 bsslast 00 bssafter aa"]
    if status == 0 and lines == want:
        return None
    return f"status {status}, lines {lines!r}, wanted {want!r}"


def expect_refusal(prog: Path, img: Path, *words: str, pin: bytes | None = None):
    """pin: the program the build pins (default: the made-up exe() every image holds)."""
    status, lines = run(prog, "load", img, sha(exe() if pin is None else pin))
    refused = [l for l in lines if l.startswith("refused: ")]
    if status != 2 or len(refused) != 1 or lines[-1] != refused[0]:
        return f"status {status}, lines {lines!r}"
    low = refused[0].lower()
    miss = [w for w in words if w.lower() not in low]
    return f"line {refused[0]!r} lacks {miss}" if miss else None


# ---- cases ---------------------------------------------------------------

def disc_cases(root: Path, prog: Path):
    d = root / "disc"
    d.mkdir()
    base = {"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": exe()}

    def make(tag, files, **kw):
        img = Image(files, **kw)
        path = d / f"{tag}.bin"
        img.write(path)
        return img, path

    img, path = make("root", base)
    yield "file-in-root", expect_load(prog, path, "SLPS_004.15", img.sector["SLPS_004.15;1"])

    # cue sheet next to the image, in another folder, relative path with a subfolder and "..", mixed case, CRLF
    (d / "img").mkdir()
    shutil.copy(path, d / "img" / "x.bin")
    (d / "sheets").mkdir()
    cue = d / "sheets" / "game.CUE"
    cue.write_bytes(b'REM made up\r\nFILE "sound.wav" WAVE\r\n  file "../img/x.bin" binary\r\n  TRACK 01 MODE2/2352\r\n    INDEX 01 00:00:00\r\n')
    status, lines = run(prog, "load", cue, sha(exe()))
    joined = f"{d / 'sheets'}/../img/x.bin"
    yield "cue-relative-path", None if status == 0 and lines[0] == f"disc: {joined}" and lines[1].startswith("program: SLPS_004.15 sector") else f"status {status}, lines {lines!r}"
    cue2 = d / "game2.cue"
    cue2.write_text('FILE "path with space.bin" BINARY\n')
    shutil.copy(path, d / "path with space.bin")
    status, lines = run(prog, "load", cue2, sha(exe()))
    yield "cue-name-with-space-in-same-folder", None if status == 0 and lines[0] == f"disc: {d}/path with space.bin" else f"status {status}, lines {lines!r}"
    cue3 = d / "none.cue"
    cue3.write_text('REM nothing\nFILE "a.wav" WAVE\nTRACK 01 AUDIO\n')
    yield "cue-without-binary-file-line", expect_refusal(prog, cue3, "cue", "FILE")
    cue4 = d / "missing.cue"
    cue4.write_text('FILE "nothere.bin" BINARY\n')
    yield "cue-names-a-missing-image", expect_refusal(prog, cue4, "image")
    yield "missing-image", expect_refusal(prog, d / "absent.bin", "image")

    # one folder down; the other folder is searched too; lower case on the disc, upper in SYSTEM.CNF
    img, path = make("folder", {"SYSTEM.CNF;1": CNF, "OTHER": {"README.TXT;1": b"x" * 10}, "BIN": {"slps_004.15;1": exe(t_size=2048 * 2 + 5)}})
    yield "file-one-folder-down-case-insensitive", expect_load(prog, path, "SLPS_004.15", img.sector["BIN/slps_004.15;1"], dict(t_size=2048 * 2 + 5))
    # SYSTEM.CNF in lower case on the disc without a version, boot name in lower case without ;1
    img, path = make("nover", {"system.cnf": b"boot=cdrom:\\slps_004.15\n", "SLPS_004.15": exe(t_size=1)})
    yield "no-version-suffix-and-lower-case", expect_load(prog, path, "slps_004.15", img.sector["SLPS_004.15"], dict(t_size=1))
    # a root directory of two sectors; the first keeps zero padding
    img, path = make("two", {"A.DAT;1": b"a", "B.DAT;1": b"b", "SYSTEM.CNF;1": CNF, "C.DAT;1": b"c", "SLPS_004.15;1": exe()}, root_sectors=2)
    yield "root-directory-of-two-sectors", expect_load(prog, path, "SLPS_004.15", img.sector["SLPS_004.15;1"])
    # a name that only begins like the file, and a file two folders down: not found
    img, path = make("prefix", {"SYSTEM.CNF;1": CNF, "SLPS_004.1;1": exe(), "SLPS_004.155;1": exe(), "X": {"SLPS_004.15X;1": exe()}})
    yield "name-that-only-begins-alike-is-not-found", expect_refusal(prog, path, "SLPS_004.15", "not in")
    img, path = make("nofnd", {"SYSTEM.CNF;1": CNF})
    yield "boot-file-missing", expect_refusal(prog, path, "SLPS_004.15")
    img, path = make("nocnf", {"SLPS_004.15;1": exe()})
    yield "system-cnf-missing", expect_refusal(prog, path, "SYSTEM.CNF")
    img, path = make("noboot", {"SYSTEM.CNF;1": b"TCB = 4\r\nBOOT2 = cdrom:\\X.EXE;1\r\n", "SLPS_004.15;1": exe()})
    yield "system-cnf-without-boot-line", expect_refusal(prog, path, "BOOT")
    # no volume descriptor
    img, path = make("novd", base)
    raw = bytearray(path.read_bytes())
    raw[16 * SECTOR + 24 + 1:16 * SECTOR + 24 + 6] = b"CDXXX"
    path.write_bytes(bytes(raw))
    yield "no-volume-descriptor", expect_refusal(prog, path, "volume descriptor")
    # truncated images
    img, path = make("cut", base)
    last = img.sector["SLPS_004.15;1"] + sectors(base["SLPS_004.15;1"]) - 1
    img.write(path, cut=last * SECTOR + 24 + 100)
    yield "image-cut-inside-the-boot-file", expect_refusal(prog, path, "truncated", str(last), "file slps_004.15 lies outside the image")
    img.write(path, cut=10 * SECTOR + 700)
    yield "image-cut-before-the-volume-descriptor", expect_refusal(prog, path, "truncated", "16")
    path.write_bytes(b"")
    yield "empty-image", expect_refusal(prog, path, "truncated")


def list_cases(root: Path, prog: Path):
    d = root / "list"
    d.mkdir()
    base = {"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": exe(), "BIN": {"A.EXE;1": exe(), "B.BIN;1": b"x" * 5000}}

    def made(tag, patch=None, cut=None, **kw):
        img = Image(base, **kw)
        if patch:
            patch(img)
        path = d / f"{tag}.bin"
        img.write(path, cut)
        return img, path

    def listing(path, mx=100):
        status, lines = run(prog, "list", path, mx)
        return status, lines

    img, path = made("ok")
    status, lines = listing(path)
    want = [f"file system.cnf {img.sector['SYSTEM.CNF;1']} {len(CNF)}", f"file slps_004.15 {img.sector['SLPS_004.15;1']} {len(exe())}",
            f"file a.exe {img.sector['BIN/A.EXE;1']} {len(exe())}", f"file b.bin {img.sector['BIN/B.BIN;1']} 5000"]
    yield "list-gives-the-root-and-one-folder-in-directory-order-without-version-suffixes", None if (status, lines) == (0, want) else f"status {status}, lines {lines!r}"
    status, lines = listing(path, 3)
    yield "list-with-more-files-than-the-caller-holds-is-refused", None if status == 2 and lines == ["refused: disc: more files than the listing holds"] else f"status {status}, lines {lines!r}"
    status, lines = listing(path, 4)
    yield "list-with-exactly-as-many-files-as-the-caller-holds-is-accepted", None if status == 0 and len(lines) == 4 else f"status {status}, lines {lines!r}"
    status, lines = listing(path, 0)
    yield "list-with-room-for-no-file-is-refused-and-writes-nothing", None if status == 2 and lines == ["refused: disc: more files than the listing holds"] else f"status {status}, lines {lines!r}"

    root_at = 20

    def end_of_records(buf):
        pos = 0
        while buf[pos]:
            pos += buf[pos]
        return pos

    def rec_at(img, sector, pos, data):
        """Append `data` after the last record (pos = 0), or after valid filler records until the next record would start at `pos` or later."""
        buf = bytearray(img.data[sector])
        end = end_of_records(buf)
        k = 0
        while end < pos:
            filler = record(("F%02d" % k).encode() + b"Q" * 60, 16, 1, False)   # a file inside the image
            buf[end:end + len(filler)] = filler
            end += len(filler)
            k += 1
        buf[end:end + len(data)] = data
        img.data[sector] = bytes(buf)

    # a record that runs past the end of its sector: length 200 at offset 2000 (the root's records end far below)
    img, path = made("runs-past-sector", lambda i: rec_at(i, root_at, 2000, bytes([200]) + bytes(40)))
    status, lines = listing(path)
    yield "list-a-directory-record-that-runs-past-its-sector-is-refused", None if status == 2 and lines == ["refused: disc: corrupt directory record"] else f"status {status}, lines {lines!r}"
    # a record whose name length runs past the record: length 34, name length 100
    r = bytearray(record(b"X;1", 5, 10, False)); r[32] = 100
    img, path = made("name-runs-past-record", lambda i: rec_at(i, root_at, 0, bytes(r)))
    status, lines = listing(path)
    yield "list-a-name-that-runs-past-its-record-is-refused", None if status == 2 and lines == ["refused: disc: corrupt directory record"] else f"status {status}, lines {lines!r}"
    # a record shorter than the fixed part
    img, path = made("short-record", lambda i: rec_at(i, root_at, 0, bytes([33]) + bytes(40)))
    status, lines = listing(path)
    yield "list-a-record-shorter-than-its-fixed-part-is-refused", None if status == 2 and lines == ["refused: disc: corrupt directory record"] else f"status {status}, lines {lines!r}"
    # a name of 200 characters: kept to what the entry holds, nothing written beyond it
    long_name = b"N" * 200
    img, path = made("long-name", lambda i: rec_at(i, root_at, 0, record(long_name + b";1", 7, 9, False)))
    status, lines = listing(path)
    got = [l for l in lines if l.endswith(" 7 9")]
    yield "list-a-name-longer-than-the-entry-is-cut-and-the-listing-goes-on", None if status == 0 and len(got) == 1 and len(got[0].split()[1]) == 31 and len(lines) == 5 else f"status {status}, lines {lines!r}"

    # sizes and extents from the image
    def big_root(i):
        pvd = bytearray(i.data[16])
        pvd[156 + 10:156 + 14] = struct.pack("<I", 0x200000)   # a root directory of 2 MB
        i.data[16] = bytes(pvd)
    img, path = made("big-root", big_root)
    status, lines = listing(path)
    yield "list-a-root-directory-larger-than-the-bound-is-refused", None if status == 2 and lines == ["refused: disc: directory is too large"] else f"status {status}, lines {lines!r}"

    def far_root(i):
        pvd = bytearray(i.data[16])
        pvd[156 + 2:156 + 6] = struct.pack("<I", 0xfffffff0)
        i.data[16] = bytes(pvd)
    img, path = made("far-root", far_root)
    status, lines = listing(path)
    yield "list-a-root-extent-beyond-the-image-is-refused-with-its-sector", None if status == 2 and lines == ["refused: disc: the image ends inside sector 4294967280 (truncated)"] else f"status {status}, lines {lines!r}"

    def long_root(i):
        pvd = bytearray(i.data[16])
        pvd[156 + 10:156 + 14] = struct.pack("<I", 2048 * 40)   # says 40 sectors; the image holds the files right behind the first
        i.data[16] = bytes(pvd)
    img = Image({"BIN": {}})            # no file: the root (sector 20) and one empty folder (21), and the image ends there
    long_root(img)
    path = d / "root-past-image.bin"
    img.write(path)
    status, lines = listing(path)
    yield "list-a-directory-that-says-it-is-longer-than-the-image-is-refused", None if status == 2 and lines == [f"refused: disc: the image ends inside sector {img.end} (truncated)"] and img.end == 22 else f"status {status}, lines {lines!r}"

    def far_folder(i):
        buf = bytearray(i.data[root_at])
        pos = 0
        while buf[pos]:
            ln = buf[pos]
            if buf[pos + 25] & 2 and buf[pos + 32] == 3:
                buf[pos + 2:pos + 6] = struct.pack("<I", 0x7fffffff)
            pos += ln
        i.data[root_at] = bytes(buf)
    img, path = made("far-folder", far_folder)
    status, lines = listing(path)
    yield "list-a-folder-extent-beyond-the-image-is-refused", None if status == 2 and lines == ["refused: disc: the image ends inside sector 2147483647 (truncated)"] else f"status {status}, lines {lines!r}"

    # the files' own ranges: every sector and size that the listing hands out lies inside the image
    def set_file(i, name: bytes, extent=None, size=None, sector=root_at):
        buf = bytearray(i.data[sector])
        pos = hit = 0
        while buf[pos]:
            ln, n = buf[pos], buf[pos + 32]
            if not buf[pos + 25] & 2 and bytes(buf[pos + 33:pos + 33 + n]) == name:
                if extent is not None:
                    buf[pos + 2:pos + 6] = struct.pack("<I", extent)
                if size is not None:
                    buf[pos + 10:pos + 14] = struct.pack("<I", size)
                hit += 1
            pos += ln
        assert hit == 1, (name, hit)
        i.data[sector] = bytes(buf)

    def outside_line(i, name, sector, size):
        return f"refused: disc: file {name} lies outside the image (sector {sector}, {size} bytes): the image ends inside sector {i.end} (truncated)"
    img, path = made("file-starts-beyond", lambda i: set_file(i, b"SYSTEM.CNF;1", extent=900))
    status, lines = listing(path)
    yield "list-a-file-that-starts-beyond-the-image-is-refused", None if status == 2 and lines == [outside_line(img, "system.cnf", 900, len(CNF))] else f"status {status}, lines {lines!r}"
    img, path = made("file-size-huge", lambda i: set_file(i, b"SYSTEM.CNF;1", size=0xffffffff))
    status, lines = listing(path)
    yield "list-a-file-of-4294967295-bytes-is-refused", None if status == 2 and lines == [outside_line(img, "system.cnf", img.sector["SYSTEM.CNF;1"], 0xffffffff)] else f"status {status}, lines {lines!r}"
    img, path = made("file-start-wraps", lambda i: set_file(i, b"SYSTEM.CNF;1", extent=0xffffffff, size=4096))
    status, lines = listing(path)
    yield "list-a-file-whose-sectors-wrap-32-bits-is-refused", None if status == 2 and lines == [outside_line(img, "system.cnf", 0xffffffff, 4096)] else f"status {status}, lines {lines!r}"
    folder_sector = root_at + 1                       # the one folder, BIN, lies right behind the root
    last = img.sector["BIN/B.BIN;1"]                  # the last file: 5000 bytes, three sectors, the image ends with them
    img, path = made("file-to-the-last-byte", lambda i: set_file(i, b"B.BIN;1", size=3 * 2048, sector=folder_sector))
    status, lines = listing(path)
    yield "list-a-file-that-ends-with-the-images-last-data-byte-is-accepted", None if status == 0 and lines[-1] == f"file b.bin {last} {3 * 2048}" and img.end == last + 3 else f"status {status}, lines {lines!r}, end {img.end}"
    img, path = made("file-one-byte-past", lambda i: set_file(i, b"B.BIN;1", size=3 * 2048 + 1, sector=folder_sector))
    status, lines = listing(path)
    yield "list-a-file-one-byte-longer-than-the-image-holds-is-refused", None if status == 2 and lines == [outside_line(img, "b.bin", last, 3 * 2048 + 1)] else f"status {status}, lines {lines!r}"
    img, path = made("empty-file-last-sector", lambda i: set_file(i, b"B.BIN;1", extent=i.end - 1, size=0, sector=folder_sector))
    status, lines = listing(path)
    yield "list-an-empty-file-in-the-images-last-sector-is-accepted", None if status == 0 and lines[-1] == f"file b.bin {img.end - 1} 0" else f"status {status}, lines {lines!r}"
    img, path = made("empty-file-past-end", lambda i: set_file(i, b"B.BIN;1", extent=i.end, size=0, sector=folder_sector))
    status, lines = listing(path)
    yield "list-an-empty-file-behind-the-images-last-sector-is-refused", None if status == 2 and lines == [outside_line(img, "b.bin", img.end, 0)] else f"status {status}, lines {lines!r}"

    # the image is cut inside the last sector of the last file: the file's start is inside, its last byte is not
    img, path = made("cut-inside-last-file", cut=(last + 2) * SECTOR + 24 + 100)
    status, lines = listing(path)
    want = f"refused: disc: file b.bin lies outside the image (sector {last}, 5000 bytes): the image ends inside sector {last + 2} (truncated)"
    yield "list-a-file-whose-last-bytes-the-image-lacks-is-refused", None if status == 2 and lines == [want] else f"status {status}, lines {lines!r}"
    img, path = made("cut-one-byte-short", cut=(last + 2) * SECTOR + 24 + (5000 - 2 * 2048) - 1)
    status, lines = listing(path)
    yield "list-an-image-that-lacks-the-last-files-last-byte-is-refused", None if status == 2 and lines == [want] else f"status {status}, lines {lines!r}"
    img, path = made("cut-behind-last-file-byte", cut=(last + 2) * SECTOR + 24 + (5000 - 2 * 2048))
    status, lines = listing(path)
    yield "list-an-image-cut-right-behind-the-last-files-last-byte-is-accepted", None if status == 0 and lines[-1] == f"file b.bin {last} 5000" else f"status {status}, lines {lines!r}"

    # the directory's declared size ends the directory, whatever stands behind it in the sector
    two = len(record(b"\0", 0, 0, True)) + len(record(b"\1", 0, 0, True))    # the records of the folder itself and of its parent
    first = len(record(b"SYSTEM.CNF;1", 0, 0, False))

    def root_size(n):
        def patch(i):
            pvd = bytearray(i.data[16])
            pvd[156 + 10:156 + 14] = struct.pack("<I", n)
            i.data[16] = bytes(pvd)
        return patch
    img, path = made("root-of-two-records", root_size(two))
    status, lines = listing(path)
    yield "list-a-root-that-declares-only-its-two-own-records-gives-no-file", None if (status, lines, two) == (0, [], 68) else f"status {status}, lines {lines!r}, two {two}"
    status, lines = run(prog, "load", path, "00" * 32)   # the other reader, which looks a file up by name
    yield "find-in-a-root-that-declares-only-its-two-own-records-finds-no-file", None if status == 2 and "is not in the root directory" in lines[-1] else f"status {status}, lines {lines!r}"
    img, path = made("root-of-three-records", root_size(two + first))
    status, lines = listing(path)
    yield "list-a-root-that-ends-behind-its-first-file-gives-that-file-only", None if (status, lines) == (0, [f"file system.cnf {img.sector['SYSTEM.CNF;1']} {len(CNF)}"]) else f"status {status}, lines {lines!r}"
    img, path = made("record-crosses-root-end", root_size(two + first - 1))
    status, lines = listing(path)
    yield "list-a-record-that-crosses-the-declared-end-of-its-directory-is-refused", None if status == 2 and lines == ["refused: disc: corrupt directory record"] else f"status {status}, lines {lines!r}"
    status, lines = run(prog, "load", path, "00" * 32)
    yield "find-a-record-that-crosses-the-declared-end-of-its-directory-is-refused", None if status == 2 and lines[-1] == "refused: disc: corrupt directory record" else f"status {status}, lines {lines!r}"
    img, path = made("record-begins-at-root-end", root_size(two + first + 1))
    status, lines = listing(path)
    yield "list-a-record-that-only-begins-inside-the-declared-end-is-refused", None if status == 2 and lines == ["refused: disc: corrupt directory record"] else f"status {status}, lines {lines!r}"

    # more folders than the table holds: the first 256 are followed, the rest ignored, nothing overflows
    def many_folders(i):
        folder_at = int.from_bytes(i.data[root_at][0:0], "little")
        buf = bytearray(i.data[root_at])
        pos = 0
        while buf[pos]:
            ln = buf[pos]
            if buf[pos + 25] & 2 and buf[pos + 32] == 3:
                folder_at = struct.unpack_from("<I", buf, pos + 2)[0]
            pos += ln
        recs = [record(b"\0", root_at, 2048 * 6, True), record(b"\1", root_at, 2048, True)]
        recs += [record(f"D{k:03d}".encode(), folder_at, 2048, True) for k in range(300)]
        sec, off = root_at, 0
        blocks = [bytearray(2048) for _ in range(6)]
        b = 0
        for r in recs:
            if off + len(r) > 2048:
                b, off = b + 1, 0
            blocks[b][off:off + len(r)] = r
            off += len(r)
        for k in range(6):
            i.data[root_at + k] = bytes(blocks[k])
        pvd = bytearray(i.data[16])
        pvd[156 + 10:156 + 14] = struct.pack("<I", 2048 * 6)
        i.data[16] = bytes(pvd)
    img, path = made("many-folders", many_folders, root_sectors=6)
    status, lines = listing(path, 4096)
    got = [l for l in lines if l.startswith("file a.exe ")]
    yield "list-more-folders-than-the-table-holds", None if status == 0 and len(got) == 256 else f"status {status}, {len(got)} folders followed, lines {lines[:2]!r}"

    # a folder inside a folder is not followed
    nested = {"SYSTEM.CNF;1": CNF, "TOP": {"IN.BIN;1": b"z"}}
    img = Image(nested)
    path = d / "nested.bin"
    img.write(path)
    status, lines = listing(path)
    yield "list-does-not-follow-a-folder-inside-a-folder", None if status == 0 and [l.split()[1] for l in lines] == ["system.cnf", "in.bin"] else f"status {status}, lines {lines!r}"


def exe_cases(root: Path, prog: Path):
    d = root / "exe"
    d.mkdir()

    def refusal(tag, e, *words):
        img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": e})
        path = d / f"{tag}.bin"
        img.write(path)
        return expect_refusal(prog, path, *words, pin=e)
    yield "bad-magic", refusal("magic", exe(magic=b"PS-X EXF"), "magic")
    yield "file-shorter-than-a-header", refusal("hdr", b"PS-X EXE" + bytes(100), "short")
    yield "text-below-ram", refusal("low", exe(t_addr=RAM - 0x1000), "outside RAM")
    yield "text-at-physical-address", refusal("phys", exe(t_addr=0x00010000), "outside RAM")
    yield "text-past-the-end-of-ram", refusal("high", exe(t_addr=RAM + 0x1ff000, t_size=0x2000, file_text=0x2000), "outside RAM")
    yield "text-size-wraps-around", refusal("wrap", exe(t_addr=RAM + 0x10000, t_size=0xffffff00, file_text=16), "outside RAM")
    yield "text-fits-exactly-to-the-end-of-ram", expect_ok_end(d, prog)
    yield "file-shorter-than-t-size", refusal("short", exe(t_size=3000, file_text=2000), "short")
    yield "bss-outside-ram", refusal("bss", exe(b_addr=RAM + 0x1fffe0, b_size=0x100), "bss")
    yield "entry-outside-ram", refusal("pc0", exe(pc0=0x80300000), "entry")
    yield "no-bss-is-fine", expect_nobss(d, prog)


def expect_ok_end(d: Path, prog: Path):
    e = exe(t_addr=RAM + 0x1ff000, t_size=0x1000 - 256, pc0=RAM + 0x1ff000, b_addr=RAM + 0x20000, b_size=4)
    img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": e})
    path = d / "end.bin"
    img.write(path)
    return expect_load(prog, path, "SLPS_004.15", img.sector["SLPS_004.15;1"], dict(t_addr=RAM + 0x1ff000, t_size=0x1000 - 256, b_size=4))


def expect_nobss(d: Path, prog: Path):
    e = exe(b_size=0, b_addr=0)
    img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": e})
    path = d / "nobss.bin"
    img.write(path)
    status, lines = run(prog, "load", path, sha(e))
    return None if status == 0 and lines[1].startswith("program: SLPS_004.15 sector") else f"status {status}, lines {lines!r}"


def boot_cases(root: Path, prog: Path):
    table = [
        ("plain", "BOOT = cdrom:\\SLPS_004.15;1\r\n", "SLPS_004.15"),
        ("no-spaces-lower-case", "boot=cdrom:\\slps_004.15;1\n", "slps_004.15"),
        ("many-spaces-and-tabs", "TCB = 4\n  BOOT \t=   CDROM:\\A.EXE;1  \n", "A.EXE"),
        ("forward-slash-and-folder", "BOOT = cdrom:/GAME/B.EXE;1\n", "B.EXE"),
        ("no-cdrom-prefix", "BOOT = \\C.EXE;1\n", "C.EXE"),
        ("no-version", "BOOT = cdrom:\\D.EXE\n", "D.EXE"),
        ("boot2-is-not-boot", "BOOT2 = cdrom:\\X;1\nBOOT = cdrom:\\Y.EXE;1\n", "Y.EXE"),
        ("last-line-without-line-end", "TCB = 4\nBOOT = cdrom:\\E.EXE;1", "E.EXE"),
        ("none", "TCB = 4\nEVENT = 10\n", None),
        ("only-boot2", "BOOT2 = cdrom:\\X;1\n", None),
        ("boot-without-name", "BOOT = cdrom:\\;1\n", None),
    ]
    for tag, text, want in table:
        status, lines = run(prog, "boot", text)
        got = "none" if want is None else f"name {want}"
        yield f"boot-line-{tag}", None if status == 0 and lines == [got] else f"status {status}, lines {lines!r}, wanted {got!r}"


def jal(target: int) -> str:
    return f"{0x0c000000 | ((target & 0x0fffffff) >> 2):08x}"


def scan_cases(root: Path, prog: Path):
    nop = "00000000"
    base = f"{RAM:08x}"

    def scan(pc0, words):
        status, lines = run(prog, "scan", f"{pc0:08x}", *words)
        return status, lines

    def case(tag, pc0, words, want):
        status, lines = scan(pc0, words)
        got = "none" if want is None else f"target {want:08x}"
        return f"scan-{tag}", None if status == 0 and lines == [got] else f"status {status}, lines {lines!r}, wanted {got!r}"

    brk = "0000000d"
    yield case("one-jal", RAM + 0x18908, [nop, jal(0x801189c4), nop, brk], 0x801189c4)
    yield case("two-jal-then-break-then-more-jal-as-the-real-entry", RAM + 0x18908,
               [nop] * 35 + [jal(0x8015780c), nop, nop, nop, nop, jal(0x801189c4), nop, "0000004d", "00200000", jal(0x80150cd0)], 0x801189c4)
    yield case("break-without-an-earlier-jal", RAM + 0x1000, [nop, brk, jal(0x80170000)], None)
    yield case("no-break-at-all", RAM + 0x1000, [nop, jal(0x80170000), nop], None)
    yield case("jal-and-break-at-instruction-64", RAM, [jal(0x80123450)] + [nop] * 62 + [brk], 0x80123450)
    yield case("break-at-instruction-65-is-outside-the-window", RAM, [jal(0x80123450)] + [nop] * 63 + [brk], None)
    yield case("jal-at-instruction-64-without-break-before-it", RAM, [nop] * 62 + [brk, jal(0x80123450)], None)
    yield case("jal-after-the-break-is-ignored", RAM, [jal(0x80000100), brk, jal(0x80123450)], 0x80000100)
    yield case("other-jumps-are-not-jal", RAM, ["08000010", "0000f809", "03e00008", brk], None)
    yield case("target-keeps-the-segment-and-word-alignment", RAM, [jal(0x00000004), brk], 0x80000004)
    yield case("jal-target-is-the-26-bit-field-times-four", RAM, ["0c0000ff", brk], 0x800003fc)
    # pc0 outside RAM: refused (no result)
    status, lines = scan(0x00010000, [nop])
    yield "scan-pc0-outside-ram", None if status == 0 and lines == ["none"] else f"status {status}, lines {lines!r}"
    status, lines = scan(RAM + 0x200000 - 4 * 63, [nop])
    yield "scan-window-past-the-end-of-ram", None if status == 0 and lines == ["none"] else f"status {status}, lines {lines!r}"


def sha_cases(root: Path, prog: Path):
    d = root / "sha"
    d.mkdir()
    for n in (0, 1, 3, 55, 56, 57, 63, 64, 65, 119, 120, 127, 128, 129, 1000, 1 << 20):
        data = bytes((i * 31 + n) & 0xff for i in range(n))
        f = d / f"s{n}.bin"
        f.write_bytes(data)
        status, lines = run(prog, "sha", f)
        yield f"sha256-of-{n}-bytes-equals-hashlib", None if status == 0 and lines == [sha(data)] else f"status {status}, lines {lines!r}, wanted {sha(data)}"


def identity_cases(root: Path, prog: Path):
    d = root / "identity"
    d.mkdir()
    right = exe()
    img = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": right})
    path = d / "right.bin"
    img.write(path)
    yield "identity-matches", expect_load(prog, path, "SLPS_004.15", img.sector["SLPS_004.15;1"])
    other = bytearray(right)
    other[0x800 + 1500] ^= 1   # one byte of the text
    img2 = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": bytes(other)})
    path2 = d / "off.bin"
    img2.write(path2)
    status, lines = run(prog, "load", path2, sha(right))
    yield "identity-one-byte-off-is-refused-before-any-copy", None if status == 2 and lines[-2:] == [
        "ram: aa aa", "refused: the disc's program is not the one this build is for"] and not any(l.startswith("program:") for l in lines) else f"status {status}, lines {lines!r}"
    other = bytearray(right)
    other[0x10] ^= 1   # one byte of the header (the entry)
    img3 = Image({"SYSTEM.CNF;1": CNF, "SLPS_004.15;1": bytes(other)})
    path3 = d / "hdr.bin"
    img3.write(path3)
    status, lines = run(prog, "load", path3, sha(right))
    yield "identity-header-byte-off-is-refused", None if status == 2 and lines[-1].endswith("is not the one this build is for") else f"status {status}, lines {lines!r}"
    status, lines = run(prog, "load", path, "00" * 32)
    yield "identity-zero-pin-is-refused", None if status == 2 and lines[-1].endswith("is not the one this build is for") else f"status {status}, lines {lines!r}"
    # the directory record says more bytes than the image holds
    img.write(path, cut=(img.sector["SLPS_004.15;1"] + 1) * SECTOR + 30)
    status, lines = run(prog, "load", path, sha(right))
    yield "identity-file-shorter-than-its-record-is-refused", None if status == 2 and lines[-2] == "ram: aa aa" and "truncated" in lines[-1] else f"status {status}, lines {lines!r}"


def gate_cases(root: Path, cc: str):
    d = root / "gate"
    d.mkdir()
    (d / "gate.c").write_text(GATE_MAIN)
    out = d / "gate"
    proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-I", str(SRC), "-o", str(out), str(d / "gate.c"), str(SRC / "jumps.c")], capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        yield "gate-builds", proc.stderr
        return
    addresses = ["80100000", "80100020", "80100040", "801fffb0",   # resident, with C and without
                 "80100001", "8010001f", "80100021", "8010003f", "80100044",   # inside a function or just beside it
                 "80180000", "80180100",   # module images are not resident
                 "00000000", "7fffffff", "80200000", "801fffb4", "ffffffff"]
    status, lines = run(out, *addresses)
    first = (status, lines)
    proc = subprocess.run([cc, "-O1", "-DDUP", "-I", str(SRC), "-o", str(out) + "dup", str(d / "gate.c"), str(SRC / "jumps.c")], capture_output=True, text=True, timeout=120)
    status, lines = run(Path(str(out) + "dup"), "80100000")
    yield "gate-two-entries-at-one-address-refused-and-nothing-known", None if status == 1 and lines == ["build: jumps: two entries at 0x80100000"] else f"status {status}, lines {lines!r}"
    resident = {"80100000", "80100020", "80100040", "801fffb0"}
    want = [f"{a} {int(a in resident)}" for a in addresses]
    yield "gate-knows-exactly-the-resident-starts", None if first == (0, want) else f"status and lines {first!r}"


def groups(root: Path, prog: Path, cc: str):
    yield disc_cases(root, prog)
    yield list_cases(root, prog)
    yield exe_cases(root, prog)
    yield boot_cases(root, prog)
    yield scan_cases(root, prog)
    yield sha_cases(root, prog)
    yield identity_cases(root, prog)
    yield gate_cases(root, cc)


def main() -> int:
    cc = shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostrun-test-") as tmp:
        root = Path(tmp)
        prog = build_test(root, cc)
        for produced in groups(root, prog, cc):
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
