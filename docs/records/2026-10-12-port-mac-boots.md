## The macOS build runs the game's start-up from the user's disc (2026-10-12)

The runtime files shared with the Windows program reach the game's memory and
code through `port/src/guest.h` (`port_g`, `port_a`, `port_fn`, `port_call*`):
on Windows each is the identity or a plain call, and `port/tools/objcheck.py`
shows that all 29 files compile to the same bytes as before (recorded from the
tree before the change, compared after). `kernel.c` was converted first as the
pilot; four parallel packages converted the rest (`docs/tasks/port-mac-runtime-conversion.md`);
`jumps.c`, `library.c`, `modules.c`, `threads.c` and `main.c` got `PORT_GUEST_VM`
branches for what is the machine's: 4-byte branch and `hvc` stubs in place of
5-byte jumps, tasks as saved register sets, pages made executable on a fault.
`port/mac/platform.c` is the platform layer (the machine's I/O exit into
`iopage.c`, the timer that interrupts the vCPU for the vblank, the hostcall
dispatcher, crash lines with the guest function's name).

`port/tools/macbuild.py` compiles the runtime with clang for this Mac and links
it with PsyZ and SDL (built by `psyzbuild.py --cc clang`, whose link list for
macOS needed a fix: `-framework NAME` was turned into a file name) and the
guest tables, signs it with the hypervisor entitlement. Run against the
user's disc: the program checks the disc's identity, writes 1,469 branch
stubs and 383 stop stubs, starts the game's `main` in the machine, serves its
library calls, loads its module `slot0f` (228 C jumps), opens a Metal window
and the game uploads textures to video memory (a dump shows its sprite sheets
and palettes). The disc's stand-alone programs (logo, opening movie) are not
built into the guest yet: without `--skip-programs` the run stops by name at
the first; with it the game loops through their calls. No picture of the game
proper, no sound and no input are shown.
