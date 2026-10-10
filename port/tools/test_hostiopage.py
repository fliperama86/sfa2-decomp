#!/usr/bin/env python3
"""Controls for the PS1's I/O page (iopage.c, and its part of mirror.c).

    python3 test_hostiopage.py --cc CROSS_CC [--run PREFIX] [--src DIR] [--only SUBSTRING]

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
- --trace: one line at exit with the faults served and the reads and writes of each cell, and the line that says the page is closed.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number of cases
that did not. --src names another copy of the runtime's folder (the mutants of the controls run on a scratch copy).
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
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


def func(name: str, body: str) -> str:
    NAMES.append(name)
    BODIES.append("void %s(void)\n{\n%s\n}\n" % (name, body))
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


def case(name, fn, lines, stop=None, status=None, args=None, after=None):
    CASES.append(dict(name=name, fn=fn, lines=lines, stop=stop, status=status if status is not None else (0 if stop is None else 10), args=args or [], after=after))


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
    fn = func("i_nomr_%x" % addr, 'SAY("before\\n");\n    __asm__ volatile("movl %s, %%%%ebx" : : : "ebx", "memory");\n    SAY("after\\n");' % hx(addr))
    case("a-read-at-%s-has-no-model-and-ends-the-program" % label, fn, ["before"],
         stop_re(addr, "read", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(addr), "8b 1d " + le(addr)))
    fn = func("i_nomw_%x" % addr, 'SAY("before\\n");\n    __asm__ volatile("movl $1, %s" : : : "memory");\n    SAY("after\\n");' % hx(addr))
    case("a-write-at-%s-has-no-model-and-ends-the-program" % label, fn, ["before"],
         stop_re(addr, "write", fn, "with an address that no model of the page claims (%s, 4 bytes)" % hx(addr), "c7 05 " + le(addr) + " 01 00 00 00"))
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
fn = func("i_thread", 'HANDLE h;\n    SAY("before\\n");\n    h = CreateThread(NULL, 0, thread_fn, NULL, 0, NULL);\n    WaitForSingleObject(h, 20000);\n    SAY("after\\n");')
case("a-read-from-a-second-thread-in-the-games-code-ends-the-program", fn, ["before"], r"stop: crash: the game used the PS1's I/O page at 0x1f801014 \(read\) in \S+ %s: [0-9a-f]{2}( [0-9a-f]{2})*" % re.escape(NOT_GAME))
fn = func("i_thread_w", 'HANDLE h;\n    SAY("before\\n");\n    h = CreateThread(NULL, 0, thread_wfn, NULL, 0, NULL);\n    WaitForSingleObject(h, 20000);\n    SAY("after\\n");')
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

DOMAINS = L.DOMAINS_NONE + r"""
unsigned host_io_read(void) { return *(volatile unsigned *)0x1f801014; }
"""
FUN = FUN_BASE + [(n, RAM + 0x101100 + 0x10 * i, n) for i, n in enumerate(NAMES)]
G = {n: a for n, a, _ in FUN}
GAME = HEADER + THREAD_FN + "".join(BODIES)
# the thread functions are used by the functions above them: declare them first
GAME = HEADER + "static DWORD WINAPI thread_fn(LPVOID p);\nstatic DWORD WINAPI thread_wfn(LPVOID p);\n" + "".join(BODIES) + THREAD_FN


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


def launch(rig: L.Rig, only: str):
    L.VARIANTS["iopage"] = Variant("iopage", GAME, FUN, ABS_BASE, DOMAINS)
    yield from checks_checks()
    yield from L.rig_diagnostic_cases(rig)
    for c in CASES:
        if only and only not in c["name"]:
            continue
        trace = rig.work / "iopage-trace.txt"
        trace.unlink(missing_ok=True)
        args = list(c["args"])
        if c["fn"] == "i_trace":
            args = ["--trace", "--trace-file", rig.native(trace)]
        status, lines, img, arg = rig.run(c["name"][:40], program(G[c["fn"]]), variant="iopage", args=args, timeout=120)
        s = f"start: 0x{G[c['fn']]:08x}"
        body = lines[lines.index(s) + 1:] if s in lines else lines
        why = check(status, body, c["status"], c["lines"], c["stop"])
        if why is None and c["fn"] == "i_trace":
            tl = trace.read_text().splitlines() if trace.exists() else []
            io = [l for l in tl if l.startswith("iopage:")]
            why = verdict(io, ["iopage: the I/O page 0x1f801000..0x1f801fff closed",
                               "iopage: 7 faults served; 0x1f801014 2 reads 1 writes, 0x1f8010f0 2 reads 1 writes, 0x1f8010f4 1 reads 1 writes"])
        yield c["name"], why


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--only", default="", help="run only the cases whose name contains this")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    if args.src:
        L.SRC = Path(args.src).resolve()
    L.BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostiopage-", dir=L.BUILD))
    try:
        rig = L.Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = total = 0
        try:
            for name, detail in launch(rig, args.only):
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
        print(f"{failed} case(s) behaved wrongly" if failed else f"all {total} cases behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
