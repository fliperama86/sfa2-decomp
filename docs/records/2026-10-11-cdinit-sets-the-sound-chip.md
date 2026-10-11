## CdInit sets the sound chip, so the logo's movie is heard (2026-10-11)

- The fault: the logo's movie, which plays in the PC program since main
  `c210be1`, was silent. The disc layer found and decoded its audio, but
  the sound chip's model mixes disc audio only with the control
  register's CD bit set and through its CD volume and main volume, and
  all three were 0 until the game started its own sound. The record of
  the logo's listing inferred that on the console the boot code leaves
  them set, and said it had no source.
- The cause, read in the game's code: it is the library's start of the
  drive. `CdInit` (`0x8015c6ec`) calls `CdReset(1)`, which after the
  drive's reset calls the routine at `0x8015e10c` (the SDK's
  `CD_initvol`). Its instructions, through the pointer at `0x80181e0c`
  (the program file holds `0x1f801c00` there): if the current main
  volume reads 0 on the left (`+0x1b8`) and then on the right
  (`+0x1ba`), the main volume is set to `0x3fff` on both sides
  (`+0x180`, `+0x182`); the CD volume is set to `0x3fff` on both sides
  (`+0x1b0`, `+0x1b2`); the control is set to `0xc001` (`+0x1aa`); the
  drive's mix is set to `0x80, 0, 0x80, 0`. PsyZ's own library has the
  routine with the same values; the port does not link it. The port's
  `CdInit` is a host routine and did none of this.
- The change: `port_CdInit` in `port/src/cd.c` does it (`initvol`),
  through two new routines of `port/src/iopage.c`
  (`port_iopage_chip_read`, `port_iopage_chip_write`: one register of
  the chip's model under the audio lock, counted with the game's reads
  and writes of the chip in the trace; in a build without PsyZ they read
  and write nothing and say so by their return). The mix goes through
  `port_CdMix`.
- One difference from the console, stated in the source and on the
  page: PsyZ's model keeps no current main volume (its two registers
  read what was last written to them), so the port asks the main volume
  itself and takes "holds 0" for "the current volume is 0". For a set,
  unswept main volume that is the same question (inferred, not measured
  on a console); a main volume in a sweep mode counts as not 0.
- Controls. `python3 port/tools/test_hostcd.py`: 236 cases, all as
  required; twelve are new (the rig's made-up file of the chip's
  registers and its record of the calls: all registers 0; the left side
  set; only the right side set; a sweep word; values in the two current
  volume registers; the CD volume and the control overwritten; a second
  and third `CdInit`; no model of the chip; the mix after another
  `CdMix`, with and without a model, and the same stream without the
  second `CdInit`; the lock). `test_hostaudio.py --cc CC --psyz-build
  DIR`: 32 cases, all as required; four are new, through the whole
  program with PsyZ (after nothing but `CdInit` the game reads `main
  3fff 3fff cd 3fff 3fff control c001` and a made-up stream of 18,816
  frames is in the dump, every frame equal to the mixer's sum with the
  CD volume `0x3fff`; a main volume that the game set on either side
  stays and both sides at 0 are written; the trace of that run counts
  `CdInit`'s reads and writes with the game's, 20 and 17 by hand;
  `CdInit`'s two reads and five writes of the model are probed for the
  lock, 27 calls with the game's 20). Its older stream games now call `CdInit` before they set the
  chip, because `CdInit` writes the CD volume and the control. Also run,
  all as required: `test_hoststream.py --cc CC` (68), `test_hostprogcalls.py
  --cc CC` (48), `test_hostiopage.py --cc CC --psyz-build DIR` (112),
  `test_hostlaunch.py --cc CC`. The rigs of the first two got the two
  routines as stand-ins that say "no model".
- Mutants, one run each on a copy of the runtime (`--src`), under a time
  limit. Fourteen of `initvol` against `test_hostcd.py` (not called; the
  main volume always written; only the left side asked; only the right
  side asked; the CD volume not written, left and right; the control
  without the CD bit; the control not written; the mix not set; writes
  without a model; another value for the main volume; the current volume
  registers asked; the control before the CD volume; another mix) and
  six of the two routines of `iopage.c` against the groups `device` and
  `stream` of `test_hostaudio.py` (the read without the lock; the write
  without the lock; the write writing nothing; the read saying "no
  model"; the read giving another register; the write dropping the top
  bit). By kind, in their last runs: all twenty failed at least one case
  by name, none ran into the time limit, none ended the control, none
  survived. Two things on the way there. Two of the fourteen at first
  did not compile (an unused function and an unused variable under
  `-Werror`), so the control ended with no case named; they were
  rewritten to compile and then failed cases by name. And one of the six
  survived its first run, the read giving another register: no case of
  the whole program had a main volume set before `CdInit`. The case of
  the main volume on either side was added for it, and the mutant then
  failed that case.
- The real program, one-off (the user's disc, without a window, SDL's
  dummy audio driver, `--skip-programs --audio-dump FILE`, 40 seconds
  each, status 124, no line beginning `refused` or `stop`; no command of
  this repository repeats them). Built from main `c210be1`: the first
  sample that was not 0 came at 18.33 seconds of the file. Built from
  this change on that commit (`sound library: 45 units of sdk/libsnd, 22
  of sdk/libspu, 6 of library_nonmatching`, `units: 3980 compiled, 161 of
  them nonmatching, 0 failed`, `programs built in: 1`, link verified): at
  0.77 seconds; the largest absolute sample of
  each of the first eleven seconds was 6625, 10059, 8198, 9909, 12978,
  15429, 13929, 8803, 2451, 249 and 8, the next seven seconds were 0,
  and seconds 18 and 19 held 14675 and 15330 in both files.
- Not done: nobody listened to it, and nothing was compared with a
  console. The current main volume of a swept main volume is not
  followed. Nothing under `ps1/` changed, and PsyZ did not.
