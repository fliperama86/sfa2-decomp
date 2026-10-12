## Fast disc reads, a default-off setting (2026-10-12)

The owner asked to take the loading waits out of the port, first by letting the
disc layer read as fast as the image file can. The waits are the console drive's
rate, which `port/src/cd.c` models by the frame (150 sectors a second at double
speed), not host I/O: the layer reads sectors on demand from the image file and
never sleeps. A load of a few hundred sectors was therefore some seconds of
frames.

New setting `fast_disc` under `[experimental]`, default off (`--fast-disc`,
`--ps1-disc-speed`, `--show-settings`). On, a data read delivers up to 256
sectors a vblank instead of 2 or 3. It ends the row where it always did: read
ended or paused by the handler, a polling reader that has not taken the last
sector, the game's ring guard. The image is not loaded into memory; every sector
is still one `fread` from the file, so the setting is the same code on macOS,
Windows and Linux.

Not made faster, on purpose:
- a read with the mode's XA bit: the game's music, whose reported position must
  follow the sound;
- a read with the runtime's own ready handler: the movies' stream, whose queue
  of sectors halts the program when it is full;
- commands (a Setloc or SeekL still completes at the next vblank).

The game installs only its stream handler (guarded by the ring) and none (it
polls), so no other handler can be lapped. The limit of 256 a vblank is a judgment
call, about 100 times the console's rate.

Evidence: `port/tools/test_hostcd.py` (new `fast_cases`, 13 controls, isolated, about
3 s whole file) and `port/tools/test_settings.py`; two mutants (the XA-bit exception
removed, the own-handler exception removed) each fail exactly their control. Not
run: the real game. How much of a real load this saves, and whether the game's
own per-frame work around a load then dominates, is not measured.
