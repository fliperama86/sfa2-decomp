#!/usr/bin/env python3
"""Controls for the port's sound out (iopage.c): the device that opens unless told not to, --no-audio, --audio and --audio-dump FILE.

    python3 test_hostaudio.py --cc CROSS_CC --psyz-build DIR [--run PREFIX] [--src DIR] [--only GROUP]

A program linked with PsyZ opens the machine's sound output through PsyZ (Psyz_AudioInit, once, at start) without being asked.
--no-audio opens none. --audio asks for the device by name and changes nothing in such a program. --audio-dump FILE appends 735 stereo
frames of PsyZ's model of the sound chip to a WAV file at the end of every vblank, with no audio device (with --audio as well a line
says so). Without PsyZ in the build --audio and --audio-dump are each refused at start, and a start with neither runs in silence.

NO CASE MAY OPEN A REAL DEVICE. Every child that this file starts gets SDL_AUDIODRIVER in its environment, set by the rig (the one of
test_hostiopage.py): `dummy`, or for the case of a device that cannot be opened a name that SDL does not have. The route into the child
from a Linux shell under WSL is WSLENV, which is set there too. Any start whose environment lacks the variable is refused by the control
itself, whatever its arguments (two cases show that).

The cases build the runtime with a made-up game (the rig of test_hostiopage.py, linked with PsyZ) and run the whole program. Each lists the
lines it must print and the status, checks the status first, and fails on a missing line, a line more or another status. The groups:

- the dump of a game that puts a sample into the chip's RAM, keys on voice 0 after 5 vblanks and returns after 12 (--no-interrupt, so that
  the vblanks are the game's own waits): the header's fields written out by hand, the data length 735 x 4 x 12 bytes, the frames before the
  key-on all zero, the frames after it not all zero, the file's size;
- the header after the ends: a return (the exit routines rewrite it), a stop line (mirror.c ends the process with ExitProcess, no exit
  routine runs: the header is at the last multiple of 60 vblanks, and the file holds at most 60 vblanks more), and the program killed from
  outside by its handle after a known time (the same bound, checked as a bound);
- a file in a folder that does not exist (the refusal line, status 2, nothing created); a write that fails (one line, the dump stops with
  the lengths of what reached the file, the game goes on, status 0);
- the device, with SDL's dummy driver: with no option, and with --audio, the line that the output is open, the game runs to its end, the
  model was pulled (the voice's envelope register, which only a pulled frame changes, is not zero) and PsyZ's own state says a device is
  open (Psyz_AudioIsPaused is 0); the same game with --no-audio: no line, envelope 0 and no device; a driver that does not exist, with no
  option and with --audio: the line, status 0, no device; --audio and --audio-dump: the file is written, the line, no device (PsyZ says
  so), the model pulled by the dump; --no-audio and --audio-dump: the same without the line; --audio and --no-audio, in either order:
  the usage line, status 2;
- the lock: with the device open the mutex exists, and every access of iopage.c to the model is probed (the link wraps PsyZ's routines; inside each locked call a
  worker thread tries the audio lock and must NOT get it during the call; after the game's accesses have returned, so after every unlock, the
  game joins all workers: each must finish within 2 s with exit code 0; a thread not created, a wait that fails or times out, or a handle that
  does not close is a failure with its reason); the same with CdInit called first, whose own reads and writes of the model are probed too;
- the disc's streamed audio through the whole program: the stream in the dump through the game's volumes; after nothing but CdInit the
  chip's registers as the library's CD_initvol leaves them (written out by hand) and the stream in the dump through them; no sink; a dump
  that fails while a stream plays; one definition of Psyz_CdPullSamples in the program;
- a build without PsyZ: --audio-dump and --audio each refused with its line, status 2, no file; with no option and with --no-audio the
  program runs, prints no line about sound, and both runs print the same.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases that ran behaved as required` or the number that did not.
--src names another copy of the runtime's folder (the mutants of the controls run on a scratch copy).
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
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import test_hostcd as C  # noqa: E402  (the made-up audio sectors and the expected frames)
import test_hostiopage as I  # noqa: E402
import test_hostlaunch as L  # noqa: E402
from test_hostlaunch import ABS_BASE, FUN_BASE, RAM, Variant, program  # noqa: E402

FRAME_BYTES = 735 * 4
BAD_DRIVER = "nosuchdriver"
SAMPLE = "dummy"

HOSTS = dict(I.PSYZ_HOSTS)
HOSTS.update({"audio_paused": "host_audio_paused", "lock_calls": "host_lock_calls", "lock_viol": "host_lock_viol", "lock_bad": "host_lock_bad", "lock_collect": "host_lock_collect",
              "CdInit": "port_CdInit", "CdControl": "port_CdControl", "CdMix": "port_CdMix", "CdIntToPos": "port_CdIntToPos", "xa_count": "host_xa_count"})
ABS = [(n, RAM + 0x101e00 + 0x10 * i, 1) for i, n in enumerate(HOSTS)]

GAMES: list[tuple[str, str]] = []   # (function name, body)


def game(name: str, body: str) -> str:
    GAMES.append((name, body))
    return name


VOICE0 = r"""
    put_sample(0x1000u);
    W16(0x1f801d80u, 0x3fffu);
    W16(0x1f801d82u, 0x3fffu);
    W16(0x1f801daau, 0xc000u);
    W16(0x1f801c00u + 0x00u, 0x3fffu);
    W16(0x1f801c00u + 0x02u, 0x3fffu);
    W16(0x1f801c00u + 0x04u, 0x1000u);
    W16(0x1f801c00u + 0x06u, 0x0200u);
    W16(0x1f801c00u + 0x08u, 0x00ffu);
    W16(0x1f801c00u + 0x0au, 0x0000u);
"""

# key on after 5 vblanks, return after 12
game("a_dump", "int i;\n" + VOICE0 + r"""
    for (i = 0; i < 5; i++) ps1_VSync(0);
    SAY("key-on at vblank %d\n", ps1_VSync(-1));
    W16(0x1f801d88u, 1u);
    for (i = 5; i < 12; i++) ps1_VSync(0);
    SAY("the game ends at vblank %d\n", ps1_VSync(-1));""")
# 130 waits, then a read of an address of the page that has no model: mirror.c ends the process with ExitProcess
game("a_stop", "int i;\n" + VOICE0 + r"""
    W16(0x1f801d88u, 1u);
    for (i = 0; i < 130; i++) ps1_VSync(0);
    SAY("the game ends at vblank %d\n", ps1_VSync(-1));
    __asm__ volatile("movl 0x1f801000, %%ebx" : : : "ebx", "memory");
    SAY("not reached\n");""")
# waits for 10 seconds unless the control ends it first
game("a_long", "int i;\n" + VOICE0 + r"""
    W16(0x1f801d88u, 1u);
    SAY("started\n");
    for (i = 0; i < 600; i++) ps1_VSync(0);
    SAY("the game ends\n");""")
# the device and the model: after 20 vblanks (a third of a second) read the envelope of voice 0 (only a pulled frame changes it)
game("a_device", "int i;\n" + VOICE0 + r"""
    W16(0x1f801d88u, 1u);
    for (i = 0; i < 20; i++) ps1_VSync(0);
    SAY("device paused %d\n", ps1_audio_paused());
    SAY("envelope not zero %d\n", R16(0x1f801c0cu) != 0);
    ps1_lock_collect();
    SAY("lock probes: %d calls, %d where the worker got the lock during the call, %d not clean after the release\n", ps1_lock_calls(), ps1_lock_viol(), ps1_lock_bad());""")
# the same with CdInit first, which reads and writes the model itself (the library's CD_initvol): for the probes of the lock
game("a_device_init", "int i;\n    ps1_CdInit();\n" + VOICE0 + r"""
    W16(0x1f801d88u, 1u);
    for (i = 0; i < 20; i++) ps1_VSync(0);
    SAY("device paused %d\n", ps1_audio_paused());
    SAY("envelope not zero %d\n", R16(0x1f801c0cu) != 0);
    ps1_lock_collect();
    SAY("lock probes: %d calls, %d where the worker got the lock during the call, %d not clean after the release\n", ps1_lock_calls(), ps1_lock_viol(), ps1_lock_bad());""")
game("a_wfail", "int i;\n" + VOICE0 + r"""
    for (i = 0; i < 6; i++) ps1_VSync(0);
    SAY("the game goes on at vblank %d\n", ps1_VSync(-1));""")

# The disc's streamed audio on the real path: CdInit, then the chip is set to mix the CD input at full volume (after CdInit, which writes
# the CD volume and the control itself), the disc layer is told the filter (file 1,
# channel 3), the mode (double speed, XA on, filter on) and the CdMix volumes, ReadS starts at the sector AUDIO_LBA of the made-up image, and the
# game waits 40 vblanks (29,400 frames of the dump; the 8 sectors played make 18,816). It prints the counts of the disc layer.
AUDIO_LBA = 200
XA_BODY = r"""
    unsigned char pos[4], res[8], vol[4] = { %s }, mode[1] = { 0xc8 }, filt[2] = { 1, 3 };
    int i;
    ps1_CdInit();
    W16(0x1f801daau, 0xc001u);
    W16(0x1f801d80u, 0x3fffu);
    W16(0x1f801d82u, 0x3fffu);
    W16(0x1f801db0u, 0x7fffu);
    W16(0x1f801db2u, 0x7fffu);
    ps1_CdControl(0xd, filt, res);
    ps1_CdControl(0xe, mode, res);
    ps1_CdMix(vol);
    ps1_CdIntToPos(%d, pos);
    ps1_CdControl(0x1b, pos, res);
    for (i = 0; i < 40; i++) ps1_VSync(0);
    SAY("sectors: played %%d held %%d refused %%d skipped %%d silent %%d\n", ps1_xa_count(0), ps1_xa_count(1), ps1_xa_count(2), ps1_xa_count(3), ps1_xa_count(4));
    SAY("frames: queued in all %%d pulled %%d in the queue %%d\n", ps1_xa_count(5), ps1_xa_count(6), ps1_xa_count(7));
"""
for _name, _vol in (("a_xa_wfail", "128, 0, 128, 0"), ("a_xa", "128, 0, 128, 0"), ("a_xa_half", "64, 0, 64, 0"), ("a_xa_mixed", "100, 30, 90, 20")):
    game(_name, XA_BODY % (_vol, AUDIO_LBA))

# The same stream with nothing but CdInit between the program's start and the read: no register of the chip is written by the game and
# CdMix is not called, as before the game's logo. What lets the stream through the chip is what CdInit does (the library's CD_initvol).
# The game prints the chip's registers before CdInit and after it, read as the game reads them.
XA_INIT_BODY = r"""
    unsigned char pos[4], res[8], mode[1] = { 0xc8 }, filt[2] = { 1, 3 };
    int i;
    SAY("chip before: main %%04x %%04x cd %%04x %%04x control %%04x\n", R16(0x1f801d80u), R16(0x1f801d82u), R16(0x1f801db0u), R16(0x1f801db2u), R16(0x1f801daau));
    ps1_CdInit();
    SAY("chip after: main %%04x %%04x cd %%04x %%04x control %%04x\n", R16(0x1f801d80u), R16(0x1f801d82u), R16(0x1f801db0u), R16(0x1f801db2u), R16(0x1f801daau));
    ps1_CdControl(0xd, filt, res);
    ps1_CdControl(0xe, mode, res);
    ps1_CdIntToPos(%d, pos);
    ps1_CdControl(0x1b, pos, res);
    for (i = 0; i < 40; i++) ps1_VSync(0);
    SAY("sectors: played %%d held %%d refused %%d skipped %%d silent %%d\n", ps1_xa_count(0), ps1_xa_count(1), ps1_xa_count(2), ps1_xa_count(3), ps1_xa_count(4));
    SAY("frames: queued in all %%d pulled %%d in the queue %%d\n", ps1_xa_count(5), ps1_xa_count(6), ps1_xa_count(7));
"""
game("a_xa_init", XA_INIT_BODY % AUDIO_LBA)
# CdInit with a main volume that the game set before it, on one side and then on the other: through the model, CdInit must read the two
# registers of the main volume and no others
game("a_init_main", r"""
    W16(0x1f801d80u, 0x1234u);
    ps1_CdInit();
    SAY("left set: main %04x %04x cd %04x %04x control %04x\n", R16(0x1f801d80u), R16(0x1f801d82u), R16(0x1f801db0u), R16(0x1f801db2u), R16(0x1f801daau));
    W16(0x1f801d80u, 0u);
    W16(0x1f801d82u, 2u);
    W16(0x1f801db0u, 0x0111u);
    W16(0x1f801daau, 0x8000u);
    ps1_CdInit();
    SAY("right set: main %04x %04x cd %04x %04x control %04x\n", R16(0x1f801d80u), R16(0x1f801d82u), R16(0x1f801db0u), R16(0x1f801db2u), R16(0x1f801daau));
    W16(0x1f801d82u, 0u);
    ps1_CdInit();
    SAY("neither set: main %04x %04x cd %04x %04x control %04x\n", R16(0x1f801d80u), R16(0x1f801d82u), R16(0x1f801db0u), R16(0x1f801db2u), R16(0x1f801daau));""")

FUN = FUN_BASE + [(n, RAM + 0x101100 + 0x10 * i, n) for i, (n, _) in enumerate(GAMES)]
G = {n: a for n, a, _ in FUN}
GAME_C = I.HEADER + I.PSYZ_HEADER + "\nstatic void unused_names(void) { (void)wait_ev; }\n" + "".join("void %s(void)\n{\n%s\n}\n" % (n, b) for n, b in GAMES)
GAME_C = GAME_C.replace("extern int ps1_EnterCriticalSection(void)", "extern int ps1_audio_paused(void), ps1_lock_calls(void), ps1_lock_viol(void), ps1_lock_bad(void), ps1_lock_collect(void);\nextern int ps1_CdInit(void), ps1_CdControl(int, unsigned char *, unsigned char *), ps1_CdMix(void *), ps1_xa_count(int);\nextern unsigned char *ps1_CdIntToPos(int, unsigned char *);\nextern int ps1_EnterCriticalSection(void)", 1)

WRAPPED = ["Psyz_SpuRead", "Psyz_SpuWrite", "Psyz_SpuFifoWriteBulk", "Psyz_SpuMemRead", "Psyz_SpuSetTransferAddr", "Psyz_SpuGetTransferAddr"]


def domains(extra: str = "") -> str:
    decls = "".join(f"extern void {h}();\n" for h in HOSTS.values() if h.startswith("port_h_") and h != "port_h_DMACallback")   # the disc layer's port_Cd* come from cd.h
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in HOSTS.items())
    return f"""
#include "port.h"
#include "cd.h"
#include <stdio.h>
#include <windows.h>
#include <psyz.h>
#include <psyz/spu.h>
#include <psyz/audio.h>
{decls}
extern int Psyz_AudioIsPaused(void);
int host_pull_nonzero(int frames) {{ (void)frames; return 0; }}
int host_chip_ram(int offset) {{ return Psyz_SpuGetRam()[offset]; }}
int host_audio_paused(void) {{ return Psyz_AudioIsPaused(); }}
int host_xa_count(int which)
{{
    struct port_xa_counts c;
    port_xa_counts_get(&c);
    switch (which) {{
    case 0: return (int)c.played;
    case 1: return (int)c.held;
    case 2: return (int)c.refused;
    case 3: return (int)c.skipped;
    case 4: return (int)c.silent;
    case 5: return (int)c.frames;
    case 6: return (int)c.pulled;
    default: return (int)c.queued;
    }}
}}
int host_lock_calls(void);
int host_lock_viol(void);
int host_lock_bad(void);
int host_lock_collect(void);
unsigned host_io_read(void) {{ return *(volatile unsigned *)0x1f801014; }}
{extra}
static const struct port_library t[] = {{ {rows} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "kernel", t }} }};
const unsigned port_domain_count = 1;
static const struct port_override o[] = {{ {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


# The lock probe, in three parts. Each routine of PsyZ that iopage.c calls on the model is wrapped by the link (--wrap). The wrapper runs inside
# the locked call: it starts a worker that tries the audio lock (one record per call, in static storage, so nothing on a stack outlives its
# frame), waits 15 ms, and notes whether the worker got the lock during the call. A correct lock holds the worker off: not acquired. The wrapper
# cannot join the worker: it returns to iopage.c, which then unlocks. So the join is host_lock_collect(), which the made-up game calls after its
# accesses have returned, when every unlock has happened. There each worker must finish within 2 s (a correct program finishes at once) with exit
# code 0, or the case fails with the reason. A thread that cannot be created, a wait that fails and a handle that cannot be closed are failures too.
PROBE = r"""
#define MAXP 64
struct rec { HANDLE h; volatile int acquired, done; };
static struct rec recs[MAXP];
static volatile int calls, viol, bad, nrec;
static DWORD WINAPI probe_thread(LPVOID p)
{
    struct rec *r = (struct rec *)p;
    Psyz_AudioLock();
    r->acquired = 1;
    Psyz_AudioUnlock();
    r->done = 1;
    return 0;
}
static void probe(void)
{
    struct rec *r;
    if (nrec >= MAXP) { bad++; printf("lock probe: more than %d probed calls\n", MAXP); fflush(stdout); return; }
    r = &recs[nrec++];
    calls++;
    r->h = CreateThread(NULL, 0, probe_thread, r, 0, NULL);
    if (!r->h) { bad++; printf("lock probe: CreateThread failed, error %lu\n", (unsigned long)GetLastError()); fflush(stdout); return; }
    Sleep(15);
    if (r->acquired) viol++;
}
int host_lock_collect(void)
{
    int i;
    for (i = 0; i < nrec; i++) {
        DWORD w, code = 1;
        if (!recs[i].h) continue;
        w = WaitForSingleObject(recs[i].h, 2000);
        if (w != WAIT_OBJECT_0) { bad++; printf("lock probe %d: the worker had not finished 2 s after the unlock (wait returned %lu)\n", i, (unsigned long)w); fflush(stdout); continue; }
        if (!GetExitCodeThread(recs[i].h, &code) || code != 0 || !recs[i].done) { bad++; printf("lock probe %d: the worker ended with code %lu, done %d\n", i, (unsigned long)code, recs[i].done); fflush(stdout); }
        if (!CloseHandle(recs[i].h)) { bad++; printf("lock probe %d: CloseHandle failed\n", i); fflush(stdout); }
        recs[i].h = NULL;
    }
    return bad;
}
unsigned short __real_Psyz_SpuRead(unsigned int);
void __real_Psyz_SpuWrite(unsigned int, unsigned short);
void __real_Psyz_SpuFifoWriteBulk(const void *, unsigned);
void __real_Psyz_SpuMemRead(unsigned, void *, unsigned);
void __real_Psyz_SpuSetTransferAddr(unsigned);
unsigned __real_Psyz_SpuGetTransferAddr(void);
unsigned short __wrap_Psyz_SpuRead(unsigned int a) { probe(); return __real_Psyz_SpuRead(a); }
void __wrap_Psyz_SpuWrite(unsigned int a, unsigned short v) { probe(); __real_Psyz_SpuWrite(a, v); }
void __wrap_Psyz_SpuFifoWriteBulk(const void *p, unsigned n) { probe(); __real_Psyz_SpuFifoWriteBulk(p, n); }
void __wrap_Psyz_SpuMemRead(unsigned a, void *p, unsigned n) { probe(); __real_Psyz_SpuMemRead(a, p, n); }
void __wrap_Psyz_SpuSetTransferAddr(unsigned a) { probe(); __real_Psyz_SpuSetTransferAddr(a); }
unsigned __wrap_Psyz_SpuGetTransferAddr(void) { probe(); return __real_Psyz_SpuGetTransferAddr(); }
"""
# A write of the dump that fails: the third call that writes 735 frames of 4 bytes returns 0.
WFAIL = r"""
size_t __real_fwrite(const void *, size_t, size_t, FILE *);
size_t __wrap_fwrite(const void *p, size_t s, size_t n, FILE *f)
{
    static int c;
    if (s == 4 && n == 735 && ++c == 3) return 0;
    return __real_fwrite(p, s, n, f);
}
"""
NOPROBE = r"""
int host_lock_calls(void) { return 0; }
int host_lock_viol(void) { return 0; }
int host_lock_bad(void) { return 0; }
int host_lock_collect(void) { return 0; }
"""
PROBE_ACCESSORS = r"""
int host_lock_calls(void) { return calls; }
int host_lock_viol(void) { return viol; }
int host_lock_bad(void) { return bad; }
"""
VARIANT_C = {"audio": domains(NOPROBE), "lock": domains(PROBE + PROBE_ACCESSORS), "wfail": domains(NOPROBE + WFAIL)}
VARIANT_FLAGS = {"lock": [f"-Wl,--wrap={n}" for n in WRAPPED], "wfail": ["-Wl,--wrap=fwrite"]}


wsl_env = I.sound_env


class AudioRig(I.PsyzRig):
    """The PsyZ rig with nothing put before a program's arguments (each case names its own sound options). Its guard, the PsyZ rig's,
    lets a child start with the dummy driver or with the name that SDL does not have, and with nothing else, whatever the arguments."""

    def __init__(self, cc, prefix, work, info):
        super().__init__(cc, prefix, work, info)
        self.env = wsl_env(SAMPLE)
        self.drivers = (SAMPLE, BAD_DRIVER)
        self.first = []

    def program_for(self, variant, pin, old_link=False):
        saved = self.libs
        self.libs = saved + VARIANT_FLAGS.get(variant, [])
        try:
            return super().program_for(variant, pin, old_link)
        finally:
            self.libs = saved



def split(lines: list[str]):
    """(the audio lines printed at start, the lines after `start:`)."""
    at = next((i for i, l in enumerate(lines) if l.startswith("start: 0x")), None)
    if at is None:
        return None, lines
    return [l for l in lines[:at] if l.startswith("audio:")], lines[at + 1:]


def expect(rig, status, lines, want_status, want_audio, want_body):
    """None, or why not: status first, then the audio lines of the start, then the game's lines (the last is `stop: main returned`)."""
    if status != want_status:
        return f"status {status}, wanted {want_status}; lines {lines[-3:]!r}; stderr {(rig.last or {}).get('stderr', [])[-3:]!r}"
    audio, body = split(lines)
    if audio is None:
        return f"no `start:` line in {lines!r}"
    if audio != want_audio:
        return f"audio lines {audio!r}, wanted {want_audio!r}"
    return None if body == want_body else f"lines {body!r}, wanted {want_body!r}"


def hdr(dlen: int) -> bytes:
    """The header of the WAV file, written out by hand: 16 bits, stereo, 44,100 Hz, little endian."""
    return (b"RIFF" + struct.pack("<I", 36 + dlen) + b"WAVE" + b"fmt " + struct.pack("<IHHIIHH", 16, 1, 2, 44100, 176400, 4, 16) + b"data" + struct.pack("<I", dlen))


def header_problem(data: bytes, dlen: int):
    return None if data[:44] == hdr(dlen) else f"header {data[:44].hex()}, wanted the one for {dlen} data bytes: {hdr(dlen).hex()}"


OPEN = "audio: sound output open, 44100 Hz stereo"
FAILED_OPEN = "audio: the machine's sound output could not be opened; the game runs without sound"
BOTH = "audio: --audio-dump and --audio together: the file is written and no sound device is opened"
WFAIL_LINE = "audio: writing the dump failed; the dump stops and the game goes on"
MAIN = "stop: main returned"


def run(rig, fn, args, env=None, tag="a", variant="audio", timeout=120):
    old = rig.env
    if env is not None:
        rig.env = env
    try:
        status, lines, _, _ = rig.run(tag[:40], program(G[fn]), variant=variant, args=args, timeout=timeout)
    finally:
        rig.env = old
    return status, lines


def cases(rig, only: str, rig_nopsyz):
    L.VARIANTS["audio"] = Variant("audio", GAME_C, FUN, ABS_BASE + ABS, VARIANT_C["audio"])
    L.VARIANTS["lock"] = Variant("lock", GAME_C, FUN, ABS_BASE + ABS, VARIANT_C["lock"])
    L.VARIANTS["wfail"] = Variant("wfail", GAME_C, FUN, ABS_BASE + ABS, VARIANT_C["wfail"])
    work = rig.work
    NI = "--no-interrupt"

    def want(name):
        return not only or only in name

    def native(p):
        return rig.native(p)

    # ---- the control's own environment ----
    if want("environment"):
        e = wsl_env(SAMPLE)
        ok = e["SDL_AUDIODRIVER"] == SAMPLE and ("SDL_AUDIODRIVER" in e.get("WSLENV", "").split(":") or not shutil.which("wslpath"))
        bad = wsl_env(BAD_DRIVER)["SDL_AUDIODRIVER"] == BAD_DRIVER
        saved, rig.env = rig.env, dict(os.environ)
        rig.env.pop("SDL_AUDIODRIVER", None)
        raised = {}
        try:
            for name, a in (("asked", ["--audio"]), ("plain", [])):
                try:
                    rig.guard(a)
                    raised[name] = False
                except AssertionError:
                    raised[name] = True
        finally:
            rig.env = saved
        print("     the child's environment: SDL_AUDIODRIVER=%s WSLENV=%s" % (rig.env.get("SDL_AUDIODRIVER"), rig.env.get("WSLENV")), flush=True)
        yield "the-environment-of-every-child-has-the-dummy-driver-and-the-route-through-wslenv", None if (ok and bad and rig.env["SDL_AUDIODRIVER"] == SAMPLE) else f"environment {e.get('SDL_AUDIODRIVER')!r} {e.get('WSLENV')!r}"
        yield "the-control-refuses-to-start-with-audio-when-the-driver-is-not-set", None if raised["asked"] else "the guard let a start with --audio and no driver through"
        yield "the-control-refuses-to-start-with-no-option-when-the-driver-is-not-set", None if raised["plain"] else "the guard let a start with no option and no driver through"

    # ---- the dump ----
    if want("dump"):
        f = work / "d.wav"
        f.unlink(missing_ok=True)
        status, lines = run(rig, "a_dump", [NI, "--audio-dump", native(f)], tag="dump")
        why = expect(rig, status, lines, 0, [], ["key-on at vblank 5", "the game ends at vblank 12", MAIN])
        if why is None:
            b = f.read_bytes()
            dlen = FRAME_BYTES * 12
            data = b[44:]
            why = (header_problem(b, dlen)
                   or (None if len(b) == 44 + dlen else f"file size {len(b)}, wanted {44 + dlen}")
                   or (None if not any(data[:FRAME_BYTES * 5]) else "a sample before the key-on is not zero")
                   or (None if any(data[FRAME_BYTES * 5:FRAME_BYTES * 6]) else "all samples of the vblank after the key-on are zero")
                   or (None if any(data[FRAME_BYTES * 5:]) else "all samples after the key-on are zero"))
            if why is None:
                nz = sum(1 for i in range(0, len(data), 2) if data[i] or data[i + 1])
                print("     data %d bytes, %d of %d samples not zero" % (len(data), nz, len(data) // 2), flush=True)
        yield "the-dump-has-the-header-and-735-frames-per-vblank-silent-before-the-key-on-and-not-after", why

    if want("stop"):
        f = work / "s.wav"
        f.unlink(missing_ok=True)
        status, lines = run(rig, "a_stop", [NI, "--audio-dump", native(f)], tag="stop")
        audio, body = split(lines)
        why = None
        if status != 10:
            why = f"status {status}, wanted 10; lines {lines[-3:]!r}"
        elif audio != [] or len(body) != 2 or body[0] != "the game ends at vblank 130" or not re.fullmatch(I.stop_re(0x1F801000, "read", "a_stop", "with an address that no model of the page claims (0x1f801000, 4 bytes)", "8b 1d " + I.le(0x1F801000)), body[1]):
            why = f"audio {audio!r} lines {body!r}"
        else:
            b = f.read_bytes()
            dlen = FRAME_BYTES * 120   # the header was last rewritten at vblank 120 (60 and 120); the program ends at 130
            real = len(b) - 44
            why = (header_problem(b, dlen)
                   or (None if dlen <= real <= FRAME_BYTES * 130 else f"the file holds {real} data bytes, wanted between {dlen} and {FRAME_BYTES * 130}")
                   or (None if real - dlen <= FRAME_BYTES * 60 else "the file is more than 60 vblanks ahead of the header"))
            if why is None:
                print("     after the stop: header says %d data bytes, the file holds %d (%d vblanks more)" % (dlen, real, (real - dlen) // FRAME_BYTES), flush=True)
        yield "after-a-stop-line-the-header-is-at-the-last-multiple-of-60-vblanks-and-the-file-at-most-60-vblanks-more", why

    if want("killed"):
        f = work / "k.wav"
        f.unlink(missing_ok=True)
        rig.guard(["--audio-dump"])
        exe = rig.program_for("audio", program(G["a_long"]))
        img = L.Image({"SYSTEM.CNF;1": L.CNF, "SLPS_004.15;1": program(G["a_long"])})
        disc = work / "k.bin"
        img.write(disc)
        proc = subprocess.Popen([*rig.prefix, str(exe), "--audio-dump", native(f), native(disc)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=rig.env)
        time.sleep(3.5)
        alive = proc.poll() is None
        proc.kill()   # by the handle of the child
        try:
            out, _ = proc.communicate(timeout=30)
        except subprocess.TimeoutExpired:
            out = ""
        sizes = []
        for _ in range(3):   # the child is gone when the file stops growing
            sizes.append(f.stat().st_size if f.exists() else -1)
            time.sleep(0.6)
        b = f.read_bytes() if f.exists() else b""
        dlen = struct.unpack("<I", b[40:44])[0] if len(b) >= 44 else -1
        real = len(b) - 44
        why = None
        if not alive:
            why = "the child had ended before the kill: %r" % out[-200:]
        elif len(set(sizes)) != 1:
            why = f"the file kept growing after the kill: {sizes}"
        else:
            why = (header_problem(b, dlen)
                   or (None if dlen % (FRAME_BYTES * 60) == 0 and dlen >= FRAME_BYTES * 60 else f"the header's data length {dlen} is not a multiple of 60 vblanks, at least one")
                   or (None if 0 <= real - dlen <= FRAME_BYTES * 60 else f"the file holds {real} data bytes, the header {dlen}: the bound of 60 vblanks ({FRAME_BYTES * 60}) is broken"))
        print("     killed after 3.5 s: header says %d data bytes (%d vblanks), the file holds %d" % (dlen, dlen // FRAME_BYTES if dlen > 0 else -1, real), flush=True)
        yield "ended-from-outside-by-its-handle-the-header-is-at-most-60-vblanks-behind-the-file", why

    if want("folder"):
        d = work / "nodir" / "x.wav"
        status, lines = run(rig, "a_dump", ["--audio-dump", native(d)], tag="folder")
        yield ("a-file-in-a-folder-that-does-not-exist-refuses-the-start-and-creates-nothing",
               f"status {status}, wanted 2; lines {lines!r}" if status != 2 else
               (None if lines == [f"refused: --audio-dump: cannot open {native(d)} for writing"] and not d.parent.exists() else f"lines {lines!r}, exists {d.parent.exists()}"))

    if want("write"):
        f = work / "w.wav"
        f.unlink(missing_ok=True)
        status, lines = run(rig, "a_wfail", [NI, "--audio-dump", native(f)], tag="wfail", variant="wfail")
        why = expect(rig, status, lines, 0, [], [WFAIL_LINE, "the game goes on at vblank 6", MAIN])
        if why is None:
            b = f.read_bytes()
            why = header_problem(b, FRAME_BYTES * 2) or (None if len(b) == 44 + FRAME_BYTES * 2 else f"file size {len(b)}, wanted {44 + FRAME_BYTES * 2}")
        yield "a-write-that-fails-prints-one-line-stops-the-dump-with-the-lengths-of-what-arrived-and-the-game-goes-on", why

    # ---- the device ----
    if want("device"):
        idle = ["device paused 1", "envelope not zero 0", "lock probes: 0 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN]
        play = ["device paused 0", "envelope not zero 1", "lock probes: 0 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN]
        status, lines = run(rig, "a_device", [NI], tag="plain")
        yield "with-no-option-the-dummy-device-is-opened-and-the-model-is-pulled", expect(rig, status, lines, 0, [OPEN], play)
        status, lines = run(rig, "a_device", [NI, "--no-audio"], tag="nodev")
        yield "with-no-audio-no-device-is-open-no-line-is-printed-and-the-model-is-not-pulled", expect(rig, status, lines, 0, [], idle)
        status, lines = run(rig, "a_device", [NI, "--audio"], tag="dummy")
        yield "audio-with-the-dummy-device-opens-it-and-the-model-is-pulled", expect(rig, status, lines, 0, [OPEN], play)
        status, lines = run(rig, "a_device", [NI], env=wsl_env(BAD_DRIVER), tag="plainbad")
        yield "with-no-option-a-driver-that-does-not-exist-is-a-line-and-the-game-runs-without-sound", expect(rig, status, lines, 0, [FAILED_OPEN], idle)
        status, lines = run(rig, "a_device", [NI, "--audio"], env=wsl_env(BAD_DRIVER), tag="baddev")
        yield "audio-with-a-driver-that-does-not-exist-is-a-line-and-the-game-runs-without-sound", expect(rig, status, lines, 0, [FAILED_OPEN], idle)
        for order in (["--audio", "--no-audio"], ["--no-audio", "--audio"]):
            status, lines = run(rig, "a_device", [NI, *order], tag="clash")
            yield ("audio-and-no-audio-together-are-the-usage-line-(%s-first)" % order[0].lstrip("-")), (
                f"status {status}, wanted 2; lines {lines!r}" if status != 2 else
                (None if len(lines) == 2 and lines[0].startswith("usage: ") and "[--audio | --no-audio]" in lines[0] else f"lines {lines!r}, wanted the two usage lines"))
        f = work / "b.wav"
        f.unlink(missing_ok=True)
        status, lines = run(rig, "a_device", [NI, "--audio", "--audio-dump", native(f)], tag="both")
        why = expect(rig, status, lines, 0, [BOTH], ["device paused 1", "envelope not zero 1", "lock probes: 0 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN])
        if why is None:
            b = f.read_bytes()
            dlen = len(b) - 44
            why = header_problem(b, dlen) or (None if dlen > 0 and dlen % FRAME_BYTES == 0 else f"data length {dlen} is not a whole number of vblanks")
        yield "both-options-write-the-file-and-open-no-device-and-say-so", why
        for label, more in (("the-dump-alone", []), ("no-audio-and-the-dump", ["--no-audio"])):
            f.unlink(missing_ok=True)
            status, lines = run(rig, "a_device", [NI, *more, "--audio-dump", native(f)], tag="dumpdev")
            why = expect(rig, status, lines, 0, [], ["device paused 1", "envelope not zero 1", "lock probes: 0 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN])
            if why is None:
                b = f.read_bytes()
                dlen = len(b) - 44
                why = header_problem(b, dlen) or (None if dlen > 0 and dlen % FRAME_BYTES == 0 else f"data length {dlen} is not a whole number of vblanks")
            yield f"{label}-writes-the-file-opens-no-device-and-prints-no-line-about-it", why
        # every access to the model is made with the lock held: 19 writes of the game and 1 read (counted by hand)
        status, lines = run(rig, "a_device", [NI], tag="lock", variant="lock")
        yield "with-the-device-open-every-access-of-iopage-to-the-model-is-made-with-the-audio-lock-held", expect(rig, status, lines, 0, [OPEN], ["device paused 0", "envelope not zero 1", "lock probes: 20 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN])
        # the same game with CdInit before it. CdInit's accesses to the model, counted by hand from the library's routine: the main volume holds
        # 0 on both sides at the start, so 2 reads (its two sides) and 5 writes (the main volume, the CD volume, the control): 7 more, 27
        status, lines = run(rig, "a_device_init", [NI], tag="lockinit", variant="lock")
        yield "the-accesses-of-cdinit-to-the-model-are-made-with-the-audio-lock-held-too", expect(rig, status, lines, 0, [OPEN], ["device paused 0", "envelope not zero 1", "lock probes: 27 calls, 0 where the worker got the lock during the call, 0 not clean after the release", MAIN])

    # ---- the disc's streamed audio ----
    if want("stream"):
        yield from stream_cases(rig, work)

    # ---- no PsyZ ----
    if want("nopsyz"):
        f = work / "n.wav"
        f.unlink(missing_ok=True)
        for opt, args in [("--audio-dump", ["--audio-dump", native(f)]), ("--audio", ["--audio"])]:
            status, lines, _, _ = rig_nopsyz.run("nopsyz", program(L.ENTRY_C), args=args, timeout=60)
            line = f"refused: {opt} needs a build with PsyZ (sound out is PsyZ's model of the chip)"
            yield ("a-build-without-psyz-refuses-%s" % opt.lstrip("-")), (f"status {status}, wanted 2; lines {lines!r}" if status != 2 else (None if lines == [line] and not f.exists() else f"lines {lines!r}, wanted {[line]!r}; file exists {f.exists()}"))
        # with no option, and with --no-audio, a build without PsyZ runs and says nothing about sound (the disc's name differs, so one tag)
        runs = [rig_nopsyz.run("nopsyzq", program(L.ENTRY_C), args=a, timeout=60)[:2] for a in ([], ["--no-audio"])]
        started = [any(l.startswith("start: 0x") for l in lines) for _, lines in runs]
        said = [l for _, lines in runs for l in lines if l.startswith(("audio:", "refused:", "usage:"))]
        yield "a-build-without-psyz-runs-in-silence-with-no-option-and-with-no-audio-and-prints-the-same", (
            None if runs[0] == runs[1] and all(started) and not said else f"no option: {runs[0]!r}; --no-audio: {runs[1]!r}")


# ---- the disc's streamed audio on the real path ----

STREAM_ORDER = [0, 1, "b", 2, 3, "b", 4, 5, "b", 6, 7, "b"]   # the made-up stream: 8 sectors of channel 3 (played), 4 of channel 4 (the filter lets them go)


def stream_disc(rig, fn, path, order=None):
    """Write the made-up image: the game's program, then zero sectors up to AUDIO_LBA, then the stream. Returns the played sectors, in order."""
    prog = program(G[fn])
    img = L.Image({"SYSTEM.CNF;1": L.CNF, "SLPS_004.15;1": prog})
    assert img.end <= AUDIO_LBA, f"the image ends at sector {img.end}, after the stream's start"
    img.write(path)
    raw = bytearray(path.read_bytes())
    raw += bytes(2352 * (AUDIO_LBA - img.end))
    played = []
    for k, tag in enumerate(STREAM_ORDER if order is None else order):
        if tag == "b":
            raw += C.xa_sector(900 + k, channel=4)
        else:
            sec = C.xa_sector(800 + tag)
            played.append(sec)
            raw += sec
    path.write_bytes(bytes(raw))
    return played


XW_STREAM = f"audio: a stream starts at sector {AUDIO_LBA}: file 1 channel 3, stereo, 37800 Hz"
# by hand: the dump fails at the end of the third vblank; the drive gave 2 + 3 + 2 = 7 sectors by then (double speed, 5 fifths a vblank at a cost of 2), the other 33 are silent
XW_SECTORS = "sectors: played 7 held 0 refused 0 skipped 0 silent 33"
# 7 sectors of 2,352 frames each; PsyZ pulled 4,096 (the size of its own ring) before the stop; the queue is empty after it
XW_FRAMES = "frames: queued in all 16464 pulled 4096 in the queue 0"


def chip_out(frames, cd=0x7fff):
    """What the model's mixer makes of CD frames with the CD input on, the CD volume `cd` on both sides (0x7fff: what the games above set),
    the main volume 0x3fff, no voice and no reverb, by the sums in PsyZ's psyz_spu.c: (frame * cd) >> 15, then (sum * 0x3fff) >> 14, clamped
    to 16 bits."""
    def one(v):
        a = (v * cd) >> 15
        return max(-32768, min(32767, (a * 0x3fff) >> 14))
    return [(one(l), one(r)) for l, r in frames]


def nm_of(rig):
    cc = rig.cc
    return cc[:-3] + "nm" if cc.endswith("gcc") else "nm"


def stream_cases(rig, work):
    NI = "--no-interrupt"
    L.VARIANTS["audio"] = Variant("audio", GAME_C, FUN, ABS_BASE + ABS, VARIANT_C["audio"])

    def go(fn, args, tag, order=None, variant="audio"):
        disc = work / f"{tag}.bin"
        played = stream_disc(rig, fn, disc, order)
        exe = rig.program_for(variant, program(G[fn]))
        status, out = rig.start(exe, [*args, rig.native(disc)], 120)
        return status, out.replace("\r\n", "\n").splitlines(), played

    for fn, vol, label in (("a_xa", (128, 0, 128, 0), "unity"), ("a_xa_half", (64, 0, 64, 0), "half"), ("a_xa_mixed", (100, 30, 90, 20), "mixed")):
        f = work / f"x_{label}.wav"
        f.unlink(missing_ok=True)
        status, lines, played = go(fn, [NI, "--audio-dump", rig.native(f)], f"x_{label}")
        want = C.volumes(C.expected_frames(played), vol)
        mixed = chip_out(want)
        n = len(want)
        why = expect(rig, status, lines, 0, [], [f"audio: a stream starts at sector {AUDIO_LBA}: file 1 channel 3, stereo, 37800 Hz", f"sectors: played 8 held 0 refused 0 skipped 4 silent 0", f"frames: queued in all {n} pulled {n} in the queue 0", MAIN])
        if why is None:
            b = f.read_bytes()
            data = b[44:]
            frames = list(struct.iter_unpack("<hh", data[: len(data) // 4 * 4]))
            first = next((i for i, fr in enumerate(frames) if fr != (0, 0)), None)
            if first is None:
                why = "the dump is all zero"
            elif mixed[0] == (0, 0):
                why = "the made-up stream starts with a silent frame, so the control cannot find its start"
            elif frames[first:first + n] != mixed:
                k = next((i for i in range(n) if first + i >= len(frames) or frames[first + i] != mixed[i]), None)
                why = f"the dump differs from the expected frames at frame {k} of {n}: {frames[first + k] if k is not None and first + k < len(frames) else None} vs {mixed[k] if k is not None else None}"
            else:
                print("     %s: %d frames of the stream found in the dump at frame %d, all equal to the mixer's sums (tolerance 0); the dump holds %d frames" % (label, n, first, len(frames)), flush=True)
                if any(fr != (0, 0) for fr in frames[first + n:]):
                    why = "a frame after the stream's end is not zero"
        yield f"the-streamed-audio-reaches-the-dump-as-the-reference-frames-through-the-volumes-({label})", why

    # nothing but CdInit: the chip's registers before and after it, written out by hand from the library's CD_initvol, and the stream in the
    # dump through the CD volume 0x3fff and the main volume 0x3fff that it leaves (about half the stream's own level)
    f = work / "x_init.wav"
    f.unlink(missing_ok=True)
    status, lines, played = go("a_xa_init", [NI, "--audio-dump", rig.native(f)], "x_init")
    want = C.expected_frames(played)
    mixed = chip_out(want, cd=0x3fff)
    n = len(want)
    why = expect(rig, status, lines, 0, [], ["chip before: main 0000 0000 cd 0000 0000 control 0000", "chip after: main 3fff 3fff cd 3fff 3fff control c001",
                                             f"audio: a stream starts at sector {AUDIO_LBA}: file 1 channel 3, stereo, 37800 Hz", "sectors: played 8 held 0 refused 0 skipped 4 silent 0",
                                             f"frames: queued in all {n} pulled {n} in the queue 0", MAIN])
    if why is None:
        data = f.read_bytes()[44:]
        frames = list(struct.iter_unpack("<hh", data[: len(data) // 4 * 4]))
        first = next((i for i, fr in enumerate(frames) if fr != (0, 0)), None)
        loud = max(abs(x) for fr in mixed for x in fr)
        if first is None:
            why = "the dump is all zero: the stream did not get through the chip"
        elif mixed[0] == (0, 0) or loud < 1000:
            why = f"the expected frames are too quiet to show anything (first {mixed[0]}, largest {loud})"
        elif frames[first:first + n] != mixed:
            k = next((i for i in range(n) if first + i >= len(frames) or frames[first + i] != mixed[i]), None)
            why = f"the dump differs from the expected frames at frame {k} of {n}: {frames[first + k] if k is not None and first + k < len(frames) else None} vs {mixed[k] if k is not None else None}"
        elif mixed == chip_out(want):
            why = "the expected frames are the ones of the CD volume 0x7fff too, so the case cannot tell the two apart"
        else:
            print("     nothing but CdInit: %d frames of the stream found in the dump at frame %d, all equal to the mixer's sums with the CD volume 0x3fff; the largest sample is %d" % (n, first, loud), flush=True)
    yield "after-nothing-but-cdinit-the-chip-holds-the-librarys-values-and-the-stream-reaches-the-dump-through-them", why
    # a main volume the game set stays, whichever side holds it; with neither it is written (every value by hand)
    trace = work / "x_main.trace"
    trace.unlink(missing_ok=True)
    status, lines, _ = go("a_init_main", [NI, "--no-audio", "--trace", "--trace-file", rig.native(trace)], "x_main")
    yield "cdinit-leaves-a-main-volume-that-the-game-set-on-either-side-and-writes-it-when-both-hold-0-through-the-model", expect(rig, status, lines, 0, [], [
        "left set: main 1234 0000 cd 3fff 3fff control c001", "right set: main 0000 0002 cd 3fff 3fff control c001", "neither set: main 3fff 3fff cd 3fff 3fff control c001", MAIN])
    # the trace counts CdInit's accesses with the game's. By hand: the game reads 15 registers (5 after each CdInit) and writes 6; the first
    # CdInit reads 1 (the left side holds a value) and writes 3, the second reads 2 and writes 3, the third reads 2 and writes 5: 20 and 17
    counts = [l for l in (trace.read_text().splitlines() if trace.exists() else []) if l.startswith("iopage: chip ")]
    yield "the-trace-counts-the-reads-and-writes-of-cdinit-with-the-games", (
        None if status == 0 and len(counts) == 1 and counts[0].startswith("iopage: chip 20 reads 17 writes; ") else f"status {status}; the trace's lines of the chip's counts: {counts!r}")

    status, lines, _ = go("a_xa", [NI, "--no-audio"], "x_nosink")
    yield "with-no-sink-the-audio-sectors-are-consumed-in-silence-and-nothing-is-queued", expect(rig, status, lines, 0, [], ["sectors: played 0 held 0 refused 0 skipped 4 silent 8", "frames: queued in all 0 pulled 0 in the queue 0", MAIN])

    # a dump that fails while a stream plays: the sink goes off, the queue is emptied, and the audio sectors that follow are consumed in silence (no
    # sector is held for ever). 40 sectors of the played channel; the third write of the dump fails (the wrapper of the `wfail` variant). Without the
    # fix the queue fills (18 sectors), the 19th is held at every vblank, and the count of held sectors rises
    f = work / "xw.wav"
    f.unlink(missing_ok=True)
    status, lines, played = go("a_xa_wfail", [NI, "--audio-dump", rig.native(f)], "xw", order=list(range(40)), variant="wfail")
    yield "a-dump-that-fails-while-a-stream-plays-switches-the-sink-off-and-the-rest-of-the-stream-is-consumed-in-silence", expect(
        rig, status, lines, 0, [], [XW_STREAM, WFAIL_LINE, XW_SECTORS, XW_FRAMES, MAIN])

    # the link: exactly one definition of the function, and no other symbol of PsyZ's libcd object in the program
    exe = rig.program_for("audio", program(G["a_xa"]))
    nm = nm_of(rig)
    prog = subprocess.run([nm, "--defined-only", str(exe)], capture_output=True, text=True, timeout=120)
    arch = [x for x in rig.info["link"] if str(x).endswith("libpsyz.a")]
    lib = subprocess.run([nm, "-A", "--defined-only", str(arch[0])], capture_output=True, text=True, timeout=120) if arch else None
    why = None
    if prog.returncode != 0 or lib is None or lib.returncode != 0:
        why = "nm failed"
    else:
        defined = {l.split()[-1] for l in prog.stdout.splitlines() if len(l.split()) == 3}
        mine = [l for l in prog.stdout.splitlines() if l.endswith(" T _Psyz_CdPullSamples") or l.endswith(" T Psyz_CdPullSamples")]
        members = [l.split() for l in lib.stdout.splitlines() if "libcd.c" in l.split(":")[-2 if l.count(":") >= 2 else 0]]
        names = {m[-1] for m in members if len(m) >= 2 and m[-2] in "TDBR"}
        others = sorted(n for n in names if n not in ("_Psyz_CdPullSamples", "Psyz_CdPullSamples") and n in defined)
        if len(mine) != 1:
            why = f"{len(mine)} definitions of Psyz_CdPullSamples in the program"
        elif len(names) < 10 or not any(n.endswith("Psyz_CdPullSamples") for n in names):
            why = f"the control found {len(names)} global symbols in PsyZ's libcd object and did not find Psyz_CdPullSamples among them"
        elif others:
            why = f"symbols of PsyZ's libcd object are in the program: {others[:5]}"
        else:
            print("     one definition of Psyz_CdPullSamples; none of the other %d global symbols of PsyZ's libcd object is in the program" % (len(names) - 1), flush=True)
    yield "the-program-has-one-definition-of-psyz-cdpullsamples-and-none-of-psyzs-libcd-object", why


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--only", default="", help="run only the group of cases with this name (environment, dump, stop, killed, folder, write, device, stream, nopsyz)")
    parser.add_argument("--psyz-build", required=True, type=Path, help="a build folder of psyzbuild.py")
    args = parser.parse_args()
    if not shutil.which(args.cc):
        print(f"the cross compiler {args.cc} is missing; these controls need it")
        return 2
    if args.src:
        L.SRC = Path(args.src).resolve()
    L.BUILD.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="hostaudio-", dir=L.BUILD))
    try:
        try:
            info = json.loads((args.psyz_build / "psyz.json").read_text())
        except (OSError, ValueError) as err:
            print(f"{args.psyz_build / 'psyz.json'} cannot be read ({err}); build PsyZ with psyzbuild.py first")
            return 2
        (work / "p").mkdir()
        (work / "n").mkdir()
        rig = AudioRig(args.cc, args.run.split(), work / "p", info)
        rig_nopsyz = L.Rig(args.cc, args.run.split(), work / "n")
        problem = rig.start_check()
        if problem:
            print(problem)
            return 2
        failed = total = 0
        try:
            for name, detail in cases(rig, args.only, rig_nopsyz):
                total += 1
                if detail is None:
                    print(f"ok   {name}", flush=True)
                else:
                    print(f"FAIL {name}: {detail}", flush=True)
                    print("\n".join(rig.report()))
                    failed += 1
        except Exception as err:  # a control must report, not crash
            print(f"FAIL the control itself raised {type(err).__name__}: {err}")
            print("\n".join(rig.report()))
            failed += 1
        if total == 0:
            print(f"no case matched --only {args.only!r}: the groups are environment, dump, stop, killed, folder, write, device, stream, nopsyz")
            return 2
        print(f"{failed} case(s) behaved wrongly" if failed else f"all {total} cases that ran behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
