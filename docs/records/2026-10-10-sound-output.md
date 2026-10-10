## Sound out: --audio-dump and --audio (2026-10-10)

- What: two options of the program, both off unless asked, both in a build
  with PsyZ. `--audio-dump FILE` pulls 735 stereo frames from PsyZ's model
  of the sound chip at the end of every vblank and appends them to a WAV
  file (16 bits, stereo, 44,100 Hz) without opening a device; the header's
  lengths are rewritten every 60 vblanks and at the exits that run the exit
  routines. `--audio` calls `Psyz_AudioInit()` once in `port_iopage_init`,
  after the model is started and before the game runs; a device that cannot
  be opened is one line and the game runs on. Both together: the file, no
  device, one line. Without PsyZ either is refused at start with one line.
- Changed: `port/src/iopage.c` (the options, the dump, the vblank's pull),
  `main.c` (the options), `port.h` (`port_iopage_init` takes the two),
  `port/tools/test_hostaudio.py` (new), `tools/ai_workflow/review.py` and
  its test (the new file gets the compiler and PsyZ), `port/README.md`.
  `interrupt.c`, `cd.c`, `mirror.c`, PsyZ and everything under `ps1/` are
  unchanged.
- Ends of the dump: a return from main or `exit()` rewrites the header. A
  stop line ends the process with `ExitProcess` (in `mirror.c` and
  `interrupt.c`), which runs no exit routine, so the header is at the last
  multiple of 60 vblanks and the file holds at most 60 vblanks more. The
  controls show a return, a stop line (header 120 vblanks, file 130) and a
  process killed from outside (the bound is checked, not a number).
- The lock: every access of `iopage.c` to the model is between
  `Psyz_AudioLock` and `Psyz_AudioUnlock`, around the one PsyZ call, never
  across a call into the game or a fault. `Psyz_AudioInit` runs on the main
  thread before the game starts, calls `Psyz_SpuInit` again (which returns
  when the model is started) and clears nothing.
- Controls: `port/tools/test_hostaudio.py --cc CC --psyz-build DIR`, 14
  cases, a file of its own so that `test_hostiopage.py` stays readable.
  Every child gets `SDL_AUDIODRIVER` from the control (`dummy`, or a name
  SDL does not have), through `WSLENV` under WSL, and the control refuses
  a start with `--audio` without it. The model being pulled is shown by the
  envelope register of a voice, which only a pulled frame changes; the
  device by `Psyz_AudioIsPaused`. 15 mutants of the runtime (a copy,
  under a time limit), each removing one rule, each fail the case that
  names it; the cases that failed, one run each:

  | rule removed | case that failed |
  | --- | --- |
  | the dump pulls from the model | the dump (samples after the key-on zero) |
  | 735 frames a vblank (700) | the dump (header) |
  | the header says 44,100 Hz (22,050) | the dump (header) |
  | the header is rewritten at the program's end | the dump (header 0) |
  | the header is rewritten every 60 vblanks | the stop line; the process killed from outside |
  | a file that cannot be opened refuses the start | the folder that does not exist |
  | a failed write stops the dump with a line | the write that fails |
  | `Psyz_AudioInit` is called | the dummy device (device paused, envelope 0) |
  | a device that cannot be opened is not a stop | the driver that does not exist |
  | with both options no device is opened | both options |
  | every access is under the audio lock | the lock probe (20 of 20 workers got the lock during the call) |
  | a build without PsyZ refuses either option | both cases without PsyZ |
  | the audio lock is released | the lock case hangs and the rig's time limit ends it |
  | off unless asked (default on) | the run with no option |
- Not here: the audio the disc streams (XA), whether sound is on by default
  (decided no: off unless asked, so that no test makes a sound), and the
  known fault of the lapped ring.
- One-off starts with the disc (a private run, not repeatable from the
  repository): 60 seconds with `--audio-dump`, no `refused` or `stop` line,
  10,423,984 data bytes (3,545.6 vblanks) against 10,584,000 for 60 full
  seconds, every window of ten seconds not silent (largest sample 15360);
  30 seconds with `--audio` and the dummy driver, the open line, no
  `refused` or `stop`; 25 seconds of the default build, no new line.
- The lock case, as repaired after review: inside each locked call a
  worker thread tries the audio lock (one record per call in static
  storage), the wrapper waits 15 ms and notes whether the worker got it
  (it must not); the made-up game joins the workers after its accesses
  have returned, so after every unlock, each within 2 s with exit code 0.
  Creation, wait and close results are checked. The whole control took
  21 s (it took 1 min 57 s before, from a 5 s wait per probed access).
