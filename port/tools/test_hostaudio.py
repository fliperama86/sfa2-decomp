#!/usr/bin/env python3
"""Controls for the port's sound out (iopage.c): --audio-dump FILE and --audio.

    python3 test_hostaudio.py --cc CROSS_CC --psyz-build DIR [--run PREFIX] [--src DIR] [--only GROUP]

--audio-dump FILE appends 735 stereo frames of PsyZ's model of the sound chip to a WAV file at the end of every vblank, with no audio
device. --audio opens the machine's sound output through PsyZ (Psyz_AudioInit, once, at start). Both are off unless asked. With both the
file is written and no device is opened. Without PsyZ in the build either is refused at start.

NO CASE MAY OPEN A REAL DEVICE. Every child that this file starts gets SDL_AUDIODRIVER in its environment, set here: `dummy`, or for the
case of a device that cannot be opened a name that SDL does not have. The route into the child from a Linux shell under WSL is WSLENV,
which is set here too. A start with --audio whose environment lacks the variable is refused by the control itself (a case shows that).

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
- --audio with SDL's dummy device: the line that the output is open, the game runs to its end, the model was pulled (the voice's envelope
  register, which only a pulled frame changes, is not zero) and PsyZ's own state says a device is open (Psyz_AudioIsPaused is 0); the
  same game with no option: envelope 0 and no device; --audio with a driver that does not exist: the line, status 0, no device;
  both options: the file is written, the line, no device (PsyZ says so), the model pulled by the dump;
- the lock: with --audio the mutex exists, and every access of iopage.c to the model is probed (the link wraps PsyZ's routines; a second thread
  tries the audio lock while the call runs: it must wait);
- a build without PsyZ: each option refused with its line, status 2, no file.

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
import test_hostiopage as I  # noqa: E402
import test_hostlaunch as L  # noqa: E402
from test_hostlaunch import ABS_BASE, FUN_BASE, RAM, Variant, program  # noqa: E402

FRAME_BYTES = 735 * 4
BAD_DRIVER = "nosuchdriver"
SAMPLE = "dummy"

HOSTS = dict(I.PSYZ_HOSTS)
HOSTS.update({"audio_paused": "host_audio_paused", "lock_calls": "host_lock_calls", "lock_viol": "host_lock_viol"})
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
    SAY("lock probes: %d calls, %d with the lock not held\n", ps1_lock_calls(), ps1_lock_viol());""")
game("a_wfail", "int i;\n" + VOICE0 + r"""
    for (i = 0; i < 6; i++) ps1_VSync(0);
    SAY("the game goes on at vblank %d\n", ps1_VSync(-1));""")

FUN = FUN_BASE + [(n, RAM + 0x101100 + 0x10 * i, n) for i, (n, _) in enumerate(GAMES)]
G = {n: a for n, a, _ in FUN}
GAME_C = I.HEADER + I.PSYZ_HEADER + "\nstatic void unused_names(void) { (void)wait_ev; }\n" + "".join("void %s(void)\n{\n%s\n}\n" % (n, b) for n, b in GAMES)
GAME_C = GAME_C.replace("extern int ps1_EnterCriticalSection(void)", "extern int ps1_audio_paused(void), ps1_lock_calls(void), ps1_lock_viol(void);\nextern int ps1_EnterCriticalSection(void)", 1)

WRAPPED = ["Psyz_SpuRead", "Psyz_SpuWrite", "Psyz_SpuFifoWriteBulk", "Psyz_SpuMemRead", "Psyz_SpuSetTransferAddr", "Psyz_SpuGetTransferAddr"]


def domains(extra: str = "") -> str:
    decls = "".join(f"extern void {h}();\n" for h in HOSTS.values() if h.startswith("port_h_") and h != "port_h_DMACallback")
    rows = "".join(f'{{ "{n}", (void *){h}, 0 }}, ' for n, h in HOSTS.items())
    return f"""
#include "port.h"
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
static volatile int calls, viol;
int host_lock_calls(void) {{ return calls; }}
int host_lock_viol(void) {{ return viol; }}
unsigned host_io_read(void) {{ return *(volatile unsigned *)0x1f801014; }}
{extra}
static const struct port_library t[] = {{ {rows} {{ 0, 0, 0 }} }};
const struct port_domain port_domains[] = {{ {{ "kernel", t }} }};
const unsigned port_domain_count = 1;
static const struct port_override o[] = {{ {{ 0, 0, 0 }} }};
const struct port_override *const port_override_sets[] = {{ o }};
const unsigned port_override_set_count = 1;
"""


# The lock probe: each routine of PsyZ that iopage.c calls on the model is wrapped by the link (--wrap). While the real routine runs, a second
# thread tries the audio lock; if the caller holds it, the thread must still be waiting after 15 ms.
PROBE = r"""
static DWORD WINAPI probe_thread(LPVOID p) { Psyz_AudioLock(); *(volatile int *)p = 1; Psyz_AudioUnlock(); return 0; }
static void probe(void)
{
    volatile int got = 0;
    HANDLE h = CreateThread(NULL, 0, probe_thread, (void *)&got, 0, NULL);
    Sleep(15);
    calls++;
    if (got) viol++;
    WaitForSingleObject(h, 5000);
    CloseHandle(h);
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
VARIANT_C = {"audio": domains(), "lock": domains(PROBE), "wfail": domains(WFAIL)}
VARIANT_FLAGS = {"lock": [f"-Wl,--wrap={n}" for n in WRAPPED], "wfail": ["-Wl,--wrap=fwrite"]}


def wsl_env(driver: str) -> dict:
    env = dict(os.environ)
    env["SDL_AUDIODRIVER"] = driver
    if shutil.which("wslpath"):
        parts = [p for p in env.get("WSLENV", "").split(":") if p and p.split("/")[0] != "SDL_AUDIODRIVER"]
        env["WSLENV"] = ":".join(parts + ["SDL_AUDIODRIVER"])
    return env


class AudioRig(I.PsyzRig):
    """The PsyZ rig whose children carry SDL_AUDIODRIVER (set here) and whose starts are guarded: a start with --audio whose environment
    lacks a driver that cannot reach a real device is refused."""

    def __init__(self, cc, prefix, work, info):
        super().__init__(cc, prefix, work, info)
        self.env = wsl_env(SAMPLE)

    def program_for(self, variant, pin, old_link=False):
        saved = self.libs
        self.libs = saved + VARIANT_FLAGS.get(variant, [])
        try:
            return super().program_for(variant, pin, old_link)
        finally:
            self.libs = saved

    def guard(self, args):
        if "--audio" in args and self.env.get("SDL_AUDIODRIVER") not in (SAMPLE, BAD_DRIVER):
            raise AssertionError("refused to start with --audio: SDL_AUDIODRIVER is %r, not %r or %r" % (self.env.get("SDL_AUDIODRIVER"), SAMPLE, BAD_DRIVER))

    def start(self, exe, args, timeout):
        self.guard(args)
        self.last = None
        try:
            proc = subprocess.run([*self.prefix, str(exe), *args], capture_output=True, text=True, timeout=timeout, env=self.env)
        except subprocess.TimeoutExpired as err:
            self.record(None, err.stdout, err.stderr, timed_out=True)
            raise
        self.record(proc.returncode, proc.stdout, proc.stderr)
        return proc.returncode, proc.stdout


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
        try:
            rig.guard(["--audio"])
            raised = False
        except AssertionError:
            raised = True
        finally:
            rig.env = saved
        print("     the child's environment: SDL_AUDIODRIVER=%s WSLENV=%s" % (rig.env.get("SDL_AUDIODRIVER"), rig.env.get("WSLENV")), flush=True)
        yield "the-environment-of-every-child-has-the-dummy-driver-and-the-route-through-wslenv", None if (ok and bad and rig.env["SDL_AUDIODRIVER"] == SAMPLE) else f"environment {e.get('SDL_AUDIODRIVER')!r} {e.get('WSLENV')!r}"
        yield "the-control-refuses-to-start-with-audio-when-the-driver-is-not-set", None if raised else "the guard let a start with --audio and no driver through"

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
        status, lines = run(rig, "a_device", [NI], tag="nodev")
        yield "with-no-option-no-device-is-open-and-the-model-is-not-pulled", expect(rig, status, lines, 0, [], ["device paused 1", "envelope not zero 0", "lock probes: 0 calls, 0 with the lock not held", MAIN])
        status, lines = run(rig, "a_device", [NI, "--audio"], tag="dummy")
        yield "audio-with-the-dummy-device-opens-it-and-the-model-is-pulled", expect(rig, status, lines, 0, [OPEN], ["device paused 0", "envelope not zero 1", "lock probes: 0 calls, 0 with the lock not held", MAIN])
        status, lines = run(rig, "a_device", [NI, "--audio"], env=wsl_env(BAD_DRIVER), tag="baddev")
        yield "audio-with-a-driver-that-does-not-exist-is-a-line-and-the-game-runs-without-sound", expect(rig, status, lines, 0, [FAILED_OPEN], ["device paused 1", "envelope not zero 0", "lock probes: 0 calls, 0 with the lock not held", MAIN])
        f = work / "b.wav"
        f.unlink(missing_ok=True)
        status, lines = run(rig, "a_device", [NI, "--audio", "--audio-dump", native(f)], tag="both")
        why = expect(rig, status, lines, 0, [BOTH], ["device paused 1", "envelope not zero 1", "lock probes: 0 calls, 0 with the lock not held", MAIN])
        if why is None:
            b = f.read_bytes()
            dlen = len(b) - 44
            why = header_problem(b, dlen) or (None if dlen > 0 and dlen % FRAME_BYTES == 0 else f"data length {dlen} is not a whole number of vblanks")
        yield "both-options-write-the-file-and-open-no-device-and-say-so", why
        # every access to the model is made with the lock held: 19 writes of the game and 1 read (counted by hand)
        status, lines = run(rig, "a_device", [NI, "--audio"], tag="lock", variant="lock")
        yield "with-the-device-open-every-access-of-iopage-to-the-model-is-made-with-the-audio-lock-held", expect(rig, status, lines, 0, [OPEN], ["device paused 0", "envelope not zero 1", "lock probes: 20 calls, 0 with the lock not held", MAIN])

    # ---- no PsyZ ----
    if want("nopsyz"):
        f = work / "n.wav"
        f.unlink(missing_ok=True)
        for opt, args in [("--audio-dump", ["--audio-dump", native(f)]), ("--audio", ["--audio"])]:
            status, lines, _, _ = rig_nopsyz.run("nopsyz", program(L.ENTRY_C), args=args, timeout=60)
            line = f"refused: {opt} needs a build with PsyZ (sound out is PsyZ's model of the chip)"
            yield ("a-build-without-psyz-refuses-%s" % opt.lstrip("-")), (f"status {status}, wanted 2; lines {lines!r}" if status != 2 else (None if lines == [line] and not f.exists() else f"lines {lines!r}, wanted {[line]!r}; file exists {f.exists()}"))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cc", required=True)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    parser.add_argument("--src", default=None, help="another copy of the runtime's folder")
    parser.add_argument("--only", default="", help="run only the group of cases with this name (environment, dump, stop, killed, folder, write, device, nopsyz)")
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
            print(f"no case matched --only {args.only!r}: the groups are environment, dump, stop, killed, folder, write, device, nopsyz")
            return 2
        print(f"{failed} case(s) behaved wrongly" if failed else f"all {total} cases that ran behaved as required")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
