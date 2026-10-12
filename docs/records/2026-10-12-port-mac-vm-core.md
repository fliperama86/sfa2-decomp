## The macOS port's virtual machine runs made-up guest code (2026-10-12)

`port/mac/vm.c` and `elf.c` are the host side of the machine: one EL1 vCPU of
the Hypervisor framework with identity page tables (4 KB pages, normal
cacheable memory), the PS1's RAM at `0x80000000` with its low and uncached
views as aliases, a scratchpad page, the I/O page unmapped so that every
access exits with its size, register and direction, and a private area for
the guest image and stacks. The host registers hostcalls (`hvc #N`, the stubs
of `guestlink.py`), an I/O handler, a fault hook and an interrupt hook; it
calls into the guest (calls nest), saves and loads whole contexts, and
loads `guest.elf` with each segment's permissions.

`python3 port/tools/test_vm.py` (21 cases, no disc, a few seconds) builds
made-up guest code with the ILP32 compiler and the real linker script and
stubs, signs the host program ad hoc with the hypervisor entitlement and
runs it. All pass. They include the loads and stores with a written-back
base register, which the exit does not describe and which `vm.c` decodes from
the instruction (a first run failed on one: a post-indexed store). The
recursive function fib(30) took 4 ms. Nothing of the game runs on the
machine yet; no runtime exists beyond these files.
