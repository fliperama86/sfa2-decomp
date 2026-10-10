#!/usr/bin/env python3
"""Controls for the PS1's copy of RAM at address 0 (mirror.c, mirrorcore.c).

    python3 test_hostmirror.py --cc CROSS_CC [--run PREFIX] [--src DIR] [--part native|launch|all] [--only SUBSTRING]

Two parts. The native part (mirror_native_test.c) links mirrorcore.c alone and starts none of the port's program:
the decision "is this fault served" at its borders, the decoder on every form the file builds (each register
operand, 223 addressing shapes) and on instructions that must be refused, the operation against the processor
(each built instruction is run for real on real memory and by the port on a copy; registers, all flags and every
byte of memory must be equal), and the start check on invented answers of the system.

The launch part builds the runtime with made-up game code (as test_hostlaunch.py does) and runs the whole program:
a served read and write of each size and the same bytes in the RAM, the destination that is the base register
(whole and partly), a store from the register that forms the address, the read of [null + 0xd], the arithmetic
with a memory operand and its flags, the lines (first use once per function, the trace lines, the start check,
the counts at exit), a loop of many served accesses with the vblank timer on, and the refusals: an access that
straddles 0x10000 (crash line), one from host code (crash line), an execute fault at a low address, a form that
is not served (stop line with the instruction's bytes), a second fault inside the handler. One timing is printed.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all cases behaved as required` or the number
of cases that did not. Windows programs run from a WSL shell as test_hostlaunch.py does. --src names another copy of
the runtime's folder (the mutants of the controls run on a scratch copy).
"""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostlaunch as L  # noqa: E402
from test_hostlaunch import ABS_BASE, FUN_BASE, PROLOGUE, RAM, Variant, program, head, verdict  # noqa: E402

M_NAMES = ["g_rw", "g_base", "g_null", "g_straddle", "g_straddle_w", "g_host", "g_unserved", "g_exec", "g_two", "g_a1", "g_a2", "g_second", "g_loop", "g_time", "g_alu"]
G = {n: RAM + 0x101900 + 0x10 * i for i, n in enumerate(M_NAMES)}
IRQ_NAMES = ["OpenEvent", "EnableEvent", "StartRCnt", "ResetCallback"]
IRQ_ADDR = {n: RAM + 0x101a00 + 0x10 * i for i, n in enumerate(IRQ_NAMES)}
ABS_M = ABS_BASE + [(n, a, 1) for n, a in IRQ_ADDR.items()]
FUN_M = FUN_BASE + [(n, a, n) for n, a in G.items()]

GAME_MIRROR = PROLOGUE + r"""
extern unsigned ps1_OpenEvent(unsigned, unsigned, unsigned, void (*)(void));
extern int ps1_EnableEvent(unsigned), ps1_StartRCnt(unsigned);
extern void *ps1_ResetCallback(void);
extern void ps1_g_a1(void), ps1_g_a2(void);
extern unsigned host_low_read(void);
extern unsigned port_mirror_raced, port_mirror_served_count;
#define LOW8(a) (*(volatile unsigned char *)(a))
#define LOW16(a) (*(volatile unsigned short *)(a))
#define LOW32(a) (*(volatile unsigned *)(a))

void g_rw(void)
{
    LOW8(0x1000) = 0xab;
    LOW16(0x1010) = 0xbeef;
    LOW32(0x1020) = 0xdeadbeefu;
    SAY("read back %02x %04x %08x\n", LOW8(0x1000), LOW16(0x1010), LOW32(0x1020));
    SAY("in the RAM %02x %04x %08x\n", LOW8(0x80001000u), LOW16(0x80001010u), LOW32(0x80001020u));
    SAY("first and last byte of the copy: %02x %02x\n", (LOW8(0) = 0x11, LOW8(0)), (LOW8(0xffff) = 0x22, LOW8(0xffff)));
    SAY("in the RAM %02x %02x\n", LOW8(0x80000000u), LOW8(0x8000ffffu));
    SAY("last word: %08x\n", (LOW32(0xfffc) = 0x01020304u, LOW32(0xfffc)));
}
void g_base(void)
{
    unsigned v, w, x, y, z;
    LOW32(0x80001100u) = 0x11223344u;
    LOW8(0x80001110u) = 0x84;
    LOW16(0x80001120u) = 0x8123;
    __asm__ volatile("movl 13(%%eax), %%eax" : "=a"(v) : "0"(0x10f3u));
    SAY("destination is the base: %08x\n", v);
    __asm__ volatile("movb 13(%%eax), %%al" : "=a"(w) : "0"(0x10f3u));
    SAY("partial destination inside the base: %08x\n", w);
    __asm__ volatile("movzbl 13(%%eax), %%eax" : "=a"(x) : "0"(0x10f3u));
    SAY("movzx into the base: %08x\n", x);
    __asm__ volatile("movsbl 13(%%eax), %%eax" : "=a"(y) : "0"(0x1103u));
    SAY("movsx into the base: %08x\n", y);
    __asm__ volatile("movswl 0(%%eax), %%eax" : "=a"(z) : "0"(0x1120u));
    SAY("movsx word into the base: %08x\n", z);
    __asm__ volatile("movl %%eax, 4(%%eax)" : : "a"(0x1200u) : "memory");
    __asm__ volatile("movw %%ax, 2(%%eax)" : : "a"(0x1300u) : "memory");
    SAY("stored from the base: %08x %04x\n", LOW32(0x80001204u), LOW16(0x80001302u));
}
void g_null(void)
{
    volatile unsigned char *frames = 0;
    SAY("byte at null + 0xd: %u\n", frames[0xd]);
}
void g_straddle(void) { SAY("before\n"); SAY("read %u\n", LOW32(0xfffe)); }
void g_straddle_w(void) { SAY("before\n"); LOW16(0xffff) = 1; SAY("after\n"); }
void g_host(void) { SAY("before\n"); SAY("read %u\n", host_low_read()); }
void g_unserved(void) { SAY("before\n"); __asm__ volatile("negl 0x1000" : : : "memory", "cc"); SAY("after\n"); }
void g_exec(void) { SAY("before\n"); ((void (*)(void))0x1000)(); SAY("after\n"); }
void g_a1(void) { LOW32(0x2000) = 1; LOW32(0x2004) = LOW32(0x2000) + 1; LOW8(0x2008) = 3; }
void g_a2(void) { LOW32(0x2010) = LOW32(0x2000) + 1; LOW16(0x2014) = LOW16(0x2010); }
void g_two(void)
{
    ps1_g_a1();
    ps1_g_a2();
    ps1_g_a1();
    SAY("done: %08x %08x %02x %08x %04x\n", LOW32(0x80002000u), LOW32(0x80002004u), LOW8(0x80002008u), LOW32(0x80002010u), LOW16(0x80002014u));
}
void g_second(void)
{
    DWORD old;
    VirtualProtect((void *)0x80001000u, 0x1000, PAGE_NOACCESS, &old);
    SAY("RAM page closed\n");
    SAY("read %u\n", LOW32(0x1000));
}
void g_alu(void)
{
    unsigned c1, c2, z, s, o;
    LOW32(0x80003000u) = 0xfffffffeu;
    __asm__ volatile("addl $5, 0x3000\n\tsetc %%al" : "=a"(c1) : : "memory", "cc");
    SAY("add: value %08x carry %u\n", LOW32(0x80003000u), c1 & 1);
    LOW8(0x80003010u) = 0x7f;
    __asm__ volatile("addb $1, 0x3010\n\tseto %%al" : "=a"(o) : : "memory", "cc");
    SAY("addb: value %02x overflow %u\n", LOW8(0x80003010u), o & 1);
    LOW16(0x80003020u) = 5;
    __asm__ volatile("cmpw $5, 0x3020\n\tsete %%al" : "=a"(z) : : "memory", "cc");
    SAY("cmpw: equal %u, value %04x\n", z & 1, LOW16(0x80003020u));
    LOW32(0x80003030u) = 1;
    __asm__ volatile("subl $2, 0x3030\n\tsets %%al" : "=a"(s) : : "memory", "cc");
    SAY("subl: value %08x sign %u\n", LOW32(0x80003030u), s & 1);
    __asm__ volatile("movl $3, %%ebx\n\txorl %%eax, %%eax\n\tcmpl 0x3030, %%ebx\n\tsetb %%al" : "=a"(c2) : : "ebx", "memory", "cc");
    SAY("cmp r,m: below %u\n", c2 & 1);
}
static volatile int count;
static void handler(void) { count++; }
void g_loop(void)
{
    unsigned i, bad = 0, ram_bad = 0, raced0 = port_mirror_raced, served0 = port_mirror_served_count;
    unsigned ev;
    int start;
    ps1_ResetCallback();
    ev = ps1_OpenEvent(0xf2000003u, 2, 0x1000, handler);
    ps1_EnableEvent(ev);
    ps1_StartRCnt(3);
    start = count;
    for (i = 0; i < 150000; i++) {
        unsigned slot = 0x4000 + (i & 255) * 4, v = i * 2654435761u + 7;
        LOW32(slot) = v;
        if (LOW32(slot) != v) bad++;
    }
    for (i = 0; i < 256; i++) {
        unsigned last = 150000 - 256 + i;
        if (LOW32(0x80004000u + 4 * (last & 255)) != last * 2654435761u + 7) ram_bad++;
    }
    SAY("loop: wrong reads %u, wrong words in the RAM %u, served %u, vblank handler ran %d times: %d\n", bad, ram_bad,
        port_mirror_served_count - served0, count - start, count - start >= 10);
    SAY("raced %u\n", port_mirror_raced - raced0);
}
void g_time(void)
{
    LARGE_INTEGER f, a, b;
    unsigned i, s = 0;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&a);
    for (i = 0; i < 10000; i++) s += LOW32(0x5000 + (i & 63) * 4);
    QueryPerformanceCounter(&b);
    SAY("timing: 10000 served reads took %lld us (%.2f us each); sum %u\n", (b.QuadPart - a.QuadPart) * 1000000 / f.QuadPart,
        (double)(b.QuadPart - a.QuadPart) * 1e6 / (double)f.QuadPart / 10000.0, s);
}
"""

DOMAINS_M = r"""
#include "port.h"
#include <windows.h>
extern void port_h_OpenEvent(), port_h_EnableEvent(), port_h_StartRCnt(), port_h_ResetCallback();
unsigned host_low_read(void) { return *(volatile unsigned *)0x2000; }
static const struct port_library t[] = {
    { "OpenEvent", (void *)port_h_OpenEvent, 0 }, { "EnableEvent", (void *)port_h_EnableEvent, 0 },
    { "StartRCnt", (void *)port_h_StartRCnt, 0 }, { "ResetCallback", (void *)port_h_ResetCallback, 0 }, { 0, 0, 0 } };
const struct port_domain port_domains[] = { { "mirror", t } };
const unsigned port_domain_count = 1;
static const struct port_override o[] = { { 0, 0, 0 } };
const struct port_override *const port_override_sets[] = { o };
const unsigned port_override_set_count = 1;
"""


def native(rig: L.Rig):
    exe = rig.work / "native.exe"
    proc = subprocess.run([rig.cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(L.SRC), "-o", str(exe), str(HERE / "mirror_native_test.c"),
                           str(L.SRC / "mirrorcore.c"), "-static"], capture_output=True, text=True, timeout=300)
    if proc.returncode != 0:
        yield "native-controls-build", proc.stderr.strip()[:600]
        return
    run = subprocess.run([*rig.prefix, str(exe)], capture_output=True, text=True, timeout=600)
    out = run.stdout.replace("\r\n", "\n").splitlines()
    seen = 0
    for line in out:
        m = re.match(r"(ok|FAIL) (\S+?)(?: \((\d+) checks\))?(?:: (.*))?$", line)
        if m:
            seen += 1
            yield m.group(2), None if m.group(1) == "ok" else (m.group(4) or "failed")
        elif line.startswith("native:"):
            print(line)
    yield "native-controls-ran-to-the-end-with-status-0-and-printed-their-count", None if run.returncode == 0 and seen > 40 and any(l.startswith("native:") for l in out) else f"status {run.returncode}, {seen} groups"


def launch(rig: L.Rig):
    L.VARIANTS["mirror"] = Variant("mirror", GAME_MIRROR, FUN_M, ABS_M, DOMAINS_M)

    def go(tag, name, args=None, timeout=120):
        status, lines, img, arg = rig.run(tag, program(G[name]), variant="mirror", args=args, timeout=timeout)
        s = f"start: 0x{G[name]:08x}"
        body = lines[lines.index(s) + 1:] if s in lines else lines
        return status, body

    def first_use(name, addr, kind):
        return f"mirror: {name} uses the PS1's copy of RAM at address 0 (first at 0x{addr:08x}; {kind})"

    status, body = go("rw", "g_rw")
    want = [first_use("g_rw", 0x1000, "write"), "read back ab beef deadbeef", "in the RAM ab beef deadbeef", "first and last byte of the copy: 11 22",
            "in the RAM 11 22", "last word: 01020304", "stop: main returned"]
    yield "a-served-write-and-read-of-each-size-and-the-borders-of-the-copy-reach-the-ram", verdict((status, body), (0, want))

    status, body = go("base", "g_base")
    want = [first_use("g_base", 0x1100, "read"), "destination is the base: 11223344", "partial destination inside the base: 00001044", "movzx into the base: 00000044",
            "movsx into the base: ffffff84", "movsx word into the base: ffff8123", "stored from the base: 00001200 1300", "stop: main returned"]
    yield "the-destination-that-is-the-base-register-whole-and-partly-and-a-store-from-the-base-register", verdict((status, body), (0, want))

    status, body = go("null", "g_null")
    yield "the-read-of-null-plus-0xd-returns-the-zero-of-the-copy", verdict((status, body), (0, [first_use("g_null", 0xd, "read"), "byte at null + 0xd: 0", "stop: main returned"]))

    status, body = go("alu", "g_alu")
    got = [l for l in body if not l.startswith("mirror:")]
    yield "arithmetic-with-a-memory-operand-sets-its-flags-and-the-memory", verdict((status, got), (0, [
        "add: value 00000003 carry 1", "addb: value 80 overflow 1", "cmpw: equal 1, value 0005", "subl: value ffffffff sign 1", "cmp r,m: below 1", "stop: main returned"]))

    trace = rig.work / "mirror-trace.txt"
    status, body = go("two", "g_two", args=["--trace", "--trace-file", rig.native(trace)])
    lines = trace.read_text().splitlines() if trace.exists() else []
    uses = [l for l in body if l.startswith("mirror:")]
    yield "first-use-prints-one-line-for-each-function-and-only-once", verdict((status, uses, body[-2:]), (
        0, [first_use("g_a1", 0x2000, "write"), first_use("g_a2", 0x2000, "read")], ["done: 00000001 00000002 03 00000002 0002", "stop: main returned"]))
    acc = [l for l in lines if re.match(r"mirror: g_a[12] (read|write) ", l)]
    yield "trace-writes-one-line-for-each-access-with-its-size-and-instruction", None if len(acc) == 12 and re.fullmatch(r"mirror: g_a1 write 0x00002000 4 \(at 0x[0-9a-f]{8}\)", acc[0]) else f"{len(acc)} lines: {acc[:2]!r}"
    yield "the-start-check-runs-and-says-so-with-trace", None if lines[:1] == ["mirror: start check: no page of 0x00000000..0x0000ffff is accessible"] else f"lines {lines[:2]!r}"
    counts = [l for l in lines if re.match(r"mirror: g_a[12]: \d+ reads, \d+ writes", l)]
    yield "trace-prints-the-count-per-function-at-exit", verdict(counts, ["mirror: g_a1: 2 reads, 6 writes", "mirror: g_a2: 2 reads, 2 writes"])

    status, body = go("straddle", "g_straddle")
    ok = status == 10 and body[-2] == "before" and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x0000fffe \(read\) in g_straddle \(at 0x[0-9a-f]{8}\)", body[-1])
    yield "a-read-that-straddles-0x10000-is-the-crash-line", None if ok else f"status {status}, lines {body[-3:]!r}"
    status, body = go("straddle-w", "g_straddle_w")
    ok = status == 10 and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x0000ffff \(write\) in g_straddle_w \(at 0x[0-9a-f]{8}\)", body[-1]) and "after" not in body
    yield "a-write-that-straddles-0x10000-is-the-crash-line-and-the-game-does-not-go-on", None if ok else f"status {status}, lines {body[-3:]!r}"
    status, body = go("host", "g_host")
    ok = status == 10 and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x00002000 \(read\) in \S+ \(at 0x[0-9a-f]{8}\)", body[-1]) and not any(l.startswith("mirror:") for l in body)
    yield "a-low-read-from-host-code-is-the-crash-line", None if ok else f"status {status}, lines {body[-3:]!r}"
    status, body = go("exec", "g_exec")
    ok = status == 10 and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x00001000 \(execute\) in \S+ \(at 0x[0-9a-f]{8}\)", body[-1])
    yield "an-execute-fault-at-a-low-address-is-the-crash-line", None if ok else f"status {status}, lines {body[-2:]!r}"
    status, body = go("unserved", "g_unserved")
    ok = status == 10 and re.fullmatch(r"stop: crash: the game used the PS1's RAM mirror at 0x00001000 \((read|write)\) in g_unserved with an instruction the port does not serve yet: f7 1d 00 10 00 00( [0-9a-f]{2})*", body[-1]) and "after" not in body
    yield "a-form-that-is-not-served-ends-the-program-with-the-stop-line-and-the-instructions-bytes", None if ok else f"status {status}, lines {body[-2:]!r}"
    status, body = go("second", "g_second")
    yield "a-second-fault-inside-the-handler-ends-the-program", verdict((status, body[-2:]), (10, ["RAM page closed", "stop: crash: a second fault (read 0x80001000) while the PS1's RAM mirror was being served"]))

    status, body = go("loop", "g_loop", timeout=300)
    pat = r"loop: wrong reads 0, wrong words in the RAM 0, served 300000, vblank handler ran \d+ times: 1"
    yield "many-served-accesses-with-the-vblank-timer-on-end-right-and-the-vblank-ran-meanwhile", None if status == 0 and any(re.fullmatch(pat, l) for l in body) else f"status {status}, lines {body[-3:]!r}"
    raced = [l for l in body if l.startswith("raced")]
    print(f"     info: {raced[0] if raced else 'no raced line'} (timer aimed the thread between a fault and the handler)")

    status, body = go("time", "g_time", args=["--no-interrupt"])
    t = [l for l in body if l.startswith("timing:")]
    print(f"     {t[0] if t else 'no timing line'}")
    yield "the-cost-of-10000-served-accesses-is-measured-and-printed", None if status == 0 and t else f"status {status}, lines {body[-3:]!r}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--part", default="all", choices=["native", "launch", "all"])
    parser.add_argument("--only", default="", help="run the launch cases from the first whose name contains this")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    if args.src:
        L.SRC = Path(args.src).resolve()
    L.BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostmirror-", dir=L.BUILD))
    try:
        rig = L.Rig(args.cc, args.run.split(), work)
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = 0
        gens = []
        if args.part in ("native", "all"):
            gens.append(native(rig))
        if args.part in ("launch", "all"):
            gens.append(launch(rig))
        try:
            for gen in gens:
                for name, detail in gen:
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
