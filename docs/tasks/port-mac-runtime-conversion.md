# Work package: the shared runtime files reach the game through guest.h

## Objective
Each file of `port/src/` that the macOS build shares with the Windows build compiles for the Mac (`-DPORT_GUEST_VM`) and reaches the game's
memory and code only through `port/src/guest.h`, while the Windows build's compiled code does not change at all. The design is
`port/mac/README.md` ("How the runtime reaches the game's memory"); the pattern is the pilot, `git show ed84c210 -- port/src/kernel.c`.

## Pinned contract
Read, in this order: `port/src/guest.h` (the rules are its header), the pilot diff above, the file(s) you own. The rules:

1. A pointer that a host routine receives from the game, or that a game structure holds, is a guest address in a pointer's type. Dereference
   it only through `port_g(p)` or `port_gs(p, size)` (size in bytes of the whole span that is touched); use the result for the access and do
   not keep it past the routine. A pointer that is only stored, passed on, compared or printed may stay as it is. Where the code checks a
   span with `port_game_span`, keep the check (the Mac side answers it).
2. A pointer into the game's memory that the runtime holds (from `port_g`, or a buffer it hands the game) becomes a guest address again with
   `port_a(p)` before it is stored in the game's memory, printed as an address or returned to the game. Casts such as
   `(uint32_t)(uintptr_t)p` of a host pointer are the places to look for; of a guest-address pointer they are fine.
3. Anything that overlays game memory or is one word of the game's has a fixed width: `uint32_t`, `uint16_t`, `int16_t`... never
   `unsigned long`, `long`, `size_t` or a pointer inside such a struct (on the Mac they are 8 bytes).
4. A function of the game (a callback, a handler, a thread's entry) is a `port_fn`. Test it against null with `PORT_FN_NULL` or plain `if
   (f)`; give it to `port_target_check` as `PORT_FN_ADDR(f)`; return it to the game with `PORT_FN_PTR(f)`; call it with `port_call0..4` (no
   result) or `port_callr0..4` (an unsigned result). Arguments of these calls are words: integers, or guest addresses.
5. Memory of the runtime that the game is given the address of (a result buffer passed to a handler, a cell whose address is returned) is, on
   the Mac, `port_cell(key, size)` (guest memory); keep the static object for Windows under `#ifdef PORT_GUEST_VM` / `#else`.
6. Code that uses the Windows API, x86 machine code, fibers or fault handlers stays inside `#ifdef _WIN32` as it is. Do not write
   replacements: list in your report each function of the file that the Mac build must therefore provide in another way (name, one line why).
7. Behavior and text change nowhere. No refactor, no rename, no new comment style. No em dash anywhere.
8. Do not edit `guest.h`, `port.h`, `port_tables.h`, `hostbuild.py`, the files of `port/mac/` or any file that is not in your list.
   Do not use git (no add, no commit, no stash): the working tree is shared and the owner of the package commits.

## Scope
The files named in the brief. Everything else is untouched.

## Acceptance commands
Run both for each of your files; the first is the Windows evidence, the second the Mac one.

```sh
python3 port/tools/objcheck.py check --only FILE.c ...        # about 2 s; must print `all N files are the same as recorded`
clang -target arm64-apple-macos -O1 -Wall -Wextra -DPORT_GUEST_VM -I port/src -c port/src/FILE.c -o /dev/null
# files that use PsyZ (gpu.c, input.c, iopage.c, press.c) also take: -DPORT_HAVE_PSYZ -D__psyz -isystem port/build/psyz-mac/src/psyz/include
```
The Mac compile must give no warning that the file did not give for Windows. If `objcheck` shows a difference, the change is wrong: find the
construct (the tool prints the first lines of the diff) and change it so that Windows compiles to the same bytes (usually a macro
that expands to the old text on Windows). No other run is needed; do not build the game and do not run it.

## Evidence
Report, under 600 words: per file, what changed in one line each; the two results above; the list from rule 6; every judgment call.

## Decisions and escalation
STOP and report the options instead of choosing when you meet a fork with design weight: a place where the code depends on a host
pointer being the game's address in a way the rules do not cover, a layout that would change, a need to change `guest.h`, `port.h` or another
file's interface, or a behavior that cannot be kept the same on both builds. Minor judgment calls may proceed but must be flagged in the report.

## Completion
Both results shown in the report; no file outside the list changed (`git status` shows it).
