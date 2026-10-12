#!/usr/bin/env python3
"""Controls for the memory card of the port: port/src/memcard.c (the card image and its file system) and port/src/card.c (the BIOS calls on it),
built natively with the host's `cc` around small drivers of this file's own (a script on standard input drives each one, one command a line).

memcard.c, on made-up images: a blank card is "MC", 15 free entries with their checksums, the broken-frame list, the write-test frame
(the layout is the console documentation's, written out here byte by byte, never read back from the runtime); a card is formatted only with
"MC" and a right checksum; creating files of 1, 3 and 15 blocks (the lowest free blocks, the chain of next pointers, the states 0x51/0x52/0x53, the
size, the name, each entry's checksum, the data zero); refusals (a name that is there, an empty or too long name, 0 or 16 blocks, no room);
finding by name (the first 20 characters count); listing in block order with the patterns `*`, `X*`, `?`; reading and writing along a chain across block
borders, never past the file's size; deleted blocks (0xa1) count as free.

card.c, on a made-up game (the C below calls the host routines the way the BIOS calls are, with strings and buffers in its own memory) and a
recorder for DeliverEvent: with no cards (the setting off) the answers are the ones the port always gave (the software time-out for the requests and
for _card_clear, -1 for the descriptor calls, 0 for the list and the format); with a card file that is not there (a card that is new and not formatted): info
answers NEW, load answers NEW, open -1, firstfile 0; after format: the file is on disk, formatted, info still NEW until _card_clear (the hardware IOE), then IOE; create,
write, close and the file on disk after a restart (a second configure) holds the data; the list (firstfile, nextfile) fills the 40-byte entry (name, size, head) in block order;
open of a missing file, of a file for creating that exists, a read on a write-only descriptor, a write on a read-only one, a closed descriptor, reads and writes
past the end; sixteen files; two cards ("bu10:" is the second, absent unless given); a file of another size refused.

What is NOT tested here: the game's own card code (it runs in the game), other programs' reading of the card file (the format is the console's), and a Windows run.
It needs `cc`; without it this file says so and ends with status 2.
"""

from __future__ import annotations

import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"

MC_DRIVER = r'''
#include "memcard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned char img[MC_SIZE];
static void hex(const unsigned char *p, int n) { int i; for (i = 0; i < n; i++) printf("%02x", p[i]); }
int main(void)
{
    char line[256], cmd[32], a[64], b[64];
    while (fgets(line, sizeof line, stdin)) {
        int x, y, n;
        a[0] = b[0] = 0;
        n = sscanf(line, "%31s %63s %63s", cmd, a, b);
        if (n < 1) continue;
        if (!strcmp(cmd, "format")) mc_format(img);
        else if (!strcmp(cmd, "zero")) memset(img, 0, sizeof img);
        else if (!strcmp(cmd, "formatted")) printf("F %d\n", mc_is_formatted(img));
        else if (!strcmp(cmd, "frame")) { x = atoi(a); printf("R "); hex(img + x * MC_FRAME, MC_FRAME); printf("\n"); }
        else if (!strcmp(cmd, "poke")) { x = atoi(a); y = (int)strtol(b, 0, 16); img[x] = (unsigned char)y; }
        else if (!strcmp(cmd, "create")) printf("C %d\n", mc_create(img, a, (unsigned)atoi(b)));
        else if (!strcmp(cmd, "find")) printf("N %d\n", mc_find(img, a));
        else if (!strcmp(cmd, "free")) printf("B %d\n", mc_free_blocks(img));
        else if (!strcmp(cmd, "match")) printf("M %d\n", mc_match(a, b));
        else if (!strcmp(cmd, "list")) {
            int first = 0;
            printf("L");
            while ((first = mc_next(img, first, a)) != 0) { char nm[21]; mc_name(img, first, nm); printf(" %d:%s:%u", first, nm, mc_size(img, first)); }
            printf("\n");
        }
        else if (!strcmp(cmd, "delete")) { x = atoi(a); img[x * MC_FRAME] = 0xa1; }
        else if (!strcmp(cmd, "write")) {   /* write first offset count byte */
            int first, off, cnt, v; unsigned char *buf;
            if (sscanf(line, "%*s %d %d %d %x", &first, &off, &cnt, &v) != 4) return 64;
            buf = malloc((size_t)cnt); memset(buf, v, (size_t)cnt);
            printf("W %u\n", mc_write(img, first, (unsigned)off, buf, (unsigned)cnt)); free(buf);
        }
        else if (!strcmp(cmd, "read")) {    /* read first offset count */
            int first, off, cnt; unsigned char *buf; unsigned got, i, sum = 0;
            if (sscanf(line, "%*s %d %d %d", &first, &off, &cnt) != 3) return 64;
            buf = calloc(1, (size_t)cnt + 1);
            got = mc_read(img, first, (unsigned)off, buf, (unsigned)cnt);
            for (i = 0; i < got; i++) sum += buf[i];
            printf("D %u sum %u first %02x last %02x\n", got, sum, got ? buf[0] : 0, got ? buf[got - 1] : 0); free(buf);
        }
        else return 64;
        fflush(stdout);
    }
    return 0;
}
'''

CARD_DRIVER = r'''
#include "port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void port_deliver_event(unsigned c, unsigned s) { printf("E %08x %04x\n", c, s); }
int port_card_configure(int on, const char *p1, const char *p2, char *err, size_t n);
void port_card_flush(void);
long port_h_card_info(long), port_h_card_load(long), port_h_card_clear(long), port_h_card_format(const char *);
void *port_h_card_firstfile(const char *, void *), *port_h_card_nextfile(void *);
long port_h_card_open(const char *, long), port_h_card_read(long, void *, long), port_h_card_write(long, void *, long), port_h_card_close(long);
int main(void)
{
    char line[512], cmd[32], a[200], b[200];
    unsigned char entry[40], buf[70000];
    while (fgets(line, sizeof line, stdin)) {
        long x;
        a[0] = b[0] = 0;
        if (sscanf(line, "%31s %199s %199s", cmd, a, b) < 1) continue;
        if (!strcmp(cmd, "configure")) {   /* configure ON PATH1 [PATH2]; "-" is no path */
            char err[256] = "";
            int r = port_card_configure(atoi(a), strcmp(b, "-") ? b : NULL, NULL, err, sizeof err);
            printf("K %d %s\n", r, err);
        }
        else if (!strcmp(cmd, "configure2")) {   /* configure2 ON PATH1 PATH2 */
            char p2[200], err[256] = "";
            if (sscanf(line, "%*s %*s %*s %199s", p2) != 1) return 64;
            printf("K %d %s\n", port_card_configure(atoi(a), b, p2, err, sizeof err), err);
        }
        else if (!strcmp(cmd, "info")) printf("I %ld\n", port_h_card_info(strtol(a, 0, 0)));
        else if (!strcmp(cmd, "load")) printf("I %ld\n", port_h_card_load(strtol(a, 0, 0)));
        else if (!strcmp(cmd, "clear")) printf("I %ld\n", port_h_card_clear(strtol(a, 0, 0)));
        else if (!strcmp(cmd, "format")) printf("O %ld\n", port_h_card_format(a));
        else if (!strcmp(cmd, "first") || !strcmp(cmd, "next")) {
            void *r;
            memset(entry, 0xee, sizeof entry);
            r = !strcmp(cmd, "first") ? port_h_card_firstfile(a, entry) : port_h_card_nextfile(entry);
            if (!strcmp(cmd, "next")) { /* the entry of the last call is carried by the test through "entry" */ }
            printf("F %d", r == (void *)entry);
            if (r) { printf(" name=%s size=%u head=%u", entry, entry[0x18] | entry[0x19] << 8 | entry[0x1a] << 16 | (unsigned)entry[0x1b] << 24, entry[0x20]); }
            printf("\n");
        }
        else if (!strcmp(cmd, "listall")) {   /* firstfile(a), then nextfile on the same entry until none */
            void *r = port_h_card_firstfile(a, entry);
            printf("A");
            while (r) {
                printf(" %s/%u/%u", entry, entry[0x18] | entry[0x19] << 8, entry[0x20]);
                r = port_h_card_nextfile(entry);
            }
            printf("\n");
        }
        else if (!strcmp(cmd, "open")) { x = port_h_card_open(a, strtol(b, 0, 0)); printf("P %ld\n", x); }
        else if (!strcmp(cmd, "close")) printf("Q %ld\n", port_h_card_close(strtol(a, 0, 0)));
        else if (!strcmp(cmd, "write") || !strcmp(cmd, "read")) {   /* write FD N BYTE | read FD N */
            long fd, n, v = 0, r; unsigned i, sum = 0;
            if (sscanf(line, "%*s %ld %ld %lx", &fd, &n, &v) < 2 || n > (long)sizeof buf) return 64;
            if (!strcmp(cmd, "write")) { memset(buf, (int)v, (size_t)n); for (i = 0; i < (unsigned)n; i++) buf[i] = (unsigned char)(v + i); r = port_h_card_write(fd, buf, n); printf("W %ld\n", r); }
            else {
                memset(buf, 0xee, sizeof buf); r = port_h_card_read(fd, buf, n);
                for (i = 0; r > 0 && i < (unsigned)r; i++) sum += buf[i];
                printf("D %ld sum %u first %02x last %02x\n", r, sum, r > 0 ? buf[0] : 0, r > 0 ? buf[r - 1] : 0);
            }
        }
        else return 64;
        fflush(stdout);
    }
    return 0;
}
'''


def check(name: str, ok: bool, detail: str = ""):
    return name, None if ok else detail


def build(root: Path, cc: str, driver: str, name: str, sources: list[str]) -> Path:
    (root / f"{name}.c").write_text(driver)
    exe = root / name
    proc = subprocess.run([cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), str(root / f"{name}.c"), *[str(SRC / s) for s in sources], "-o", str(exe)],
                          capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        raise RuntimeError("does not build:\n" + proc.stderr[-1500:])
    return exe


def run(exe: Path, script: str) -> list[str]:
    proc = subprocess.run([str(exe)], input=script, capture_output=True, text=True, timeout=30)
    if proc.returncode != 0:
        raise RuntimeError(f"exit {proc.returncode}: {proc.stdout}{proc.stderr}")
    return proc.stdout.splitlines()


def xor(b: bytes) -> int:
    x = 0
    for v in b:
        x ^= v
    return x


def frame(first: bytes, n: int = 128, rest: int = 0) -> bytes:
    f = bytearray([rest]) * 0
    f = bytearray(first) + bytearray(n - 1 - len(first))
    f.append(xor(f))
    return bytes(f)


def mc_cases(root: Path, cc: str):
    exe = build(root, cc, MC_DRIVER, "mc", ["memcard.c"])

    def one(script: str) -> list[str]:
        return run(exe, script)

    blank = one("format\nframe 0\nframe 1\nframe 15\nframe 16\nframe 35\nframe 36\nframe 63\nformatted\n")
    free = frame(bytes([0xa0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff]))
    broken = frame(bytes([0xff] * 4 + [0] * 4 + [0xff, 0xff]))
    header = frame(b"MC")
    yield check("a blank card: frame 0 is MC and its checksum (0x0e)", blank[0] == "R " + header.hex() and header[127] == 0x0e, blank[0])
    yield check("a blank card: all 15 directory entries are free with their checksum (0xa0)", blank[1] == "R " + free.hex() and blank[2] == "R " + free.hex() and free[127] == 0xa0, blank[1])
    yield check("a blank card: the broken-frame list is 0xffffffff, zeros, 0xffff, zeros and a checksum", blank[3] == "R " + broken.hex() and blank[4] == "R " + broken.hex() and broken[127] == 0, blank[3])
    yield check("a blank card: frames 36 to 62 are zero and frame 63 repeats frame 0", blank[5] == "R " + ("00" * 128) and blank[6] == "R " + header.hex() and blank[7] == "F 1", f"{blank[5:]}")
    r = one("zero\nformatted\nformat\nformatted\npoke 5 11\nformatted\nformat\npoke 127 00\nformatted\nformat\npoke 1 00\nformatted\n")
    yield check("formatted: not for zeros, yes after format, not with a changed byte, not with a wrong checksum, not without the C", r == ["F 0", "F 1", "F 0", "F 0", "F 0"], f"{r}")
    r = one("format\ncreate GAME 1\nfind GAME\nfind NOPE\nfind GAMEX\nfree\nframe 1\n")
    first = frame(b"\x51\0\0\0" + (8192).to_bytes(4, "little") + b"\xff\xff" + b"GAME")
    yield check("a one-block file: block 1, found by name only, 14 blocks left, state 0x51, size 8192, next 0xffff, the name", r[:5] == ["C 1", "N 1", "N 0", "N 0", "B 14"] and r[5] == "R " + first.hex(), f"{r}")
    r = one("format\ncreate A 1\ncreate B 3\nframe 2\nframe 3\nframe 4\nfind B\nfree\n")
    e2 = frame(b"\x51\0\0\0" + (24576).to_bytes(4, "little") + b"\x02\x00" + b"B")      # next block 3 is stored as 2
    e3 = frame(b"\x52\0\0\0" + (0).to_bytes(4, "little") + b"\x03\x00")
    e4 = frame(b"\x53\0\0\0" + (0).to_bytes(4, "little") + b"\xff\xff")
    yield check("a three-block file: blocks 2, 3, 4, the states 0x51, 0x52, 0x53, the size in the first only, the pointers (a block's number less one), 11 left",
                r[0] == "C 1" and r[1] == "C 2" and r[2:5] == ["R " + e2.hex(), "R " + e3.hex(), "R " + e4.hex()] and r[5] == "N 2" and r[6] == "B 11", f"{r}")
    r = one("format\ncreate ALL 15\ncreate X 1\nfree\nlist *\nformat\ncreate Y 16\ncreate Y 0\ncreate \ncreate AAAAAAAAAAAAAAAAAAAAA 1\ncreate AAAAAAAAAAAAAAAAAAAA 1\nfind AAAAAAAAAAAAAAAAAAAAB\n")
    yield check("a card of 15 blocks in one file is full; 16 or 0 blocks, an empty name and a 21-character name are refused, 20 characters are fine and count in find",
                r == ["C 1", "C 0", "B 0", "L 1:ALL:122880", "C 0", "C 0", "C 0", "C 0", "C 1", "N 1"], f"{r}")
    r = one("format\ncreate A 1\ncreate A 1\ncreate B 1\nlist *\nlist A*\nlist ?\nlist B\nlist C*\nmatch AB* ABCD\nmatch A?C ABC\nmatch A?C ABCD\nmatch * \nmatch ABC AB\nmatch AB ABC\n")
    yield check("listing in block order, with patterns '*', 'A*', '?', a name, and the matcher's cases",
                r == ["C 1", "C 0", "C 2", "L 1:A:8192 2:B:8192", "L 1:A:8192", "L 1:A:8192 2:B:8192", "L 2:B:8192", "L", "M 1", "M 1", "M 0", "M 1", "M 0", "M 0"], f"{r}")
    r = one("format\ncreate F 3\nwrite 1 0 24576 7\nread 1 0 24576\nread 1 8190 4\nread 1 24570 100\nwrite 1 8191 2 99\nread 1 8190 4\nwrite 1 24576 4 1\nwrite 1 24570 20 5\nread 1 100 0\n")
    yield check("reading and writing across the blocks of a chain: the whole file, 4 bytes over a block border, past the end cut at the size, nothing at the end",
                r[1] == "W 24576" and r[2] == f"D 24576 sum {24576 * 7 & 0xffffffff} first 07 last 07" and r[3] == "D 4 sum 28 first 07 last 07" and r[4] == "D 6 sum 42 first 07 last 07"
                and r[5] == "W 2" and r[6] == f"D 4 sum {7 + 0x99 + 0x99 + 7} first 07 last 07" and r[7] == "W 0" and r[8] == "W 6" and r[9] == "D 0 sum 0 first 00 last 00", f"{r}")
    r = one("format\ncreate A 1\ncreate B 1\ndelete 1\nfree\ncreate C 1\nfind C\nlist *\n")
    yield check("a deleted block (0xa1) counts as free and the lowest free block is used again", r == ["C 1", "C 2", "B 14", "C 1", "N 1", "L 1:C:8192 2:B:8192"], f"{r}")
    r = one("format\ncreate A 1\ncreate B 1\ncreate C 1\npoke 128 52\nlist *\n")
    yield check("only an entry in state 0x51 starts a file in the list", r == ["C 1", "C 2", "C 3", "L 2:B:8192 3:C:8192"], f"{r}")


def card_cases(root: Path, cc: str):
    exe = build(root, cc, CARD_DRIVER, "card", ["card.c", "memcard.c"])
    d = root / "cards"
    d.mkdir()
    c1, c2 = d / "one.mcr", d / "two.mcr"
    SW, HW = "f4000001", "f0000011"

    def ev(c, s):
        return f"E {c} {s:04x}"

    def go(script: str) -> list[str]:
        return run(exe, script)

    # the setting off: the answers the port always gave
    r = go("configure 0 -\ninfo 0\nload 0\nclear 0\nopen bu00:X 1\nread 0 128\nwrite 0 128 1\nclose 0\nfirst bu00:*\nnext\nformat bu00:\n")
    yield check("setting off: the requests answer the software time-out and return 1; open, read, write, close -1; the list and the format 0",
                r == ["K 0 ", ev(SW, 0x100), "I 1", ev(SW, 0x100), "I 1", ev(SW, 0x100), "I 1", "P -1", "D -1 sum 0 first 00 last 00", "W -1", "Q -1", "F 0", "F 0", "O 0"], f"{r}")
    # a card file that is not there: new and not formatted
    r = go(f"configure 1 {c1}\ninfo 0\nload 0\nopen bu00:X 1\nfirst bu00:*\ninfo 0x10\nload 0x10\nclear 0x10\n")
    yield check("a card file that is not there: info NEW, load NEW (not formatted), open -1, the list 0; slot 2 empty: the software time-out, and clear the hardware one",
                r == ["K 0 ", ev(SW, 0x2000), "I 1", ev(SW, 0x2000), "I 1", "P -1", "F 0", ev(SW, 0x100), "I 1", ev(SW, 0x100), "I 1", ev(HW, 0x100), "I 1"] and not c1.exists(), f"{r} {c1.exists()}")
    # format: the file is written, info is still NEW until clear
    r = go(f"configure 1 {c1}\nformat bu00:\ninfo 0\nload 0\nclear 0\ninfo 0\ninfo 0\nload 0\nformat bu10:\n")
    yield check("format writes the card file; info is NEW until _card_clear (hardware IOE), then IOE; load is IOE; formatting the empty second slot gives 0",
                r == ["K 0 ", "O 1", ev(SW, 0x2000), "I 1", ev(SW, 0x4), "I 1", ev(HW, 0x4), "I 1", ev(SW, 0x4), "I 1", ev(SW, 0x4), "I 1", ev(SW, 0x4), "I 1", "O 0"]
                and c1.exists() and c1.stat().st_size == 131072 and c1.read_bytes()[:2] == b"MC", f"{r}")
    # create, write, close, and after a restart read back
    r = go(f"configure 1 {c1}\nopen bu00:BISLPS-00415-SFZ2SYS 1\nopen bu00:BISLPS-00415-SFZ2SYS 0x10200\nwrite 0 8192 1\nclose 0\nopen bu00:BISLPS-00415-SFZ2SYS 0x10200\n"
           f"open bu00:BISLPS-00415-SFZ2SYS 2\nwrite 0 8192 1\nwrite 0 128 1\nclose 0\nclose 0\n")
    yield check("create (0x10200 is FCREAT with one block) is refused when the file is there and the file is write-only; write fills the file, a write past its end does nothing; close twice: -1",
                r == ["K 0 ", "P -1", "P 0", "W 8192", "Q 0", "P -1", "P 0", "W 8192", "W 0", "Q 0", "Q -1"], f"{r}")
    r = go(f"configure 1 {c1}\nopen bu00:BISLPS-00415-SFZ2SYS 1\nread 0 8192\nread 0 128\nwrite 0 128 1\nclose 0\nfirst bu00:*\nnext\nlistall bu00:*\n")
    want_sum = sum((1 + i) & 0xff for i in range(8192))
    yield check("after a restart (a second configure) the file is there with its data: the read gives the 8192 bytes written, a second read the end, a write on a read-only descriptor -1",
                r[:2] == ["K 0 ", "P 0"] and r[2] == f"D 8192 sum {want_sum} first 01 last {(1 + 8191) & 0xff:02x}" and r[3] == "D 0 sum 0 first 00 last 00" and r[4] == "W -1" and r[5] == "Q 0", f"{r}")
    yield check("firstfile fills the 40-byte entry (name, size 8192, head 1) and returns its address; nextfile after the only file returns 0",
                r[6] == "F 1 name=BISLPS-00415-SFZ2SYS size=8192 head=1" and r[7] == "F 0" and r[8] == "A BISLPS-00415-SFZ2SYS/8192/1", f"{r[6:]}")
    # a list of several files, in block order, with a pattern
    r = go(f"configure 1 {c1}\nopen bu00:ZED 0x20200\nclose 0\nopen bu00:ABC 0x10200\nclose 0\nlistall bu00:*\nlistall bu00:A*\nlistall bu00:?ED\nlistall bu00:NONE\nlistall bu00:\nopen bu00:ZED 1\nread 0 24576\n")
    yield check("the list is in block order and takes patterns ('*', 'A*', '?ED', no match); a missing pattern lists everything; a two-block file is 16384 bytes",
                r[5:10] == ["A BISLPS-00415-SFZ2SYS/8192/1 ZED/16384/2 ABC/8192/4", "A ABC/8192/4", "A ZED/16384/2", "A", "A BISLPS-00415-SFZ2SYS/8192/1 ZED/16384/2 ABC/8192/4"]
                and r[10] == "P 0" and r[11] == "D 16384 sum 0 first 00 last 00", f"{r}")
    # other refusals: no room, a file of another size
    r = go(f"configure 1 {c1}\nformat bu00:\n" + "".join(f"open bu00:F{i} 0x10200\nclose 0\n" for i in range(16)) + "listall bu00:*\n")
    opens = [x for x in r if x.startswith("P ")]
    yield check("fifteen one-block files fill the card; the sixteenth cannot be made (and it is not listed)",
                opens == ["P 0"] * 15 + ["P -1"] and r[-1].count("/") == 30 and "F15" not in r[-1], f"{opens} {r[-1][:80]}")
    bad = d / "bad.mcr"
    bad.write_bytes(b"x" * 1000)
    r = go(f"configure 1 {bad}\n")
    yield check("a card file of another size is refused with a line that names it", r[0].startswith("K -1 ") and "bad.mcr" in r[0] and bad.read_bytes() == b"x" * 1000, f"{r}")
    # two cards
    r = go(f"configure2 1 {c1} {c2}\nformat bu10:\ninfo 0x10\nclear 0x10\nopen bu10:ONLY2 0x10200\nclose 0\nlistall bu00:*\nlistall bu10:*\n")
    yield check("two cards: bu10: is the second file, its files are not on the first, and the hardware clear answers IOE for it",
                r[:6] == ["K 0 ", "O 1", ev(SW, 0x2000), "I 1", ev(HW, 0x4), "I 1"] and r[6] == "P 0" and r[8].startswith("A ") and "ONLY2" not in r[8] and r[9] == "A ONLY2/8192/1" and c2.exists(), f"{r}")


def main() -> int:
    global SRC
    if len(sys.argv) == 3 and sys.argv[1] == "--src":
        SRC = Path(sys.argv[2]).resolve()
    elif len(sys.argv) != 1:
        print("usage: test_hostmemcard.py [--src DIR]")
        return 2
    cc = shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    failed = 0
    with tempfile.TemporaryDirectory(prefix="hostmemcard-test-") as tmp:
        root = Path(tmp)
        for group in (mc_cases, card_cases):
            for name, detail in group(root, cc):
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
