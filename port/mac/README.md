# macOS on Apple Silicon

Design and state of the port's native macOS build. Nothing here changes
anything under `ps1/`, and no game source changes for it. Read
[`../README.md`](../README.md) first: this page only adds what is different
on this machine.

**State: the guest image builds and verifies (package 1 below). No Mac program exists yet.**

## Why the Windows design does not carry over

The Windows program puts the PS1's memory at the PS1's own addresses in a
32-bit process, so the game's C runs natively and its stored four-byte
addresses mean what they meant. On a Mac with Apple Silicon:

- A native process cannot map memory below 4 GB. The kernel kills an arm64
  program whose `__PAGEZERO` is smaller than 4 GB at exec (tried 3 GB, 2 GB,
  1 GB and 16 KB); freeing it at run time succeeds, and mapping at
  `0x80000000` then fails with "invalid address".
- There is no 32-bit user mode, and no Rosetta mode with four-byte pointers.

Measured on macOS 26.5 with Apple clang 21, on 2026-10-12.

## The design

The game's C is compiled for **AArch64 with 32-bit pointers (ILP32)** and
runs **as guest code in a tiny virtual machine** made with Apple's
Hypervisor framework, at the PS1's own addresses. The runtime (this
folder, `../src`, PsyZ, SDL) is ordinary 64-bit host code and serves what
the game asks for. This is hardware virtualization of the project's own
compiled C: no emulator, no interpreter, and no original code runs. The
owner's rule of 2026-10-09 holds: the guest is the project's C only, and a
function that is not C stops the program with its name.

Why this and not the alternatives:

| Way | Verdict |
| --- | --- |
| Source change: pointers become 32-bit handles read through accessors | Ruled out by the owner's rule of 2026-10-09 (and 75 structs, thousands of sites). |
| Compile-time address translation of every memory access | Possible (the instruction set is small, see below) but a new toolchain stage that must be right for 440 thousand instructions; the stack, the statics and every function address need their own handling; debugging has no hardware help. |
| Wine or CrossOver on the Windows `.exe` | Not native; a stopgap at best; untested. |
| ILP32 guest in a micro-VM (chosen) | The compiler's output runs unchanged. Addresses are the PS1's own. The RAM views alias for free. An access to the I/O page exits to the host with the register, size and direction already decoded. |

What was measured (2026-10-12, scratch only, no repository change):

- GCC 16.2.0 for `aarch64-elf` (the version of the Windows build's
  compiler) with `-mabi=ilp32` compiles all 3,909 units of the build
  configuration with 0 failures, 12,688 functions with C. All objects link
  with `ld -m aarch64elf32` into one 5 MB image (1.4 MB of code) with no
  undefined name except the host routines that the build already binds.
- Apple clang with `-target arm64_32-apple-watchos` also compiles all 3,909
  (250 of 250 structs keep their layout, pointers 4 bytes, `long` 4 bytes).
  The objects hold 70 distinct instructions in 440 thousand; no atomics,
  floating point or SIMD.
- Hypervisor framework: with an ad-hoc signature and the
  `com.apple.security.hypervisor` entitlement, an ILP32 function ran at
  `0x80010000` on an EL1 vCPU; the same host memory mapped at `0x0`,
  `0x80000000` and `0xa0000000`; a read of the unmapped `0x1f801070`
  exited with the access size, the register and the direction.

## The machine

One vCPU, one VM per process. The guest runs at EL1 with the MMU on
(identity page tables made by the host) and normal cacheable memory.

| Range | What |
| --- | --- |
| `0x00000000` to `0x001fffff` | the console's copy of RAM at address 0 (an alias, no fault handler needed) |
| `0x80000000` to `0x801fffff` | RAM, loaded from the disc |
| `0xa0000000` to `0xa01fffff` | RAM, uncached view (an alias) |
| `0x1f800000` | the scratchpad (1 KB), mapped to a private page |
| `0x1f801000` to `0x1f801fff` | the I/O page: not mapped, every access exits to the host (`iopage.c`) |
| `0x80200000` to `0x87ffffff` | the guest's private area: the port's compiled C (impl code), stacks, page tables, vectors. Within the 128 MB reach of a branch from any PS1 address |

- The host sets the system registers itself (page table base, vectors,
  FP enable); there is no boot code.
- Every vector entry is an `hvc` that reports the exception class, so a
  crash prints the guest's PC, its function name, `ESR` and `FAR`.
- Hypercalls are `hvc #N`: `0` returns from a call into the guest, small
  numbers index the port's table of host routines and of functions
  without C. The arguments are in `x0` to `x7`, the result in `x0`; the
  host then continues at the caller's return address, so a stub is the one
  4-byte `hvc`.
- A function with C gets `b impl_NAME` at its PS1 address (the same idea as
  the Windows build's 5-byte jump). A function without C, and every
  library routine the host serves, gets an `hvc`.
- A call from the host into the guest (a callback, a thread's entry, the
  game's `main`) sets the registers, points the return address at an `hvc
  #0` and runs the vCPU until it comes back. Calls nest. The PS1's tasks
  are saved and restored register sets; no fibers.
- The vblank comes from a timer thread that cancels the running vCPU
  (`hv_vcpus_exit`) and runs the interrupt work at that exit, with the
  game's interrupt-disable state respected.

## The guest image

```sh
brew install aarch64-elf-gcc        # GCC 16.2.0 and binutils for aarch64-elf
python3 port/tools/hostbuild.py --guest [--build DIR] [--jobs N] [--list]
```

`hostbuild.py --guest` compiles every unit with `aarch64-elf-gcc -mabi=ilp32`
(the flags of the Windows build, plus the target's), renames each definition
to `impl_NAME` and each reference to `ps1_NAME`, and links the objects with
`aarch64-elf-ld -m aarch64elf32` and `names.ld` into `port/build/guest/guest.elf`:
code at `0x80200000`, read-only data and data on 16 KB boundaries, three
segments. Then it checks the file as it checks the Windows program (every
`ps1_` name at its PS1 address, every implementation outside the PS1's
ranges and present, no plain game name at a PS1 address) and the ELF header
and segments (`guestlink.py`: 32-bit, AArch64, inside the guest's area,
none writable and executable). It writes `gen/guest_tables.c`, the same
tables as the Windows build with each routine's guest address in place of a
symbol. The stand-alone programs of the disc, the sound library with PsyZ
and the runtime are not part of it yet; it takes about a minute on this
machine (4,000 compiles at ten at a time).

On 2026-10-12: 3,909 units, 12,688 functions with C, 17 of them overrides in
C, 383 without C (382 library, 1 game), 45,912 names, linked and verified.
The image holds 1.4 MB of code, 1 KB of read-only data and no data or bss.
`python3 port/tools/test_guestlink.py` (under a second, no compiler) checks
the new pieces on made-up input.

## How the runtime reaches the game's memory

**A pointer that the game gives the host is a guest address, typed as a
pointer, and is read through `port_g()`.** In the Windows program
`port_g(p)` is `p`, so its objects do not change (a check, below). On the
Mac it maps the address to the host buffer. A function pointer that the
game gives the host is a guest address too, and is called through
`port_call`. No thunk is generated: the hypercall passes `x0` to `x7` to the
same C routine that the Windows program calls. Pointers that the host
returns to the game are guest addresses.

PsyZ is compiled for the host (64-bit `u_long`, which it supports). Only
`gpu.c`'s builder of PsyZ's packet chunk, which assumes four-byte
`u_long`, needs care. The sound library and the SPU are game C running in
the guest; the SPU's registers and DMA channel 4 are served from the I/O
exit (`iopage.c`). The surface between the game and PsyZ is about ten
graphics calls, the pad read, MDEC, `VSync` and the present.

## Work packages, in order

Each is a small pull request with isolated checks (see
[`../../docs/efficiency.md`](../../docs/efficiency.md)).

| # | Package | Check that needs no disc and no running game |
| --- | --- | --- |
| 1 | This page and the guest image build (`hostbuild.py --guest`). **Done.** | `test_guestlink.py`; the link verification |
| 2 | VM core in `port/mac/`: memory map, page tables, vectors, `hvc` dispatch, MMIO exits, call into the guest, saved register sets, cancel | a test program that runs made-up guest code |
| 3 | `port_g` / `port_call` in the shared runtime files, no change to the Windows objects | the Windows objects before and after, compared |
| 4 | Mac start: load the disc, write the stubs, call the game's `main`, kernel, C library, threads | stops by name at the first missing function |
| 5 | Video through PsyZ, pads | |
| 6 | The I/O page: SPU, DMA, sound out | |
| 7 | CD, streams, MDEC, the movie programs, overlays | |

## What this does not show

- That the game runs on a Mac. Nothing here is built yet beyond the three
  scratch measurements above.
- Anything about speed: every I/O-page access and every library call is an
  exit from the guest. The Windows program pays a fault per access too.
- Distribution. The program needs the hypervisor entitlement; an ad-hoc
  signature is enough to run a local build. A signed or notarized app is
  not looked at.
- The compiler's differences. GCC compiles the guest as it compiled the
  Windows program (same version, `-O1`), but `-mabi=ilp32` is a deprecated
  GCC option and `ld -m aarch64elf32` a rarely used linker mode.
