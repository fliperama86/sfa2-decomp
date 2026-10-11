#!/usr/bin/env python3
"""Controls for the PS1's I/O page (iopage.c, and its part of mirror.c).

    python3 test_hostiopage.py --cc CROSS_CC [--psyz-build DIR] [--run PREFIX] [--src DIR] [--only SUBSTRING]

The page [0x1F801000, 0x1F802000) is closed at start. The game's thread, from the game's code, reaches it through the
mirror layer's fault handler, which carries each instruction out on a temporary value; three addresses have a model
(plain 32-bit cells); everything else ends the program with the line
`stop: crash: the game used the PS1's I/O page at 0xADDRESS (read|write) in NAME REASON: BYTES`.

The cases build the runtime with made-up game code (as test_hostmirror.py does, whose rig and helpers this file imports) and
run the whole program. Each case lists the lines it must print, written out here, and the status it must end with; a program
that prints its lines and then dies, ends with another status, prints a line more or fewer, or never gets to its line is a
failed case, never a pass (the first group of cases shows that the check says so). The groups:

- the page is closed (asked of the system) and the byte before and after it is plain memory;
- each of the three cells: 0 at start, written and read back, `or`, `and`, `add` to memory, `cmp` and `test` with the
  flags the game then reads, the branches of a compare;
- the stops, each with the instruction's own bytes: 16-bit and 8-bit reads and writes of a cell, a word two bytes into a
  cell, the bytes of the page with no model (the first, the last, one just before a cell, the chip's and the DMA's), a form
  that is not served, a word that straddles an end of the page, an execute fault, an access from the runtime's own code and
  from a second thread;
- the mirror beside it: a served read and write at a low address give the mirror's lines, in the same program as a cell,
  and the mirror's own crash line is the old one;
- --trace: three lines at exit: the one that says the page is closed, the faults served with the reads and writes of each cell, and
  the counts of the chip, DMA and the completions.

With --psyz-build DIR (a build folder of psyzbuild.py) the runtime's iopage.c is compiled for PsyZ, every program is linked with PsyZ,
and 45 more cases run (the chip, DMA channel 4, the completion of a transfer, the services); the two made-up routines that stand where
a mixer would be pull frames out of PsyZ's model under its audio lock and read a byte of the chip's RAM; no audio device is opened:
a program linked with PsyZ opens the machine's sound output unless told not to, so the rig starts every program with --no-audio, and
with SDL's dummy audio driver in its environment (SDL_AUDIODRIVER, under WSL through WSLENV).
Without it those cases are listed as `NOT RUN NAME`, the last line says how many, and the status is 3. The cases of the chip and DMA
addresses in a build without PsyZ (they have no model there) run only without it. The groups with PsyZ:

- the chip: a write and a read of plain registers (16 bits), the two key-on registers (a voice with a sample that the game put into
  the chip's RAM is silent before the key-on and sounds after it: the pull gives frames that are not all zero), the status register's
  mirror of the control's low bits, and the stops (32 and 8 bits, an odd address, the bytes either side of the chip);
- DMA: the three registers' read-back and bit 24, a transfer to the chip and back with the bytes equal, an address with the top byte
  0, a span that ends exactly at the end of RAM and one of exactly the chip's 512 KB, and each refusal with its exact line (an address
  outside RAM, a span that ends outside, a span whose size wraps in 32 bits, the largest block control, a top byte that is neither
  00 nor 80, no blocks, no words, more than the chip's RAM, a second start while the completion is pending);
- the completion: once per transfer; at the first poll with no wait (with and without the timer); by the timer when the game only
  spins, and not before the due time (the measured delay is printed, as `measured:`, and the bounds are the due time and 500 ms);
  by the vblank's delivery before the vblank's handlers; both due, by a poll and by the timer, the completion first; held back inside
  a critical section and delivered once after it; not inside a handler; dropped and counted with no routine; the routine's address
  refused at the call, by a poll and by the timer;
- the services: DMACallback's previous routine, an event delivered and seen once, WaitEvent after a delivery;
- the counts in the trace, written out by hand, and the completions taken by each way (`--no-interrupt` where only a poll can take it).

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases that ran behaved as required` or the number of cases
that did not. --src names another copy of the runtime's folder (the mutants of the controls run on a scratch copy).
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostlaunch as L  # noqa: E402
from test_hostlaunch import ABS_BASE, FUN_BASE, PROLOGUE, RAM, Variant, program, verdict  # noqa: E402

CELLS = [0x1F801014, 0x1F8010F0, 0x1F8010F4]
NAMES: list[str] = []
BODIES: list[str] = []
PNAMES: list[str] = []    # the functions of the cases that need PsyZ (--psyz-build)
PBODIES: list[str] = []


def func(name: str, body: str, psyz: bool = False) -> str:
    (PNAMES if psyz else NAMES).append(name)
    (PBODIES if psyz else BODIES).append("void %s(void)\n{\n%s\n}\n" % (name, body))
    return name


# ---- the made-up game ----
R32 = "(*(volatile unsigned *)(%s))"
HEADER = PROLOGUE + r"""
extern unsigned host_io_read(void);
#define W32(a, v) (*(volatile unsigned *)(a) = (v))
#define R32(a) (*(volatile unsigned *)(a))
#define LOW32(a) (*(volatile unsigned *)(a))
#define LOW8(a) (*(volatile unsigned char *)(a))
"""
CASES: list[dict] = []   # name, function, lines (before the end), end: None (main returned) or a regex of the stop line, status, args


def case(name, fn, lines, stop=None, status=None, args=None, after=None, psyz=False, counts=None, no_psyz=False, periodic=None):
    """counts: a regex that the second `iopage:` line of the trace (the chip's, the DMA's and the completions' counts) must match as a whole."""
    CASES.append(dict(name=name, fn=fn, lines=lines, stop=stop, status=status if status is not None else (0 if stop is None else 10), args=args or [], after=after, psyz=psyz, counts=counts, no_psyz=no_psyz, periodic=periodic))


def le(addr: int) -> str:
    return " ".join("%02x" % b for b in struct.pack("<I", addr))


def hx(addr: int) -> str:
    return "0x%08x" % addr


# the page is closed: ask the system
func("i_closed", r'''
    MEMORY_BASIC_INFORMATION m;
    VirtualQuery((void *)0x1f801000u, &m, sizeof m);
    SAY("page protection %lx, size %lx\n", (unsigned long)m.Protect, (unsigned long)m.RegionSize);
    VirtualQuery((void *)0x1f800000u, &m, sizeof m);
    SAY("scratchpad protection %lx\n", (unsigned long)m.Protect);
    VirtualQuery((void *)0x1f802000u, &m, sizeof m);
    SAY("after the page protection %lx\n", (unsigned long)m.Protect);''')
case("the-io-page-is-closed-at-start-and-the-bytes-around-it-are-not", "i_closed",
     ["page protection 1, size 1000", "scratchpad protection 40", "after the page protection 40"])

# plain memory just outside the page, as before
func("i_outside", r'''
    LOW8(0x1f800fffu) = 0x5a;
    LOW8(0x1f802000u) = 0xa5;
    LOW32(0x1f800ffcu) = 0x01020304u;
    LOW32(0x1f802000u) = 0x0a0b0c0du;
    SAY("outside %02x %02x %08x %08x\n", LOW8(0x1f800fffu), LOW8(0x1f802000u), LOW32(0x1f800ffcu), LOW32(0x1f802000u));''')
case("the-byte-before-the-page-and-the-byte-after-it-are-plain-memory", "i_outside", ["outside 01 0d 01020304 0a0b0c0d"])

for n, c in enumerate(CELLS):
    a = hx(c)
    func("i_cell%d" % n, r'''
    SAY("at start %%08x\n", R32(%(a)s));
    W32(%(a)s, 0x12345678u);
    SAY("written %%08x\n", R32(%(a)s));
    W32(%(a)s, 0u);
    SAY("zero %%08x\n", R32(%(a)s));
    W32(%(a)s, 0xffffffffu);
    SAY("all ones %%08x\n", R32(%(a)s));''' % dict(a=a))
    case("cell-%s-reads-0-at-start-and-reads-back-what-was-written" % a, "i_cell%d" % n,
         ["at start 00000000", "written 12345678", "zero 00000000", "all ones ffffffff"])

    func("i_rmw%d" % n, r'''
    unsigned e;
    W32(%(a)s, 0xf0f0u);
    __asm__ volatile("orl $0x0f, %(a)s" : : : "memory", "cc");
    SAY("or %%08x\n", R32(%(a)s));
    __asm__ volatile("andl $0x0ff0, %(a)s" : : : "memory", "cc");
    SAY("and %%08x\n", R32(%(a)s));
    __asm__ volatile("addl $0x20, %(a)s" : : : "memory", "cc");
    SAY("add %%08x\n", R32(%(a)s));
    W32(%(a)s, 0xfffffffeu);
    __asm__ volatile("addl $2, %(a)s\n\tsetc %%%%al" : "=a"(e) : : "memory", "cc");
    SAY("add wraps %%08x carry %%u\n", R32(%(a)s), e & 1);
    W32(%(a)s, 0x1234u);
    R32(%(a)s) |= 0x00f0u;
    SAY("c or %%08x\n", R32(%(a)s));
    R32(%(a)s) &= 0xff0fu;
    SAY("c and %%08x\n", R32(%(a)s));
    R32(%(a)s) += 0x1111u;
    SAY("c add %%08x\n", R32(%(a)s));
    __asm__ volatile("subl $0x15, %(a)s\n\txorl $0xff, %(a)s" : : : "memory", "cc");
    SAY("sub xor %%08x\n", R32(%(a)s));''' % dict(a=a))
    case("cell-%s-or-and-add-sub-xor-to-memory-give-the-right-value" % a, "i_rmw%d" % n,
         ["or 0000f0ff", "and 000000f0", "add 00000110", "add wraps 00000000 carry 1", "c or 000012f4", "c and 00001204", "c add 00002315", "sub xor 000023ff"])

    func("i_flags%d" % n, r'''
    unsigned eq, ne, below, t1, t0, rm;
    W32(%(a)s, 5u);
    __asm__ volatile("cmpl $5, %(a)s\n\tsete %%%%al" : "=a"(eq) : : "memory", "cc");
    __asm__ volatile("cmpl $6, %(a)s\n\tsete %%%%al" : "=a"(ne) : : "memory", "cc");
    __asm__ volatile("cmpl $6, %(a)s\n\tsetb %%%%al" : "=a"(below) : : "memory", "cc");
    SAY("cmp 5: equal %%u, 6: equal %%u, below %%u, cell %%08x\n", eq & 1, ne & 1, below & 1, R32(%(a)s));
    __asm__ volatile("testl $4, %(a)s\n\tsetnz %%%%al" : "=a"(t1) : : "memory", "cc");
    __asm__ volatile("testl $2, %(a)s\n\tsetnz %%%%al" : "=a"(t0) : : "memory", "cc");
    SAY("test 4: %%u, test 2: %%u, cell %%08x\n", t1 & 1, t0 & 1, R32(%(a)s));
    __asm__ volatile("movl $3, %%%%ebx\n\txorl %%%%eax, %%%%eax\n\tcmpl %(a)s, %%%%ebx\n\tsetb %%%%al" : "=a"(rm) : : "ebx", "memory", "cc");
    SAY("cmp r,m: 3 below 5: %%u, cell %%08x\n", rm & 1, R32(%(a)s));
    if (R32(%(a)s) == 5u) SAY("branch: equal\n"); else SAY("branch: not equal\n");
    W32(%(a)s, 7u);
    if (R32(%(a)s) == 5u) SAY("branch: equal\n"); else SAY("branch: not equal\n");
    if (R32(%(a)s) & 2u) SAY("branch: bit 1 set\n"); else SAY("branch: bit 1 clear\n");
    W32(%(a)s, 4u);
    if (R32(%(a)s) & 2u) SAY("branch: bit 1 set\n"); else SAY("branch: bit 1 clear\n");''' % dict(a=a))
    case("cell-%s-compare-and-test-set-the-flags-the-game-branches-on" % a, "i_flags%d" % n,
         ["cmp 5: equal 1, 6: equal 0, below 1, cell 00000005", "test 4: 1, test 2: 0, cell 00000005", "cmp r,m: 3 below 5: 1, cell 00000005",
          "branch: equal", "branch: not equal", "branch: bit 1 set", "branch: bit 1 clear"])

# the stops. Each prints a value first, so that a program that never got to its access is a failed case.
STOP = r"stop: crash: the game used the PS1's I/O page at %s \((read|write)\) in %s %s: %s( [0-9a-f]{2})*"


def stop_re(addr, kind, name, reason, prefix):
    return r"stop: crash: the game used the PS1's I/O page at %s \(%s\) in %s %s: %s( [0-9a-f]{2})*" % (hx(addr), kind, name, re.escape(reason), prefix)


def width_reason(n):
    return "with an access of %d bytes on a register that takes 4" % n


for n, c in enumerate(CELLS):
    a = hx(c)
    forms = [
        ("r16", "read", 2, '__asm__ volatile("movzwl %s, %%%%eax" : : : "eax", "memory");' % a, "0f b7 05 " + le(c)),
        ("r8", "read", 1, '__asm__ volatile("movzbl %s, %%%%eax" : : : "eax", "memory");' % a, "0f b6 05 " + le(c)),
        ("w16", "write", 2, '__asm__ volatile("movw $0xbeef, %s" : : : "memory");' % a, "66 c7 05 " + le(c) + " ef be"),
        ("w8", "write", 1, '__asm__ volatile("movb $0x12, %s" : : : "memory");' % a, "c6 05 " + le(c) + " 12"),
    ]
    for tag, kind, size, asm, prefix in forms:
        fn = func("i_%s_%d" % (tag, n), 'W32(%s, 0x11223344u);\n    SAY("before %%08x\\n", R32(%s));\n    %s\n    SAY("after\\n");' % (a, a, asm))
        case("cell-%s-%d-bit-%s-ends-the-program-with-the-stop-line-and-the-instructions-bytes" % (a, size * 8, kind), fn, ["before 11223344"],
             stop_re(c, kind, fn, width_reason(size), prefix))
    off = c + 2
    fn = func("i_offr_%d" % n, 'W32(%s, 0x11223344u);\n    SAY("before %%08x\\n", R32(%s));\n    __asm__ volatile("movl %s, %%%%ebx" : : : "ebx", "memory");\n    SAY("after\\n");' % (a, a, hx(off)))
    case("a-word-read-two-bytes-into-cell-%s-ends-the-program" % a, fn, ["before 11223344"],
         stop_re(off, "read", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(off), "8b 1d " + le(off)))
    fn = func("i_offw_%d" % n, 'W32(%s, 0x11223344u);\n    SAY("before %%08x\\n", R32(%s));\n    __asm__ volatile("movl $1, %s" : : : "memory");\n    SAY("after\\n");' % (a, a, hx(off)))
    case("a-word-written-two-bytes-into-cell-%s-ends-the-program" % a, fn, ["before 11223344"],
         stop_re(off, "write", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(off), "c7 05 " + le(off) + " 01 00 00 00"))

# addresses with no model: the first and the last byte, one before a cell, the chip's, the DMA's
for addr, label in [(0x1F801000, "the-first-byte-of-the-page"), (0x1F801010, "0x1f801010-just-before-a-cell"), (0x1F801018, "0x1f801018-just-after-a-cell"),
                    (0x1F8010C0, "the-first-dma-register"), (0x1F801C00, "the-first-register-of-the-sound-chip"), (0x1F801FFC, "the-last-word-of-the-page")]:
    # the DMA and the chip have models only in a build with PsyZ: the cases of those two addresses are for the build without it
    # (the build with it has cases of its own for them below)
    no_psyz_only = addr in (0x1F8010C0, 0x1F801C00)
    fn = func("i_nomr_%x" % addr, 'SAY("before\\n");\n    __asm__ volatile("movl %s, %%%%ebx" : : : "ebx", "memory");\n    SAY("after\\n");' % hx(addr))
    case("a-read-at-%s-has-no-model-and-ends-the-program" % label, fn, ["before"],
         stop_re(addr, "read", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(addr), "8b 1d " + le(addr)), no_psyz=no_psyz_only)
    fn = func("i_nomw_%x" % addr, 'SAY("before\\n");\n    __asm__ volatile("movl $1, %s" : : : "memory");\n    SAY("after\\n");' % hx(addr))
    case("a-write-at-%s-has-no-model-and-ends-the-program" % label, fn, ["before"],
         stop_re(addr, "write", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(addr), "c7 05 " + le(addr) + " 01 00 00 00"), no_psyz=no_psyz_only)
fn = func("i_last_r", 'SAY("before\\n");\n    __asm__ volatile("movzbl 0x1f801fff, %%eax" : : : "eax", "memory");\n    SAY("after\\n");')
case("a-byte-read-at-the-last-byte-of-the-page-has-no-model-and-ends-the-program", fn, ["before"],
     stop_re(0x1F801FFF, "read", fn, "with an address that no model of the page claims (0x1f801fff, 1 bytes)", "0f b6 05 " + le(0x1F801FFF)))
fn = func("i_last_w", 'SAY("before\\n");\n    __asm__ volatile("movb $1, 0x1f801fff" : : : "memory");\n    SAY("after\\n");')
case("a-byte-written-at-the-last-byte-of-the-page-has-no-model-and-ends-the-program", fn, ["before"],
     stop_re(0x1F801FFF, "write", fn, "with an address that no model of the page claims (0x1f801fff, 1 bytes)", "c6 05 " + le(0x1F801FFF) + " 01"))
fn = func("i_first_w", 'SAY("before\\n");\n    __asm__ volatile("movb $1, 0x1f801000" : : : "memory");\n    SAY("after\\n");')
case("a-byte-written-at-the-first-byte-of-the-page-has-no-model-and-ends-the-program", fn, ["before"],
     stop_re(0x1F801000, "write", fn, "with an address that no model of the page claims (0x1f801000, 1 bytes)", "c6 05 " + le(0x1F801000) + " 01"))

# a word that straddles an end of the page
fn = func("i_straddle_lo", 'SAY("before\\n");\n    __asm__ volatile("movl 0x1f800ffe, %%ebx" : : : "ebx", "memory");\n    SAY("after\\n");')
case("a-word-that-begins-before-the-page-and-ends-inside-it-ends-the-program", fn, ["before"],
     stop_re(0x1F801000, "read", fn, "with an instruction whose operand is at 0x1f800ffe, not there", "8b 1d " + le(0x1F800FFE)))
fn = func("i_straddle_hi", 'SAY("before\\n");\n    __asm__ volatile("movl $1, 0x1f801ffe" : : : "memory");\n    SAY("after\\n");')
case("a-word-that-begins-inside-the-page-and-ends-after-it-ends-the-program", fn, ["before"],
     stop_re(0x1F801FFE, "write", fn, "with an access that straddles the end of the page", "c7 05 " + le(0x1F801FFE) + " 01 00 00 00"))

# a form that is not served, on a cell
fn = func("i_unserved", 'SAY("before\\n");\n    __asm__ volatile("negl 0x1f801014" : : : "memory", "cc");\n    SAY("after\\n");')
case("a-form-that-is-not-served-on-a-cell-ends-the-program-with-the-instructions-bytes", fn, ["before"],
     stop_re(0x1F801014, "(read|write)", fn, "with an instruction the port does not serve yet", "f7 1d " + le(0x1F801014)))

# an execute fault, the runtime's own code, a second thread
NOT_GAME = "from a thread or code that is not the game's"
fn = func("i_exec", 'SAY("before\\n");\n    ((void (*)(void))0x1f801014u)();\n    SAY("after\\n");')
case("an-execute-fault-in-the-page-ends-the-program", fn, ["before"], r"stop: crash: the game used the PS1's I/O page at 0x1f801014 \(execute\) in \? %s: " % re.escape(NOT_GAME))
fn = func("i_host", 'SAY("before\\n");\n    SAY("read %u\\n", host_io_read());')
case("a-read-from-the-runtimes-own-code-ends-the-program", fn, ["before"], r"stop: crash: the game used the PS1's I/O page at 0x1f801014 \(read\) in \? %s: " % re.escape(NOT_GAME))
THREAD_FN = r'''
static DWORD WINAPI thread_fn(LPVOID p)
{
    (void)p;
    __asm__ volatile("movl 0x1f801014, %%ebx" : : : "ebx", "memory");
    return 0;
}
'''
fn = func("i_thread", 'HANDLE h;\n    SAY("before\\n");\n    h = CreateThread(NULL, 0, thread_fn, NULL, 0, NULL);\n    if (!h) SAY("thread: CreateThread failed, error %lu\\n", (unsigned long)GetLastError());\n    else SAY("thread: wait returned %lu\\n", (unsigned long)WaitForSingleObject(h, 20000));\n    SAY("after\\n");')
case("a-read-from-a-second-thread-in-the-games-code-ends-the-program", fn, ["before"], r"stop: crash: the game used the PS1's I/O page at 0x1f801014 \(read\) in \S+ %s: [0-9a-f]{2}( [0-9a-f]{2})*" % re.escape(NOT_GAME))
fn = func("i_thread_w", 'HANDLE h;\n    SAY("before\\n");\n    h = CreateThread(NULL, 0, thread_wfn, NULL, 0, NULL);\n    if (!h) SAY("thread: CreateThread failed, error %lu\\n", (unsigned long)GetLastError());\n    else SAY("thread: wait returned %lu\\n", (unsigned long)WaitForSingleObject(h, 20000));\n    SAY("after\\n");')
THREAD_FN += r'''
static DWORD WINAPI thread_wfn(LPVOID p)
{
    (void)p;
    __asm__ volatile("movl $1, 0x1f801014" : : : "memory");
    return 0;
}
'''
case("a-write-from-a-second-thread-in-the-games-code-ends-the-program", fn, ["before"], r"stop: crash: the game used the PS1's I/O page at 0x1f801014 \(write\) in \S+ %s: [0-9a-f]{2}( [0-9a-f]{2})*" % re.escape(NOT_GAME))

# the mirror beside it
func("i_mirror", r'''
    LOW32(0x2000) = 0x600d600du;
    W32(0x1f801014, 0xabcdu);
    SAY("low %08x cell %08x\n", LOW32(0x2000), R32(0x1f801014));
    SAY("in the RAM %08x\n", LOW32(0x80002000u));
    LOW8(0x2004) = 0x77;
    SAY("low byte %02x\n", LOW8(0x2004));''')
case("the-mirror-still-serves-a-low-read-and-write-beside-a-cell", "i_mirror",
     ["mirror: i_mirror uses the PS1's copy of RAM at address 0 (first at 0x00002000; write)", "low 600d600d cell 0000abcd", "in the RAM 600d600d", "low byte 77"])
fn = func("i_mstraddle", 'SAY("before\\n");\n    SAY("read %u\\n", LOW32(0xfffe));')
case("the-mirrors-own-crash-line-is-unchanged", fn, ["before"], r"stop: crash: the game used the PS1's RAM mirror at 0x0000fffe \(read\) in i_mstraddle \(at 0x[0-9a-f]{8}\)")

# --trace
func("i_trace", r'''
    SAY("%08x\n", R32(0x1f801014));
    W32(0x1f801014, 1u);
    SAY("%08x\n", R32(0x1f801014));
    W32(0x1f8010f0, 2u);
    __asm__ volatile("orl $4, 0x1f8010f4" : : : "memory", "cc");
    __asm__ volatile("cmpl $1, 0x1f8010f0\n\ttestl $1, 0x1f8010f0" : : : "memory", "cc");''')
case("with-trace-the-page-is-reported-closed-and-the-counts-are-one-line-at-exit", "i_trace", ["00000000", "00000001"])

UNREG = L.UNREGISTERED

# ================= the sound chip, DMA channel 4 and the completion of a transfer (they need PsyZ: --psyz-build) =================
#
# The game's code is the made-up game's, the library routines it calls are the runtime's own (kernel.c, iopage.c) behind the
# names of PSYZ_ABS, and two tiny routines of the made-up runtime (PSYZ_DOMAINS) stand where a mixer would be: one pulls frames
# out of PsyZ's model of the chip under its audio lock and counts the samples that are not zero, the other reads a byte of the
# chip's RAM. No audio device is opened.
PSYZ_HEADER = r"""
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_TestEvent(unsigned), ps1_WaitEvent(unsigned), ps1_StartRCnt(unsigned), ps1_VSync(int);
extern int ps1_EnterCriticalSection(void), ps1_ExitCriticalSection(void), ps1_pull_nonzero(int), ps1_chip_ram(int);
extern void ps1_DeliverEvent(unsigned, unsigned);
extern void *ps1_DMACallback(int, void (*)(void)), *ps1_ResetCallback(void);
#define W16(a, v) (*(volatile unsigned short *)(a) = (v))
#define R16(a) (*(volatile unsigned short *)(a))
#define BUF_A ((volatile unsigned char *)0x80180000u)
#define BUF_B ((volatile unsigned char *)0x80181000u)
#define TO_CHIP 0x01000201u
#define FROM_CHIP 0x01000200u
static volatile int dma_count, vb_count;
static volatile char order[16];
static volatile int order_n;
static void note(char c) { if (order_n < 15) { order[order_n++] = c; order[order_n] = 0; } }
static unsigned us_now(void)
{
    LARGE_INTEGER f, c;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return (unsigned)((unsigned long long)c.QuadPart * 1000000ull / (unsigned long long)f.QuadPart);
}
/* a busy wait of the game's code that makes no call (the timer interrupts it as it would the console's code); gives up after a count */
#define SPIN_UNTIL(cond) do { unsigned long n_ = 0; while (!(cond) && n_++ < 1500000000ul) { } } while (0)
static void dma_routine(void) { dma_count++; }
static void dma_note(void) { dma_count++; note('D'); }
static void vb_handler(void) { vb_count++; }
static void vb_note(void) { vb_count++; note('V'); }
static void start_dma(unsigned ram, unsigned bytes, unsigned control)
{
    W32(0x1f8010c0u, ram);
    W32(0x1f8010c4u, ((bytes / 64u) << 16) | 16u);
    W32(0x1f8010c8u, control);
}
static void wait_dma(int n) { while (dma_count < n) ps1_VSync(-1); }
static void vblank_on(void (*h)(void))
{
    unsigned ev;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, h);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
}
static void put_sample(unsigned chip_addr)
{
    int i;
    W16(0x1f801da6u, chip_addr >> 3);
    W16(0x1f801da8u, 0x0000);                 /* the block's header word: shift 12 (the nibbles count as they are), no flags */
    for (i = 0; i < 7; i++) W16(0x1f801da8u, 0x7777);
}
"""

PSYZ_NAMES = ["OpenEvent", "EnableEvent", "TestEvent", "WaitEvent", "DeliverEvent", "DMACallback", "EnterCriticalSection", "ExitCriticalSection",
              "ResetCallback", "StartRCnt", "VSync", "pull_nonzero", "chip_ram"]
PSYZ_ABS = [(n, RAM + 0x101e00 + 0x10 * i, 1) for i, n in enumerate(PSYZ_NAMES)]
PSYZ_HOSTS = {"OpenEvent": "port_h_OpenEvent", "EnableEvent": "port_h_EnableEvent", "TestEvent": "port_h_TestEvent", "WaitEvent": "port_h_WaitEvent",
              "DeliverEvent": "port_h_DeliverEvent", "DMACallback": "port_h_DMACallback", "EnterCriticalSection": "port_h_EnterCriticalSection",
              "ExitCriticalSection": "port_h_ExitCriticalSection", "ResetCallback": "port_h_ResetCallback", "StartRCnt": "port_h_StartRCnt",
              "VSync": "port_h_VSync", "pull_nonzero": "host_pull_nonzero", "chip_ram": "host_chip_ram"}


def psyz_domains() -> str:
    decls = "".join(f"extern void {h}();\n" for h in PSYZ_HOSTS.values() if not h.startswith("host_") and h != "port_h_DMACallback")
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in PSYZ_HOSTS.items())
    return f"""
#include "port.h"
#include <psyz.h>
#include <psyz/spu.h>
#include <psyz/audio.h>
{decls}
int host_pull_nonzero(int frames)
{{
    static short buf[2 * 4096];
    int i, n = 0;
    Psyz_AudioLock();
    Psyz_SpuPullSamples(buf, frames);
    Psyz_AudioUnlock();
    for (i = 0; i < frames * 2; i++) if (buf[i]) n++;
    return n;
}}
int host_chip_ram(int offset) {{ return Psyz_SpuGetRam()[offset]; }}
unsigned host_io_read(void) {{ return *(volatile unsigned *)0x1f801014; }}
static const struct port_library t[] = {{ {rows} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "kernel", t }} }};
const unsigned port_domain_count = 1;
static const struct port_override o[] = {{ {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


def pfunc(name: str, body: str) -> str:
    return func(name, body, psyz=True)


def pcase(name, fn, lines, stop=None, status=None, args=None, counts=None, periodic=None):
    case(name, fn, lines, stop, status, args, psyz=True, counts=counts, periodic=periodic)


COMP = r"completions %s taken \(poll %s, timer %s, vblank %s\), %s dropped without a routine"


def comp(taken, poll, timer, vblank, dropped=0, transfers=None, to_chip=None, from_chip=None):
    """The regex for the second `iopage:` line of the trace: the completions are written out, the accesses are counted by `\\d+`."""
    t = r"\d+" if transfers is None else str(transfers)
    a = r"\d+" if to_chip is None else str(to_chip)
    b = r"\d+" if from_chip is None else str(from_chip)
    return r"iopage: chip \d+ reads \d+ writes; dma \d+ reads \d+ writes; %s transfers, %s bytes to the chip, %s bytes from the chip; " % (t, a, b) + COMP % (taken, poll, timer, vblank, dropped)


# ---- the chip ----
pfunc("p_chip_plain", r'''
    SAY("at start %04x %04x\n", R16(0x1f801d84u), R16(0x1f801c00u));
    W16(0x1f801d84u, 0x1234u);
    W16(0x1f801c00u, 0xffffu);
    SAY("written %04x %04x\n", R16(0x1f801d84u), R16(0x1f801c00u));
    W16(0x1f801d84u, 0u);
    SAY("zero %04x\n", R16(0x1f801d84u));
    W16(0x1f801dfeu, 0xa5a5u);
    SAY("last register %04x\n", R16(0x1f801dfeu));''')
pcase("chip-registers-read-0-at-start-and-read-back-what-was-written-16-bits-at-a-time", "p_chip_plain",
      ["at start 0000 0000", "written 1234 ffff", "zero 0000", "last register a5a5"])

# a voice with a sample the game put into the chip's RAM (by the transfer address and the FIFO registers): silent before the key-on,
# sound after it; the key-on register of voices 0 to 15 (offset 0x188) and of voices 16 to 23 (0x18a)
for voice, keyon_reg, vbase in [(0, 0x1f801d88, 0x1f801c00), (16, 0x1f801d8a, 0x1f801d00)]:
    pfunc("p_keyon%d" % voice, r'''
    int before, after;
    put_sample(0x1000u);
    SAY("transfer address read back %%04x\n", R16(0x1f801da6u));
    W16(0x1f801d80u, 0x3fffu);
    W16(0x1f801d82u, 0x3fffu);
    W16(0x1f801daau, 0xc000u);
    W16(%(v)su + 0x00u, 0x3fffu);
    W16(%(v)su + 0x02u, 0x3fffu);
    W16(%(v)su + 0x04u, 0x1000u);
    W16(%(v)su + 0x06u, 0x0200u);
    W16(%(v)su + 0x08u, 0x00ffu);
    W16(%(v)su + 0x0au, 0x0000u);
    before = ps1_pull_nonzero(512);
    SAY("the chip's RAM holds the sample %%d, silent before the key-on %%d\n", ps1_chip_ram(0x1003) == 0x77, before == 0);
    W16(%(k)su, 1u);
    after = ps1_pull_nonzero(512);
    SAY("sound after the key-on %%d\n", after > 0);''' % dict(v=hx(vbase), k=hx(keyon_reg)))
    pcase("voice-%d-with-a-sample-in-the-chips-ram-is-silent-before-its-key-on-and-sounds-after-it" % voice, "p_keyon%d" % voice,
          ["transfer address read back 0202", "the chip's RAM holds the sample 1, silent before the key-on 1", "sound after the key-on 1"])

pfunc("p_status", r'''
    W16(0x1f801daau, 0x8035u);
    SAY("control 8035: status %04x\n", R16(0x1f801daeu));
    W16(0x1f801daau, 0xc03fu);
    SAY("control c03f: status %04x\n", R16(0x1f801daeu));
    W16(0x1f801daeu, 0xffffu);
    SAY("a write to the status: status %04x control %04x\n", R16(0x1f801daeu), R16(0x1f801daau));
    W16(0x1f801daau, 0u);
    SAY("control 0: status %04x\n", R16(0x1f801daeu));''')
pcase("the-status-register-mirrors-the-low-six-bits-of-the-control-and-ignores-a-write", "p_status",
      ["control 8035: status 0035", "control c03f: status 003f", "a write to the status: status 003f control c03f", "control 0: status 0000"])

# the chip's accesses that stop
CHIP_W = "with an access of %d bytes on a register that takes 2"
for tag, kind, size, asm, prefix, addr in [
    ("r32", "read", 4, '__asm__ volatile("movl 0x1f801d84, %%ebx" : : : "ebx", "memory");', "8b 1d ", 0x1f801d84),
    ("r8", "read", 1, '__asm__ volatile("movzbl 0x1f801d84, %%eax" : : : "eax", "memory");', "0f b6 05 ", 0x1f801d84),
    ("w32", "write", 4, '__asm__ volatile("movl $0x11, 0x1f801d84" : : : "memory");', "c7 05 ", 0x1f801d84),
    ("w8", "write", 1, '__asm__ volatile("movb $0x12, 0x1f801d84" : : : "memory");', "c6 05 ", 0x1f801d84),
]:
    fn = pfunc("p_chip_%s" % tag, 'W16(0x1f801d84u, 0x4321u);\n    SAY("before %%04x\\n", R16(0x1f801d84u));\n    %s\n    SAY("after\\n");' % asm)
    tail = {"r32": "", "r8": "", "w32": " 11 00 00 00", "w8": " 12"}[tag]
    pcase("chip-register-%d-bit-%s-ends-the-program-with-the-stop-line" % (size * 8, kind), fn, ["before 4321"],
          stop_re(addr, kind, fn, CHIP_W % size, prefix + le(addr) + tail))
for tag, kind, asm, prefix, tail in [
    ("odd_r", "read", '__asm__ volatile("movzwl 0x1f801d85, %%eax" : : : "eax", "memory");', "0f b7 05 ", ""),
    ("odd_w", "write", '__asm__ volatile("movw $0x1234, 0x1f801d85" : : : "memory");', "66 c7 05 ", " 34 12"),
]:
    fn = pfunc("p_chip_%s" % tag, 'SAY("before\\n");\n    %s\n    SAY("after\\n");' % asm)
    pcase("chip-16-bit-%s-at-an-odd-address-ends-the-program-with-the-stop-line" % kind, fn, ["before"],
          stop_re(0x1f801d85, kind, fn, "with a 16-bit access at the odd address 0x1f801d85", prefix + le(0x1f801d85) + tail))
for addr, label in [(0x1f801bfe, "just-before-the-chip"), (0x1f801e00, "just-after-the-chip")]:
    fn = pfunc("p_chip_edge_%x" % addr, 'SAY("before\\n");\n    __asm__ volatile("movzwl %s, %%%%eax" : : : "eax", "memory");\n    SAY("after\\n");' % hx(addr))
    pcase("a-16-bit-read-%s-has-no-model-and-ends-the-program" % label, fn, ["before"],
          stop_re(addr, "read", fn, "with an address that no model of the page claims (%s, 2 bytes)" % hx(addr), "0f b7 05 " + le(addr)))
fn = pfunc("p_chip_32_at_chip_end", 'SAY("before\\n");\n    __asm__ volatile("movl 0x1f801dfe, %%ebx" : : : "ebx", "memory");\n    SAY("after\\n");')
pcase("a-32-bit-read-that-begins-in-the-chip-and-ends-after-it-ends-the-program", fn, ["before"],
      stop_re(0x1f801dfe, "read", fn, CHIP_W % 4, "8b 1d " + le(0x1f801dfe)))

# ---- DMA channel 4 ----
pfunc("p_dma_regs", r'''
    SAY("at start %08x %08x %08x\n", R32(0x1f8010c0u), R32(0x1f8010c4u), R32(0x1f8010c8u));
    W32(0x1f8010c0u, 0x80123456u);
    W32(0x1f8010c4u, 0x00020010u);
    W32(0x1f8010c8u, 0x00000201u);
    SAY("written %08x %08x %08x\n", R32(0x1f8010c0u), R32(0x1f8010c4u), R32(0x1f8010c8u));
    W32(0x1f8010c0u, 0x80180000u);
    W32(0x1f8010c4u, 0x00010010u);
    W32(0x1f8010c8u, TO_CHIP);
    SAY("after a start %08x %08x %08x\n", R32(0x1f8010c0u), R32(0x1f8010c4u), R32(0x1f8010c8u));''')
pcase("dma-registers-read-0-at-start-read-back-what-was-written-and-bit-24-reads-clear-after-a-transfer", "p_dma_regs",
      ["at start 00000000 00000000 00000000", "written 80123456 00020010 00000201", "after a start 80180000 00010010 00000201"])

pfunc("p_dma_roundtrip", r'''
    int i, same = 1, held = 1;
    ps1_DMACallback(4, dma_routine);
    for (i = 0; i < 256; i++) BUF_A[i] = (unsigned char)(i * 7 + 3);
    for (i = 0; i < 256; i++) BUF_B[i] = 0;
    W16(0x1f801da6u, 0x40u);
    start_dma(0x80180000u, 256, TO_CHIP);
    wait_dma(1);
    SAY("to the chip: address now %04x, routine ran %d\n", R16(0x1f801da6u), dma_count);
    for (i = 0; i < 256; i++) if (ps1_chip_ram(0x200 + i) != BUF_A[i]) held = 0;
    W16(0x1f801da6u, 0x40u);
    start_dma(0x80181000u, 256, FROM_CHIP);
    wait_dma(2);
    for (i = 0; i < 256; i++) if (BUF_A[i] != BUF_B[i]) same = 0;
    SAY("from the chip: address now %04x, routine ran %d\n", R16(0x1f801da6u), dma_count);
    SAY("the chip holds the bytes %d, the bytes came back equal %d, not all zero %d\n", held, same, BUF_B[1] == 10 && BUF_B[255] == (unsigned char)(255 * 7 + 3));''')
pcase("dma-writes-256-bytes-to-the-chips-ram-and-reads-them-back-equal", "p_dma_roundtrip",
      ["to the chip: address now 0060, routine ran 1", "from the chip: address now 0060, routine ran 2", "the chip holds the bytes 1, the bytes came back equal 1, not all zero 1"],
      args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"], counts=comp(2, 2, 0, 0, 0, 2, 256, 256))

pfunc("p_dma_low_address", r'''
    int i, held = 1;
    ps1_DMACallback(4, dma_routine);
    for (i = 0; i < 64; i++) BUF_A[i] = (unsigned char)(i + 1);
    W16(0x1f801da6u, 0x00u);
    start_dma(0x00180000u, 64, TO_CHIP);
    wait_dma(1);
    for (i = 0; i < 64; i++) if (ps1_chip_ram(i) != BUF_A[i]) held = 0;
    SAY("an address with the top byte 0 is the same RAM: %d\n", held);''')
pcase("dma-takes-an-address-with-the-top-byte-0-as-the-same-ram", "p_dma_low_address", ["an address with the top byte 0 is the same RAM: 1"])

pfunc("p_dma_edge", r'''
    ps1_DMACallback(4, dma_routine);
    W16(0x1f801da6u, 0x00u);
    start_dma(0x801fffc0u, 64, TO_CHIP);
    wait_dma(1);
    SAY("a transfer that ends exactly at the end of RAM: %d\n", dma_count);''')
pcase("dma-accepts-a-span-that-ends-exactly-at-the-end-of-ram", "p_dma_edge", ["a transfer that ends exactly at the end of RAM: 1"])

pfunc("p_dma_chip_full", r'''
    ps1_DMACallback(4, dma_routine);
    BUF_A[0] = 0x5a;
    BUF_A[1] = 0xa5;
    W16(0x1f801da6u, 0x00u);
    start_dma(0x80180000u, 524288, TO_CHIP);
    wait_dma(1);
    SAY("a transfer of the chip's whole 512 KB: %d %02x %02x, address now %04x\n", dma_count, ps1_chip_ram(0), ps1_chip_ram(1), R16(0x1f801da6u));''')
pcase("dma-accepts-a-transfer-of-exactly-the-chips-512-kb", "p_dma_chip_full", ["a transfer of the chip's whole 512 KB: 1 5a a5, address now 0000"])

DMA_REFUSALS = [
    ("an-address-outside-ram", "W32(0x1f8010c0u, 0x80200000u); W32(0x1f8010c4u, 0x00010010u);", "with a DMA of 64 bytes at 0x80200000 that does not lie in RAM"),
    ("a-span-that-ends-outside-ram", "W32(0x1f8010c0u, 0x801fffc0u); W32(0x1f8010c4u, 0x00020010u);", "with a DMA of 128 bytes at 0x801fffc0 that does not lie in RAM"),
    ("a-span-whose-size-wraps-in-32-bits", "W32(0x1f8010c0u, 0x80100000u); W32(0x1f8010c4u, 0x80008000u);", "with a DMA of 4294967296 bytes at 0x80100000 that does not lie in RAM"),
    ("the-largest-block-control", "W32(0x1f8010c0u, 0x80100000u); W32(0x1f8010c4u, 0xffffffffu);", "with a DMA of 17179344900 bytes at 0x80100000 that does not lie in RAM"),
    ("a-top-byte-that-is-neither-00-nor-80", "W32(0x1f8010c0u, 0xa0180000u); W32(0x1f8010c4u, 0x00010010u);", "with a DMA address 0xa0180000 whose top byte is not 0x00 or 0x80"),
    ("no-blocks", "W32(0x1f8010c0u, 0x80180000u); W32(0x1f8010c4u, 0x00000010u);", "with a DMA block control 0x00000010 that has a zero half"),
    ("no-words-in-a-block", "W32(0x1f8010c0u, 0x80180000u); W32(0x1f8010c4u, 0x00010000u);", "with a DMA block control 0x00010000 that has a zero half"),
    ("more-than-the-chips-ram", "W32(0x1f8010c0u, 0x80100000u); W32(0x1f8010c4u, 0x20010010u);", "with a DMA of 524352 bytes, more than the chip's RAM of 524288"),
]
for label, setup, reason in DMA_REFUSALS:
    fn = pfunc("p_dma_refuse_%s" % label.replace("-", "_"), 'SAY("before\\n");\n    %s\n    __asm__ volatile("movl $0x01000201, 0x1f8010c8" : : : "memory");\n    SAY("after\\n");' % setup)
    pcase("dma-refuses-%s" % label, fn, ["before"], stop_re(0x1f8010c8, "write", fn, reason, "c7 05 " + le(0x1f8010c8) + " 01 02 00 01"))
# the largest block control, 0x20010010: 0x2001 blocks of 64 bytes is 524352; keep the table above honest
fn = pfunc("p_dma_second_start", r'''
    ps1_DMACallback(4, dma_routine);
    ps1_EnterCriticalSection();
    start_dma(0x80180000u, 64, TO_CHIP);
    SAY("first started\n");
    __asm__ volatile("movl $0x01000201, 0x1f8010c8" : : : "memory");
    SAY("after\n");''')
pcase("dma-refuses-a-second-start-while-the-completion-of-the-first-is-pending", fn, ["first started"],
      stop_re(0x1f8010c8, "write", fn, "with a DMA start while the completion of the last transfer is pending", "c7 05 " + le(0x1f8010c8) + " 01 02 00 01"))

# ---- the completion ----
pfunc("p_comp_each", r'''
    int i;
    unsigned t0;
    ps1_DMACallback(4, dma_routine);
    for (i = 0; i < 3; i++) {
        start_dma(0x80180000u, 2048, TO_CHIP);
        wait_dma(i + 1);
        SAY("transfer %d: routine ran %d times\n", i + 1, dma_count);
    }
    t0 = us_now();
    while (us_now() - t0 < 30000u) { }
    for (i = 0; i < 5; i++) ps1_VSync(-1);
    SAY("after more polls: %d\n", dma_count);''')
pcase("the-routine-is-called-once-per-transfer", "p_comp_each",
      ["transfer 1: routine ran 1 times", "transfer 2: routine ran 2 times", "transfer 3: routine ran 3 times", "after more polls: 3"],
      args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"], counts=comp(3, 3, 0, 0, 0, 3, 6144))

# the first poll takes it, with no wait: the transfer is large (due 50 ms later), the poll is the next call
pfunc("p_comp_poll", r'''
    unsigned t0, t1;
    ps1_DMACallback(4, dma_routine);
    W16(0x1f801da6u, 0x00u);
    t0 = us_now();
    start_dma(0x80180000u, 131072, TO_CHIP);
    ps1_VSync(-1);
    t1 = us_now();
    SAY("the routine ran in the first poll: %d, before the due time of 50300 us: %d\n", dma_count, t1 - t0 < 50300u);
    fprintf(stderr, "measured: first poll after %u us\n", t1 - t0);
    fflush(stderr);''')
pcase("the-first-poll-takes-the-completion-with-no-wait", "p_comp_poll",
      ["the routine ran in the first poll: 1, before the due time of 50300 us: 1"], args=["--trace", "--trace-file", "@TRACE"], counts=comp(1, 1, 0, 0, 0, 1, 131072))
pcase("without-the-timer-the-first-poll-takes-the-completion-with-no-wait", "p_comp_poll",
      ["the routine ran in the first poll: 1, before the due time of 50300 us: 1"], args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"], counts=comp(1, 1, 0, 0, 0, 1, 131072))

# the timer takes it when the game only spins, and not before the due time: 8192 bytes, due 3144 us after the start (8192 * 13 * 1e6 / 33868800);
# the start is made just after a vblank was delivered, so that the next one is 16 ms away
pfunc("p_comp_timer", r'''
    unsigned t0, t1;
    int n0, i;
    vblank_on(vb_handler);
    ps1_DMACallback(4, dma_routine);
    n0 = vb_count;
    SPIN_UNTIL(vb_count != n0);
    W16(0x1f801da6u, 0x00u);
    t0 = us_now();
    start_dma(0x80180000u, 8192, TO_CHIP);
    SPIN_UNTIL(dma_count != 0);
    t1 = us_now();
    for (i = 0; i < 3; i++) ps1_VSync(-1);
    SAY("the routine ran: %d, not before the due time of 3144 us: %d, within 500 ms: %d\n", dma_count, t1 - t0 >= 3144u, t1 - t0 < 500000u);
    SAY("after three polls: %d\n", dma_count);
    fprintf(stderr, "measured: timer delivered after %u us\n", t1 - t0);
    fflush(stderr);''')
pcase("the-timer-takes-the-completion-when-the-game-only-spins-and-not-before-the-due-time", "p_comp_timer",
      ["the routine ran: 1, not before the due time of 3144 us: 1, within 500 ms: 1", "after three polls: 1"],
      args=["--trace", "--trace-file", "@TRACE"], counts=comp(1, 0, 1, 0, 0, 1, 8192))

# the vblank's delivery takes it before the vblank's handlers: inside a critical section a transfer starts, the game waits 20 ms, so that a
# vblank is due and the 50 ms of the transfer are not
pfunc("p_comp_vblank", r'''
    unsigned t0;
    vblank_on(vb_note);
    ps1_DMACallback(4, dma_note);
    ps1_EnterCriticalSection();
    W16(0x1f801da6u, 0x00u);
    start_dma(0x80180000u, 131072, TO_CHIP);
    t0 = us_now();
    while (us_now() - t0 < 20000u) { }
    SAY("in the critical section: %d %d\n", dma_count, vb_count);
    ps1_ExitCriticalSection();
    SPIN_UNTIL(dma_count != 0 && vb_count != 0);
    SAY("order %s\n", order);''')
pcase("the-vblank-delivers-the-completion-before-its-own-handlers", "p_comp_vblank", ["in the critical section: 0 0", "order DV"],
      args=["--trace", "--trace-file", "@TRACE"], counts=comp(1, 0, 0, 1, 0, 1, 131072))

# both are due and the game polls: the completion first, then the vblank
pfunc("p_comp_both_poll", r'''
    unsigned t0;
    vblank_on(vb_note);
    ps1_DMACallback(4, dma_note);
    ps1_EnterCriticalSection();
    start_dma(0x80180000u, 131072, TO_CHIP);
    t0 = us_now();
    while (us_now() - t0 < 20000u) { }
    ps1_ExitCriticalSection();
    ps1_VSync(-1);
    SAY("order %s\n", order);''')
pcase("when-the-vblank-and-the-completion-are-both-due-a-poll-delivers-the-completion-first", "p_comp_both_poll", ["order DV"],
      args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"], counts=comp(1, 1, 0, 0, 0, 1, 131072))

# both are due and the game spins: the timer claims the completion and delivers it before the vblank (a short transfer, due in 25 us)
pfunc("p_comp_both_timer", r'''
    unsigned t0;
    vblank_on(vb_note);
    ps1_DMACallback(4, dma_note);
    ps1_EnterCriticalSection();
    start_dma(0x80180000u, 64, TO_CHIP);
    t0 = us_now();
    while (us_now() - t0 < 20000u) { }
    ps1_ExitCriticalSection();
    SPIN_UNTIL(dma_count != 0 && vb_count != 0);
    SAY("order %s\n", order);''')
pcase("when-the-vblank-and-the-completion-are-both-due-the-timer-delivers-the-completion-first", "p_comp_both_timer", ["order DV"],
      args=["--trace", "--trace-file", "@TRACE"], counts=comp(1, 0, 1, 0, 0, 1, 64))

# not inside a critical section: held until it ends, then delivered once
for how, wait_for in [("p_comp_cs", "SPIN_UNTIL(dma_count != 0);"), ("p_comp_cs_poll", "ps1_VSync(-1);")]:
  pfunc(how, r'''
    unsigned t0;
    int n;
    ps1_DMACallback(4, dma_routine);
    ps1_EnterCriticalSection();
    start_dma(0x80180000u, 64, TO_CHIP);
    t0 = us_now();
    while (us_now() - t0 < 100000u) { }
    ps1_VSync(-1);
    ps1_TestEvent(0xf1000000u);
    SAY("in the critical section, after 100 ms and two polls: %d\n", dma_count);
    ps1_ExitCriticalSection();
    WAIT_FOR
    n = dma_count;
    t0 = us_now();
    while (us_now() - t0 < 50000u) { }
    ps1_VSync(-1);
    SAY("after the critical section: %d, 50 ms and a poll later: %d\n", n, dma_count);'''.replace("WAIT_FOR", wait_for))
pcase("a-completion-is-held-back-inside-a-critical-section-and-delivered-once-after-it", "p_comp_cs",
      ["in the critical section, after 100 ms and two polls: 0", "after the critical section: 1, 50 ms and a poll later: 1"],
      args=["--trace", "--trace-file", "@TRACE"], counts=comp(1, 0, 1, 0, 0, 1, 64))
pcase("without-the-timer-a-completion-is-held-back-inside-a-critical-section-until-the-first-poll-after-it", "p_comp_cs_poll",
      ["in the critical section, after 100 ms and two polls: 0", "after the critical section: 1, 50 ms and a poll later: 1"], args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"],
      counts=comp(1, 1, 0, 0, 0, 1, 64))

# not inside a handler of the game: a handler starts a transfer and polls; the routine runs after the handler ends
pfunc("p_comp_handler", r'''
    vblank_on(vb_handler_starts);
    ps1_DMACallback(4, dma_note);
    SPIN_UNTIL(dma_count != 0);
    SAY("order %s\n", order);''')
PSYZ_HEADER += r"""
static void vb_handler_starts(void)
{
    static int done;
    if (done) return;
    done = 1;
    note('H');
    start_dma(0x80180000u, 64, TO_CHIP);
    ps1_VSync(-1);
    ps1_TestEvent(0xf1000000u);
    note('h');
}
"""
pcase("a-completion-is-not-delivered-inside-a-handler-of-the-game", "p_comp_handler", ["order HhD"], args=["--trace", "--trace-file", "@TRACE"],
      counts=comp(1, 0, 1, 0, 0, 1, 64))

# no routine: dropped and counted
pfunc("p_comp_dropped", r'''
    start_dma(0x80180000u, 64, TO_CHIP);
    ps1_VSync(-1);
    ps1_VSync(-1);
    SAY("polled\n");''')
pcase("a-completion-with-no-routine-registered-is-dropped-and-counted", "p_comp_dropped", ["polled"], args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"],
      counts=comp(1, 1, 0, 0, 1, 1, 64))
pfunc("p_comp_cleared", r'''
    ps1_DMACallback(4, dma_routine);
    ps1_DMACallback(4, 0);
    start_dma(0x80180000u, 64, TO_CHIP);
    ps1_VSync(-1);
    SAY("routine ran %d\n", dma_count);''')
pcase("a-routine-that-was-taken-off-is-not-called-and-the-completion-is-dropped", "p_comp_cleared", ["routine ran 0"], args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"],
      counts=comp(1, 1, 0, 0, 1, 1, 64))

# the routine's address is checked at the call, not when it is registered
for how, args in [("poll", ["--no-interrupt"]), ("timer", [])]:
    fn = pfunc("p_comp_bad_%s" % how, r'''
    void *old = ps1_DMACallback(4, (void (*)(void))0x%08xu);
    SAY("registered, previous %%d\n", old == 0);
    start_dma(0x80180000u, 64, TO_CHIP);
    SAY("started\n");
    %s''' % (UNREG, "ps1_VSync(-1);" if how == "poll" else "SPIN_UNTIL(0);"))
    pcase("a-routine-outside-the-games-code-is-refused-at-the-call-not-when-registered-%s" % how, fn, ["registered, previous 1", "started"],
          r"refused: dma callback 0x%08x is not a function this program installed" % UNREG, status=12, args=args)

# ---- the services ----
pfunc("p_svc_dmacallback", r'''
    void *a, *b, *c, *d, *e, *f;
    a = ps1_DMACallback(4, dma_routine);
    b = ps1_DMACallback(4, dma_note);
    c = ps1_DMACallback(4, 0);
    d = ps1_DMACallback(7, dma_routine);
    e = ps1_DMACallback(4, dma_routine);
    f = ps1_DMACallback(3, dma_note);
    SAY("first previous is null %d, second is the first routine %d, third is the second %d\n", a == 0, b == (void *)dma_routine, c == (void *)dma_note);
    SAY("channel 7 returns null %d, did not change channel 4 %d, channel 3 is its own %d\n", d == 0, e == 0, f == 0);''')
pcase("dmacallback-keeps-a-routine-per-channel-and-returns-the-previous-one", "p_svc_dmacallback",
      ["first previous is null 1, second is the first routine 1, third is the second 1", "channel 7 returns null 1, did not change channel 4 1, channel 3 is its own 1"])

pfunc("p_svc_deliver", r'''
    unsigned ev, other, hev;
    int a, b, c;
    ev = ps1_OpenEvent(0xf0000009u, 0x20, 0x2000, 0);
    other = ps1_OpenEvent(0xf0000009u, 0x21, 0x2000, 0);
    ps1_EnableEvent(ev);
    ps1_EnableEvent(other);
    SAY("before a delivery: %d\n", ps1_TestEvent(ev));
    ps1_DeliverEvent(0xf0000009u, 0x20);
    a = ps1_TestEvent(ev);
    b = ps1_TestEvent(ev);
    c = ps1_TestEvent(other);
    SAY("the event is seen once: %d, then %d; the other event: %d\n", a, b, c);
    hev = ps1_OpenEvent(0xf0000009u, 0x22, 0x1000, vb_handler);
    ps1_EnableEvent(hev);
    ps1_DeliverEvent(0xf0000009u, 0x22);
    SAY("an event of the handler mode runs its handler: %d\n", vb_count);''')
pcase("an-event-delivered-is-seen-by-testevent-once", "p_svc_deliver",
      ["before a delivery: 0", "the event is seen once: 1, then 0; the other event: 0", "an event of the handler mode runs its handler: 1"])

PSYZ_HEADER += r"""
static unsigned wait_ev;
static void dma_delivers(void) { dma_count++; ps1_DeliverEvent(0xf0000009u, 0x20); }
"""
pfunc("p_svc_wait", r'''
    int r, a, b;
    unsigned never;
    wait_ev = ps1_OpenEvent(0xf0000009u, 0x20, 0x2000, 0);
    ps1_EnableEvent(wait_ev);
    ps1_DMACallback(4, dma_delivers);
    start_dma(0x80180000u, 2048, TO_CHIP);
    r = ps1_WaitEvent(wait_ev);
    SAY("WaitEvent returned %d after the routine ran %d times\n", r, dma_count);
    SAY("the event is enabled again: %d\n", ps1_TestEvent(wait_ev));
    never = ps1_OpenEvent(0xf0000009u, 0x30, 0x2000, 0);
    a = ps1_WaitEvent(never);
    b = ps1_WaitEvent(0x12345678u);
    SAY("a disabled event: %d, a bad descriptor: %d\n", a, b);''')
pcase("waitevent-returns-after-a-delivery-as-the-sound-librarys-completion-makes-it", "p_svc_wait",
      ["WaitEvent returned 1 after the routine ran 1 times", "the event is enabled again: 0", "a disabled event: 0, a bad descriptor: 0"],
      args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"], counts=comp(1, 1, 0, 0, 0, 1, 2048))

# the counts of the trace, written out by hand: 2 chip writes and 1 read, 3 DMA writes and 3 reads, one transfer of 64 bytes, taken by the poll
pfunc("p_trace_counts", r'''
    ps1_DMACallback(4, dma_routine);
    W16(0x1f801d84u, 0x1111u);
    W16(0x1f801d86u, 0x2222u);
    (void)R16(0x1f801d84u);
    W32(0x1f8010c0u, 0x80180000u);
    W32(0x1f8010c4u, 0x00010010u);
    W32(0x1f8010c8u, TO_CHIP);
    (void)R32(0x1f8010c0u);
    (void)R32(0x1f8010c4u);
    (void)R32(0x1f8010c8u);
    ps1_VSync(-1);
    SAY("routine ran %d\n", dma_count);''')
pcase("with-trace-the-chips-and-dmas-counts-and-the-completions-are-one-line-at-exit", "p_trace_counts", ["routine ran 1"],
      args=["--no-interrupt", "--trace", "--trace-file", "@TRACE"],
      counts=r"iopage: chip 1 reads 2 writes; dma 3 reads 3 writes; 1 transfers, 64 bytes to the chip, 0 bytes from the chip; " + COMP % (1, 1, 0, 0, 0))


# a run that is ended from outside does not reach its exit: every 60 vblanks the counts line is written again when the counts changed
pfunc("p_trace_periodic", r'''
    vblank_on(vb_handler);
    W16(0x1f801d84u, 0x1111u);
    { unsigned long n = 0; unsigned t0 = us_now(); while (vb_count < 65 && !((++n & 0xfffffu) == 0 && us_now() - t0 > 6000000u)) { } }
    SAY("vblanks passed: %d\n", vb_count >= 65);''')
pcase("with-trace-the-counts-are-written-again-at-the-60th-vblank-when-they-changed", "p_trace_periodic", ["vblanks passed: 1"],
      args=["--trace", "--trace-file", "@TRACE"], periodic=r"iopage: at vblank 60: chip 0 reads 1 writes; dma 0 reads 0 writes; 0 transfers, 0 bytes to the chip, 0 bytes from the chip; " + COMP % (0, 0, 0, 0, 0))


DOMAINS = L.DOMAINS_NONE + r"""
unsigned host_io_read(void) { return *(volatile unsigned *)0x1f801014; }
"""
FUN = FUN_BASE + [(n, RAM + 0x101100 + 0x10 * i, n) for i, n in enumerate(NAMES + PNAMES)]
G = {n: a for n, a, _ in FUN}
# the thread functions are used by the functions above them: declare them first
THREAD_DECL = "static DWORD WINAPI thread_fn(LPVOID p);\nstatic DWORD WINAPI thread_wfn(LPVOID p);\n"
GAME = HEADER + THREAD_DECL + "".join(BODIES) + THREAD_FN
GAME_PSYZ = HEADER + PSYZ_HEADER + THREAD_DECL + "".join(BODIES) + "".join(PBODIES) + THREAD_FN


QUIET_DRIVER = "dummy"   # SDL's audio driver that opens no device


def sound_env(driver: str = QUIET_DRIVER) -> dict:
    """This process's environment with SDL_AUDIODRIVER set to `driver`, and under WSL with the route that carries the variable into a
    Windows child (WSLENV)."""
    env = dict(os.environ)
    env["SDL_AUDIODRIVER"] = driver
    if shutil.which("wslpath"):
        parts = [p for p in env.get("WSLENV", "").split(":") if p and p.split("/")[0] != "SDL_AUDIODRIVER"]
        env["WSLENV"] = ":".join(parts + ["SDL_AUDIODRIVER"])
    return env


class PsyzRig(L.Rig):
    """The launch rig with PsyZ's model of the chip: iopage.c and the made-up runtime's routines are compiled for it, and every program
    is linked with PsyZ's libraries (the build folder's psyz.json says how).

    A program linked with PsyZ opens the machine's sound output unless it is told not to. NO CASE MAY OPEN A REAL DEVICE: every child
    this rig starts gets `self.env`, which names SDL's dummy audio driver, and `run` puts `self.first` before the arguments of the
    port's program (here --no-audio: the cases of this file use the model with nothing pulling from it)."""

    def __init__(self, cc, prefix, work, info):
        super().__init__(cc, prefix, work)
        self.info = info
        self.libs = [str(x) for x in info["link"]]
        self.env = sound_env()
        self.first = ["--no-audio"]

    def start(self, exe, args, timeout):
        """As the launch rig's, with this rig's environment."""
        self.last = None
        try:
            proc = subprocess.run([*self.prefix, str(exe), *args], capture_output=True, text=True, timeout=timeout, env=self.env)
        except subprocess.TimeoutExpired as err:
            self.record(None, err.stdout, err.stderr, timed_out=True)
            raise
        self.record(proc.returncode, proc.stdout, proc.stderr)
        return proc.returncode, proc.stdout

    def run(self, tag, data, pin=None, variant="base", args=None, timeout=60, old_link=False):
        return super().run(tag, data, pin, variant, [*self.first, *(args or [])], timeout, old_link)

    def compile(self, source, extra=None):
        if source.name == "iopage.c" or source.name.endswith("-domains.c"):
            extra = [*(extra or []), "-DPORT_HAVE_PSYZ", *(f"-D{d}" for d in self.info["define"]), "-isystem", str(self.info["include"])]
        return super().compile(source, extra)


# ---- the check every case goes through ----
def check(status, body, want_status, want_lines, stop):
    """None when the program ended with `want_status`, printed exactly `want_lines` and then (stop is None) `stop: main returned`
    or (stop is a regex) one line that matches it as a whole, and nothing else."""
    tail = ["stop: main returned"] if stop is None else None
    if status != want_status:
        return f"status {status}, wanted {want_status}; lines {body[-3:]!r}"
    if stop is None:
        return verdict(body, want_lines + tail)
    if len(body) != len(want_lines) + 1 or body[:-1] != want_lines:
        return f"lines {body!r}, wanted {want_lines!r} and then the stop line"
    if not re.fullmatch(stop, body[-1]):
        return f"stop line {body[-1]!r} does not match {stop!r}"
    return None


def checks_checks():
    """The check itself, on invented runs: it must fail a missing line, a line more, a wrong status, a death after the
    right line and a stop line that is not the one wanted."""
    stop = r"stop: crash: x at 0x[0-9a-f]+"
    good = ["a", "stop: crash: x at 0x10"]
    yield "the-check-passes-the-run-that-printed-the-lines-and-the-stop-line-with-the-status", check(10, good, 10, ["a"], stop)
    yield "the-check-passes-a-run-that-returned", check(0, ["a", "stop: main returned"], 0, ["a"], None)
    bad = {
        "a-missing-line": check(10, ["stop: crash: x at 0x10"], 10, ["a"], stop),
        "a-line-more": check(10, ["a", "b", "stop: crash: x at 0x10"], 10, ["a"], stop),
        "another-status": check(1, good, 10, ["a"], stop),
        "a-status-0-after-the-stop-line": check(0, good, 10, ["a"], stop),
        "a-death-after-the-right-line-without-the-stop-line": check(-1073741819, ["a"], 10, ["a"], stop),
        "a-stop-line-that-is-another": check(10, ["a", "stop: crash: y at 0x10"], 10, ["a"], stop),
        "a-return-instead-of-the-stop": check(0, ["a", "stop: main returned"], 10, ["a"], stop),
        "a-stop-instead-of-the-return": check(10, ["a", "stop: crash: x at 0x10"], 0, ["a"], None),
        "no-lines-at-all": check(10, [], 10, ["a"], stop),
    }
    for name, got in bad.items():
        yield "the-check-fails-" + name, None if got is not None else "the check passed it"


def launch(rig: L.Rig, only: str, psyz: bool, not_run: list[str]):
    if psyz:
        L.VARIANTS["iopage"] = Variant("iopage", GAME_PSYZ, FUN, ABS_BASE + PSYZ_ABS, psyz_domains())
    else:
        L.VARIANTS["iopage"] = Variant("iopage", GAME, [f for f in FUN if f[0] not in PNAMES], ABS_BASE, DOMAINS)
    yield from checks_checks()
    yield from L.rig_diagnostic_cases(rig)
    name = "the-rig-starts-a-program-linked-with-psyz-with-no-audio-and-it-prints-no-line-about-sound"
    if psyz and (not only or only in name):
        # such a program opens the sound output and says so (`audio: ...`) unless it is told not to: no such line, so it was told
        status, lines, img, arg = rig.run("quiet", program(G[CASES[0]["fn"]]), variant="iopage", args=CASES[0]["args"], timeout=120)
        yield name, None if (any(l.startswith("start: 0x") for l in lines) and not any(l.startswith("audio:") for l in lines)
                             and rig.env.get("SDL_AUDIODRIVER") == QUIET_DRIVER) else f"SDL_AUDIODRIVER {rig.env.get('SDL_AUDIODRIVER')!r}; lines {lines!r}"
    for c in CASES:
        if only and only not in c["name"]:
            continue
        if c["psyz"] and not psyz:
            not_run.append(c["name"])
            continue
        if c["no_psyz"] and psyz:
            continue
        trace = rig.work / "iopage-trace.txt"
        trace.unlink(missing_ok=True)
        args = [rig.native(trace) if a == "@TRACE" else a for a in c["args"]]
        if c["fn"] == "i_trace":
            args = ["--trace", "--trace-file", rig.native(trace)]
        status, lines, img, arg = rig.run(c["name"][:40], program(G[c["fn"]]), variant="iopage", args=args, timeout=120)
        s = f"start: 0x{G[c['fn']]:08x}"
        body = lines[lines.index(s) + 1:] if s in lines else lines
        why = check(status, body, c["status"], c["lines"], c["stop"])
        if why is None and (c["counts"] is not None or c["periodic"] is not None):
            tl = trace.read_text().splitlines() if trace.exists() else []
            io = [l for l in tl if l.startswith("iopage:")]
            per = [l for l in io if l.startswith("iopage: at vblank")]   # the counts line that is written every 60 vblanks
            io = [l for l in io if l not in per]
            if c["periodic"] is not None and (len(per) != 1 or not re.fullmatch(c["periodic"], per[0])):
                why = f"the trace has the periodic lines {per!r}, wanted one that matches {c['periodic']!r}"
            elif c["counts"] is None:
                pass
            elif len(io) != 3 or io[0] != "iopage: the I/O page 0x1f801000..0x1f801fff closed":
                why = f"the trace has the iopage lines {io!r}, wanted three, the first the one that says the page is closed"
            elif not re.fullmatch(c["counts"], io[2]):
                why = f"the counts line {io[2]!r} does not match {c['counts']!r}"
        if why is None and c["fn"] == "i_trace":
            tl = trace.read_text().splitlines() if trace.exists() else []
            io = [l for l in tl if l.startswith("iopage:") and not l.startswith("iopage: at vblank")]
            why = verdict(io, ["iopage: the I/O page 0x1f801000..0x1f801fff closed",
                               "iopage: 7 faults served; 0x1f801014 2 reads 1 writes, 0x1f8010f0 2 reads 1 writes, 0x1f8010f4 1 reads 1 writes",
                               "iopage: chip 0 reads 0 writes; dma 0 reads 0 writes; 0 transfers, 0 bytes to the chip, 0 bytes from the chip; "
                               "completions 0 taken (poll 0, timer 0, vblank 0), 0 dropped without a routine"])
        for line in (rig.last or {}).get("stderr", []):
            if line.startswith("measured:"):
                print("     " + line, flush=True)
        yield c["name"], why


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--only", default="", help="run only the cases whose name contains this")
    parser.add_argument("--psyz-build", default=None, type=Path, help="a build folder of psyzbuild.py: the cases of the chip, DMA and the completion need it")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    if args.src:
        L.SRC = Path(args.src).resolve()
    L.BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostiopage-", dir=L.BUILD))
    try:
        info = None
        if args.psyz_build:
            try:
                info = json.loads((args.psyz_build / "psyz.json").read_text())
            except (OSError, ValueError) as err:
                print(f"{args.psyz_build / 'psyz.json'} cannot be read ({err}); build PsyZ with psyzbuild.py first")
                return 2
        rig = PsyzRig(args.cc, args.run.split(), work, info) if info else L.Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = total = 0
        not_run: list[str] = []
        try:
            for name, detail in launch(rig, args.only, info is not None, not_run):
                total += 1
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    print("\n".join(rig.report()))
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            print("\n".join(rig.report()))
            failed += 1
        for name in not_run:
            print(f"NOT RUN {name}: it needs --psyz-build")
        if not_run:
            print(f"{len(not_run)} case(s) were not run: they need --psyz-build DIR (a build folder of psyzbuild.py)")
        print(f"{failed} case(s) behaved wrongly" if failed else f"all {total} cases that ran behaved as required")
        return 1 if failed else (3 if not_run else 0)
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
