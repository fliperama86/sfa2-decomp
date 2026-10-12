## The macOS port's guest image builds and verifies (2026-10-12)

`hostbuild.py --guest` compiles the build's 3,909 units with
`aarch64-elf-gcc -mabi=ilp32` (GCC 16.2.0, the version of the Windows
build's compiler), renames definitions and references as the Windows build
does, links them with `aarch64-elf-ld -m aarch64elf32` and the same
`names.ld` into one ELF image at `0x80200000` (1.4 MB of code, three
segments, none writable and executable), verifies every name at its PS1
address and writes the runtime's tables with guest addresses. 12,688
functions with C (17 overrides), 383 without, 45,912 names, 0 failed units,
about 50 seconds on this machine. Nothing is run: no program of the Mac build
exists yet.

The Windows build is unchanged: the new flags are empty without `--guest`
and the tool's own controls (`test_hostbuild.py`, 85 seconds) pass.
`test_guestlink.py` (nine cases, no compiler, under a second) covers the
instruction words, the stubs, the linker script, the ELF checks, the `nm`
reading and the tables. Not done: the disc's stand-alone programs, the sound
library, and any runtime. The design is `port/mac/README.md`.
