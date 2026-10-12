## macOS port design: a 32-bit-pointer guest in a micro-VM (2026-10-12)

The owner asked for a native macOS build. The Windows program cannot be
carried over: a native Apple Silicon process cannot map memory below 4 GB
(the kernel kills a smaller `__PAGEZERO` at exec; freeing it at run time and
mapping at `0x80000000` fails), and there is no 32-bit user mode.

Measured in scratch only, nothing in the repository ran or changed:

- GCC 16.2.0 for `aarch64-elf` with `-mabi=ilp32` compiled all 3,909 units
  of the build configuration, 12,688 functions with C, 0 failures, and `ld -m
  aarch64elf32` linked them into one 5 MB image with no undefined name beyond
  the host routines the build already binds.
- Apple clang with `-target arm64_32-apple-watchos` also compiled all 3,909;
  250 of 250 structs keep their layout; the objects hold 70 distinct
  instructions, none atomic, floating point or SIMD.
- Under the Hypervisor framework (ad-hoc signature with the hypervisor
  entitlement) an ILP32 function ran at `0x80010000` on an EL1 vCPU; the same
  host memory mapped at `0x0`, `0x80000000` and `0xa0000000`; a read at the
  unmapped `0x1f801070` exited with size, register and direction decoded.

The decision, with its alternatives and the ordered work packages, is
`port/mac/README.md`. No source of the game changes, no original code runs,
and a function without C still stops with its name. The program is not
built; nothing is shown to run.
