## Sound is part of the port's normal build (2026-10-10)

- The owner's decision. Told that the game's sound only played when the
  program was built and started with extra options (which the private
  script he starts the game with hid from him), and asked whether to
  make it part of the normal published build, he answered "go". Until
  then `port/README.md` said "Whether sound is on by default is not
  decided; here it is off unless asked, so that no test and no script
  makes a sound."
- The build: `hostbuild.py --psyz DIR` compiles the game's own sound
  library without being asked. `--no-sound-library` leaves it out (the
  build that `--psyz DIR` alone used to be). `--sound-library` is still
  taken: with `--psyz` it changes nothing, without it it is refused as
  before, and together with `--no-sound-library` it is refused. Without
  `--psyz` nothing changed: no library, and `--no-sound-library` does
  nothing. A configuration with no unit of the library is refused when
  the library would be compiled, also when nothing was said, and the
  line names `--no-sound-library`.
- The program: a build with PsyZ opens the machine's sound output at
  start without being asked. `--no-audio` opens none. `--audio` asks by
  name: nothing changes with PsyZ, and without PsyZ it is refused as
  before. `--audio` with `--no-audio` is the usage line. `--audio-dump
  FILE` writes the file and opens no device, as before; the line about
  both is printed only when `--audio` was given. A build without PsyZ
  with no sound option runs in silence and prints nothing, as before.
  `port_iopage_init` takes one of three values for the device
  (`PORT_AUDIO_DEFAULT`, `PORT_AUDIO_OFF`, `PORT_AUDIO_ASKED` in
  `port.h`).
- What this asks of everyone who starts the program in a test or a
  script: `--no-audio` or `--audio-dump`, or SDL's dummy audio driver in
  the child's environment. A start of a build with PsyZ with none of
  them now plays on the machine.
- The controls. Two control files link the port's program with PsyZ,
  through one rig (`PsyzRig` in `test_hostiopage.py`). The rig now gives
  every child SDL's dummy driver (`SDL_AUDIODRIVER`, under WSL through
  `WSLENV`), refuses a start whose environment names no driver or
  another one, whatever the arguments (the guard that `test_hostaudio.py`
  had for `--audio`, moved into the rig), refuses the launch rig's
  bounded run, which takes no environment, and in `test_hostiopage.py`
  puts `--no-audio` before the arguments. Two new cases there: the
  program then prints no line about sound (it failed when the rig was
  changed to put nothing there: one run, the change taken out again),
  and the three refusals. `test_hostaudio.py` names its own sound
  options per case. It has 28 cases; the new ones: the
  device with no option (opened, the model pulled), `--no-audio` (no
  line, not pulled), a driver that does not exist with no option, both
  orders of `--audio` with `--no-audio`, the dump alone and with
  `--no-audio`, the guard with no option, and a build without PsyZ with
  no option and with `--no-audio` (it runs, prints nothing about sound,
  and both print the same). `test_hostbuild.py`: the made-up game of the
  PsyZ cases has no sound library, so they name `--no-sound-library`;
  the sound cases show `--psyz` alone compiling the library, the option
  by name giving the same run, the library left out, both options at
  once refused, no library unit refused with the way out named, and no
  change without `--psyz`.
- What ran, on this change over main `f201b77`:
  `python3 port/tools/test_hostbuild.py` (406 cases, all as required);
  `test_hostaudio.py --cc CC --psyz-build DIR` (28); `test_hostiopage.py
  --cc CC --psyz-build DIR` (112). These three are of the change's last
  commit that touches a file under `port/`. `python
  tools/ai_workflow/workflow.py review` ran once, on the change's first
  commit: 24 checks, all passed (every control file of the port, and the
  build without `--psyz`), in ten and a half minutes. What came after
  that commit is the guard's move into the rig, in the two control files
  named above, and text; the review was not run a second time for it. PsyZ was the pin `6d01339a`, the
  compiler MinGW-w64 GCC 16.2.0 for 32-bit Windows. The real build with
  `--psyz DIR` and nothing else printed `sound library: 45 units of
  sdk/libsnd, 22 of sdk/libspu, 6 of library_nonmatching` and `units:
  3980 compiled, 161 of them nonmatching, 0 failed`, and linked verified.
  Three one-off starts of it with the user's disc, offscreen, one at a
  time, none audible: `port/README.md` has their lines ("One-off starts
  of the build with nothing said about sound").
- After the owner's review: main `85ea4a2` (the build that takes the C
  of a listed stand-alone program) was merged in. One conflict, the
  usage line of `hostbuild.py`, which now names `--programs` and
  `--no-sound-library`; the rest merged by itself. The page's opening
  paragraph still said sound came "only when asked" and was corrected.
  On the merge commit: `test_hostbuild.py` 519 cases, all as required
  (main's program cases and this change's); `test_hostaudio.py` 28 and
  `test_hostiopage.py` 112, as before; the real build with `--psyz DIR`
  and nothing else printed the same line of the sound library and the
  same counts, `programs built in: 0`, and linked verified; one start of
  it (the dummy driver, offscreen, 20 seconds, `--skip-programs`)
  printed `audio: sound output open, 44100 Hz stereo` and no line that
  began `refused` or `stop`. The review command was not run again.
- Mutants, one run each, under a time limit, every one taken out again.
  Seven of the program, on a copy of the runtime (`--src`), against the
  group of `test_hostaudio.py` that covers the rule: the device not
  opened unless asked; `--no-audio` doing nothing; a build without PsyZ
  refusing a start with no sound option; the line about both options
  printed when `--audio` was not given; `--audio` taken after
  `--no-audio`; `--no-audio` taken after `--audio`; `--no-audio` with
  the dump writing no file. Five of the build tool against
  `test_hostbuild.py`: the library only when asked; `--no-sound-library`
  ignored; the library compiled without `--psyz` too; both options at
  once taken; the refusal not naming `--no-sound-library`. By kind: all
  twelve failed at least one case by name, none ran into the time limit,
  none ended the control, none survived. For two of the build tool's
  (the second and the third) a group of the control also raised an
  exception beside the cases that failed by name.
- Not done: nobody listened to those three starts. The other control
  files that start the port's program do not link PsyZ's sound (their
  `iopage.c` is compiled without it), so nothing changes for them; that
  is read from their rigs, and they were not run for this change.
  Nothing under `ps1/` changed, and PsyZ did not.
