## One build command for the port: build.sh (2026-10-13)

The owner asked how to build, and for a script like `./build.sh win`, with an
option `-r` that runs the game, and that does not build again what is built.

- `build.sh` at the root: `./build.sh [-r] win|mac [PROGRAM OPTION ...]`. It
  runs `psyzbuild.py`, `hostbuild.py` and, for the Mac, `macbuild.py` in the
  documented order with their default folders, and fetches the PsyZ submodule
  when its folder is empty. Its head is its contract.
- Skipping is by step. The library step is skipped for the same PsyZ commit
  with a clean tree, the same patch, tool and compiler version line. The
  program step is skipped when, besides that, `git status` shows nothing under
  `ps1/` and `port/` and the commit's trees of the two folders and the script
  are those of the last run that ended well. Each key is a file beside the
  step's folder. A failed step leaves no key.
- Not done: skipping inside the program step. When a file changed,
  `hostbuild.py` compiles every unit again; a memory for single units would
  have to be built into that tool.
- `-r` starts the program with `--skip-programs`, the settings file
  (`port/settings.ini` unless `SFA2_SETTINGS` or the caller's `--settings`),
  the caller's options and the disc of `SFA2_DISC`. One machine's paths go in
  `local/build.env`, which is not in Git.
- `port/tools/test_build_sh.py`: 18 cases with stand-ins for the tools, the
  compilers, `wslpath` and the submodule fetch. `tools/ai_workflow/port_subjects.toml`
  has its line.

What ran, on main `3522d905` plus this change, on Linux under WSL:
`test_build_sh.py` (`all 18 cases behaved as required`); `./build.sh win`
from an empty build folder (submodule fetched, library built, program
`linked: port/build/host/sfa2.exe, verified`, `3982 compiled`, `0 failed`);
again with a changed tree (library skipped, program built); again with
nothing changed (both skipped); `./build.sh -r win --no-audio` hidden, which
started the program with the disc (`identity: SHA-256 matches the build's
baseline`) until a time limit ended it. `test_workflow.py`: six failures, the
same six as on the unchanged commit.

Not run: the `mac` path on a Mac (stand-ins only).
