## The port runs natively on Linux (2026-10-23)

The owner asked whether there was a Linux version, then whether the Mac's
64-bit support would help, "otherwise do the 32-bit". It does not help: the
Mac runs the game's C as AArch64 guest code in Apple's Hypervisor framework,
and Linux on x86-64 runs 32-bit programs and lets a process map the PS1's
addresses. So the Linux program is the Windows design. The design and its
state are on [the Linux page](../../port/linux/README.md).

- Runtime: Linux branches (`PORT_LINUX`, defined in `port.h` for a 32-bit
  x86 Linux build) in `memory.c` (one memory file mapped at `0x80000000` and
  again from `0x10000`: the console's copy of RAM without faults), `main.c`
  (one fault handler on an alternate stack that hands faults to the layers,
  and the crash lines), `mirror.c` (faults below `0x10000` and the I/O page),
  `modules.c`, `iopage.c`, `interrupt.c` (the vblank by a real-time signal
  to the game's thread, whose handler aims the context at Windows'
  interruption routine), `threads.c` (`ucontext.h` tasks) and `debug.c`
  (the watchdog). Every runtime file preprocesses for Windows to the same
  text as before (compared with a stand-in `windows.h`).
- Build: `psyzbuild.py --m32`, `hostbuild.py --linux` (an ELF check in place
  of the PE one; every unit with `-m32 -maccumulate-outgoing-args`),
  `./build.sh [-r] linux`. The second flag came from the controls: GCC for
  Linux passes a value read at a low address with `push` of the memory
  operand, which `mirror.c` does not serve; GCC for Windows stores arguments
  instead.
- Controls: the rig of `test_hostlaunch.py` and the files that use it take
  `--linux`. A stale case of `test_hostlaunch.py` (the card routines, which
  became host routines with the memory cards) fails on every system; it is
  left as it is.

What ran, on this machine (Arch Linux, x86-64, GCC 16.2.1 with multilib):
the build of PsyZ and SDL (14 s) and of the program (3,982 units, 14 s,
`linked: port/build/linux/sfa2, verified`); `test_hostbuild.py`,
`test_psyzbuild.py`, `test_build_sh.py` (all cases); `test_hostmirror.py
--linux` 103, `test_hostmodules.py --linux` 55, `test_hostiopage.py --linux
--psyz-build` 112 and `test_hostlaunch.py --linux` 67 of 68, in 24 s
together. With the user's disc: the logo movie (198 frames), the title,
the attract screens and the first module, with sound through PipeWire,
which the owner heard; the arcade intro with a pack imported from the
user's arcade archive (4 s); and a run of 150 s driven by a script of
button presses into a fight against the computer (Ryu against Dhalsim, to
the K.O.), with the character and stage modules placed and the game's
accesses below `0x10000` served by `mirror.c`, ending only at its time
limit.

Found on the way, not changed: the disc's streamed audio runs dry for 20 to
60 ms now and then (the owner heard it in the logo movie). The disc layer
delivers a sector of 53 ms of sound every 50 or 67 ms by the 60 Hz clock
and nothing is held back before the sound starts. Measured with a private
instrumented copy of the runtime: 175 sectors at intervals of 50 to 66 ms,
and 117 spans in 14 s in which the device found the queue empty. A dump file has no
gap: it pulls in steps of the same clock. A start buffer would trade a
little delay of the stream's sound for no gaps on every system; that is the
owner's decision.

Not run: the Windows program (no MinGW compiler here) and the Mac build.
