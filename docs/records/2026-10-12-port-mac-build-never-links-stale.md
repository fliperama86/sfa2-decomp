## The macOS build does not link what did not compile; the Mac files have controls (2026-10-12)

The review of the boot pull request showed that `macbuild.py` could link and sign old
objects after a compile that failed without a diagnostic (a compiler killed by a
signal ends with a bad status and prints nothing): it counted only what the compilers
printed, and then checked that each object file existed.

- Every object is deleted before its compile; each compile's status is kept apart from
  its text; any status other than 0 ends the build before the link, with the file named
  and the status shown; warnings are shown and do not fail the build; the old program
  and the old copy of the guest image are deleted first, and a failed link or signature
  leaves no program.
- `port/tools/test_macbuild.py` (a stand-in for the compiler, the linker and the signer,
  a fraction of a second): the stale object with a silent bad status the review used, a
  failure with text, warnings, a failed link and a failed signature, the files and flags,
  and, on macOS with Apple Silicon only, a syntax check of every Mac file with the
  system's clang (elsewhere it says it was not run and counts the cases that ran).
- `port/tools/test_objcheck.py` (made-up `cc` and `objdump`): record, the same, a
  difference shown, nothing recorded, `--only`, a file not recorded.
- `tools/ai_workflow/port_subjects.toml` has lines for both, and `port/src/guest.h` is
  named by the launch control; no changed file of the stack selects every control file.
