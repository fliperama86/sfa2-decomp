# Linux on x86-64

Design and state of the port's native Linux build. Nothing here changes
anything under `ps1/`, and no game source changes for it. Read
[`../README.md`](../README.md) first: this page only adds what is different
on Linux.

**State, 2026-10-23: the game runs natively on Linux from the user's disc: the logo movie, the title and the attract screens, with the
game's sound through PipeWire, a fight, and the opt-in arcade intro. What is shown and what is not is under "What this does not show".**

## Why the Windows design and not the Mac's

The Mac build runs the game's C as a guest in a small virtual machine,
because a Mac program cannot hold memory at the PS1's addresses and has no
32-bit mode (see [`../mac/README.md`](../mac/README.md)). Neither limit holds
on Linux:

- Linux on x86-64 runs 32-bit x86 programs natively (the multilib
  packages of the distribution give the 32-bit C library and the libraries
  that SDL loads).
- A Linux process may map memory at `0x80000000`, at `0x1f800000` and
  from `0x10000` up.

So the Linux program is the Windows program's design: a 32-bit process with
the PS1's memory at its own addresses, the game's C compiled by GCC for
i686 with the same flags, and the 5-byte jumps of `jumps.c`. The Mac's
pieces do not carry over: its machine is Apple's Hypervisor framework and
its guest is AArch64 code. Its 64-bit runtime support (`guest.h`) is not
needed here: as on Windows, a game address is a host address.

The x32 ABI (64-bit mode with 4-byte pointers) was not taken: many kernels
leave it out, and it gives nothing that the 32-bit mode does not.

## The program

An ELF program for i386, not position independent (`-no-pie`), linked
against the system's C library, with PsyZ and SDL as static libraries.
SDL loads the window system's, Vulkan's and the sound server's libraries
at run time, so the machine needs their 32-bit versions (on Arch Linux the
`lib32-` packages of the `multilib` repository; a machine with Steam has
them). Its stack is not executable (`-z noexecstack`): otherwise the
kernel gives a 32-bit program `READ_IMPLIES_EXEC` and the modules layer
could not take execution away from a page.

Every unit is compiled with `-maccumulate-outgoing-args`, which GCC uses by
default for the Windows target: arguments are stored into the stack and not
pushed. Without it GCC for Linux passes a value read from memory with a
`push` of the memory operand, a form that `mirror.c`'s decoder does not
serve; the controls met it (below). With it the game's code makes the
accesses that the decoder was brought up on with the Windows program.

## Memory

| Range | What |
| --- | --- |
| `0x00000000` to `0x0000ffff` | nothing: Linux maps no page below `vm.mmap_min_addr` (65536 by default). Every access faults and `mirror.c` serves it, as on Windows |
| `0x00010000` to `0x001fffff` | the console's copy of RAM: the same pages as the RAM's own from `0x80010000`, mapped a second time (read and write). No fault |
| `0x1f800000` to `0x1f80ffff` | the scratchpad (its first 1 KB) and the I/O page `0x1f801000`, closed by `iopage.c` |
| `0x80000000` to `0x801fffff` | RAM, read, write and execute, loaded from the disc |

The RAM is one memory file (`memfd_create`) mapped shared at both places,
so a write through one view is seen through the other. Every mapping is
made with `MAP_FIXED_NOREPLACE`: a range that is taken already is refused
with a line, never replaced. The start check of `mirror.c` reads
`/proc/self/maps` and refuses to start when a page below `0x10000` is
mapped.

Different from Windows: an access to `0x10000` to `0x1fffff` does not
fault. It is a plain access of the copy, as on the console and on the Mac.
So it is fast, and it prints no first-use line and no trace line. The
image header page of the Windows program does not exist here.

`port_game_span` takes the live stack from the current frame up to the top
of the stack of the task that runs (`threads.c` keeps it).

## Faults

One handler of `SIGSEGV` and `SIGBUS` (`main.c`) hands each fault to the
layers that serve faults, in this order, and the first that serves it
resumes the program:

1. `modules.c`: an execute fault on a page of the RAM that the disc wrote
   (the page fault's error code, bit 4, says it was an instruction fetch).
2. `mirror.c`: a read or a write below `0x10000`, or in the I/O page, by
   the game's thread in the game's code. The decoder (`mirrorcore.c`) and
   the decisions are Windows'.

Anything else prints the crash line of the Windows program and ends with
status 10 (`_exit`, no exit handlers, as `ExitProcess` there). `SIGILL`,
`SIGFPE` and `SIGTRAP` print the exception line. The kind of an access is
taken from the error code: 8 execute (bit 4), 1 write (bit 1), else 0
read: the numbers of a Windows access violation.

The handler is installed with `SA_NODEFER`, so a fault inside a layer's
work comes back to the handler, which ends the program with the second-
fault line, as on Windows. The vblank signal (below) is blocked while the
handler runs.

## The vblank

As on Windows, a timer thread interrupts the game's thread between two
instructions of the game's code, with the same limits (`interrupt.c`).
Windows suspends the thread and rewrites its context. Linux has no
suspension of another thread, so the timer sends a signal to the game's
thread (`pthread_kill`, the first real-time signal) and the signal's
handler, running on that thread, makes the same decision on its own
context: when the instruction pointer is in the game's code or in the
mapped RAM and an interruption is allowed and something is due
(`port_interrupt_take`), it stores the instruction pointer and points the
context at `port_interrupt_entry`. Returning from the handler resumes the
thread there; the routine is Windows' assembly, unchanged.

- The handler only reads flags and the clock and swaps words with an atomic
  operation: no lock, nothing that a signal handler may not do.
- `port_interrupt_aimed` is never true here. The signal is blocked while
  the fault handler runs, and the kernel delivers a fault before a pending
  signal, so a fault's context always holds the faulting instruction.
- The handler restarts system calls (`SA_RESTART`). Sleeps end early on
  the signal; `port_tick` measures time again after a sleep, so an early
  end costs nothing.
- The signal goes to the game's thread only. PsyZ's and SDL's own threads
  never see it.
- The gate of the suspenders and its `--timer-burst` self-checks are kept
  with a mutex, for the same contract. With `--timer-burst` the timer waits
  0.1 ms between rounds, not nothing: a mutex is not fair, and a stop that
  waits for the gate at an exit never got it from a loop that took it again
  at once (the controls hung on it).

## Tasks

The BIOS threads (`threads.c`) are contexts of `ucontext.h`
(`makecontext`, `swapcontext`), each with a stack of 1 MB of its own and an
inaccessible guard page below it, in place of Windows' fibers. A task that
closes itself has its stack freed after the switch away from it.

## The watchdog

`--watchdog N` works as on Windows: a thread that sees no vblank for N
seconds takes the gate and sends the game's thread a second real-time
signal, whose handler copies the instruction pointer and the stack words
for the line. The watchdog prints the line and ends the program with
status 11. If the game's thread does not answer within a second, the line
says that the place is not known.

## Building it

```sh
python3 port/tools/psyzbuild.py --cc gcc --m32 --build port/build/psyz-linux
python3 port/tools/hostbuild.py --linux --psyz port/build/psyz-linux    # port/build/linux/sfa2
port/build/linux/sfa2 [--skip-programs] DISC
```

or `./build.sh [-r] linux`. The compiler is the system's GCC with `-m32`.
Each step takes about 15 seconds on the machine of the first build. SDL is
built with its X11 driver; its Wayland driver needs the 32-bit
`xkbcommon` package as well, and without it the window goes through
XWayland. The arcade intro's pack is made as on the other systems
([`../intro/README.md`](../intro/README.md)), and the program takes
`--arcade-intro --intro-data FILE`.

## Controls

The control files of the runtime run the Linux program with `--linux`
(the compiler then defaults to `gcc`): the rig of `test_hostlaunch.py`
compiles with the target's flags, links without the filler section and
starts the programs directly; the test's own C gets a small `windows.h` of
the rig for the few Windows calls it makes. Cases about the Windows image
(the header page, the filler, the old link) print `not on Linux`, and the
cases of the copy of RAM that are about the served path use addresses
below `0x10000`.

On 2026-10-23, at the commit of this page: `test_hostmirror.py --linux`
103 cases, the native part included; `test_hostmodules.py --linux` 55;
`test_hostiopage.py --linux --psyz-build port/build/psyz-linux` 112;
`test_hostlaunch.py --linux` 67 of 68. The one that fails,
`card-routines-are-listed-as-doing-nothing-on-purpose-with-the-note`, is
stale on every system: the memory cards became host routines after it was
written. The four files took 24 seconds together.

## What this does not show

- That the game plays to its end. Shown on 2026-10-23 with the user's
  disc of `SLPS_004.15`: the start-up, the logo movie through the game's
  own decoder, the title and the attract screens, the first module placed
  on its first call, sound from the sound chip's model and from the disc's
  streamed audio through PipeWire (heard by the owner), and the opt-in
  arcade intro started, and a fight against the computer driven by an
  input script (Ryu against Dhalsim, to the K.O.), with the game's
  accesses below `0x10000` served by `mirror.c`. Pictures were taken of
  the video memory, not of the window.
- Streamed audio has short gaps at times, on Linux as measured, and by its
  design on the other systems too: the disc layer gives the stream a
  sector (53 ms of sound) every 50 or 67 ms, by the 60 Hz clock, and
  nothing is held back before the sound starts, so the queue runs dry for
  20 to 60 ms now and then. A file of `--audio-dump` has none, because it
  takes the sound in steps of the same clock. Not changed here.
- The Windows program was not built or run: no MinGW compiler on the
  machine. Every runtime file preprocesses for Windows to the same text as
  before this change (compared with a stand-in `windows.h`), and the build
  tool's Windows command lines are pinned by `test_hostbuild.py`.
- A controller, the other modes, long runs, the settings of
  `[experimental]` other than the arcade intro, and Wayland without
  XWayland were not tried.
