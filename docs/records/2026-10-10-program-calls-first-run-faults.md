## Three faults of the program calls that the logo's first run found (2026-10-10)

What happened: the logo's program was started for the first time, in
a private trial build that is not in the tree (main with the open
changes for the build and for `Exec` merged in, and the logo listed).
`Exec` found the program, its three checks passed (the file's hash,
the header and the body in RAM), and the program's C ran. It then
failed at three places, each in a routine of the runtime that is on
main with a passing test. Each is corrected here with the case that
shows it. No program is listed by this change.

The three:
- `CdReadSync(0)` gave up at once. Its wait counted 600 calls of the
  runtime's tick. The tick sleeps only while the next vblank is more
  than a millisecond away and returns at once otherwise; a temporary
  counter in the trial showed 145,388 calls of the tick in 155
  vblanks. So 600 calls passed in under a millisecond, and the read
  of the stills' file, 74 sectors, was given up and started again
  7,633 times in a run of 60 seconds. The wait now counts vblanks:
  it goes on while the read is on and fewer than 600 vblanks have
  passed since the last sector, or since the call. The test's
  stand-in for the tick already had a divisor (the frame moves at
  every Kth call); its cases used it at 400, below the limit of 600
  calls, and at 0 (no vblank at all). The cases now use 7 and a
  number far above the limit, and a new command lets the disc
  deliver nothing while vblanks pass.
- `StGetNext` stopped the program: its two pointers were checked to
  lie in the game's RAM. They are the caller's own variables, and a
  program's C runs natively with its locals on the host's stack. The
  check is gone; a null pointer stops with a line.
- `StGetNext` never let the clock go. The logo's frame function
  polls it a fixed number of times (0x100000) before it gives up,
  which takes seconds on the console and milliseconds on a PC: the
  program ended before its first frame had come and returned to the
  game. A poll that has no frame to give now calls the tick once, as
  the runtime's other polls do; a poll that gives a frame does not.

What the trial showed after the three were corrected in it (once; a
private build, nothing started with a window): the program ran to its
end and returned to the game, which went on into its own start. Video
memory dumped once a second held movie frames for about 9 seconds;
two of them, rebuilt as 24-bit pictures, showed the logo's animation.
Nothing was compared with a console, the window's own picture was not
seen (the run had no screen), and the sound was off.

What ran here: `python port/tools/test_hostprogcalls.py --cc CC`,
`all 48 cases behaved as required` (45 before); `python
port/tools/test_hoststream.py --cc CC`, `all 68 cases behaved as
required` (65 before). Against main's code: the two new cases of the
wait with a divisor above 1 fail (at a divisor of 7 the wait ended
after 86 vblanks; with a far divisor the read timed out), the two
about a late sector pass (with one vblank a call, calls and vblanks
are the same count), and the four cases of `StGetNext` fail at the
old check, so the tick alone is shown by the mutants. Mutants of the
changed lines in bounded runs by a private script, on the final test
files, by kind: 28; 27 failed a named case; 1 hit the time limit (the
wait with its tick taken out, which never moves the clock); none
passed; none ended a tester without a name. The PC program built
with the picture library and without it (`linked: ..., verified`).

Why the tests had passed: in the first test no case put more calls
of the tick between two sectors than the limit of 600, so a count of
calls never ran out during a read that was going on; the second test handed
`StGetNext` pointers into the game's memory "as on the console",
where a program's locals are; and nothing polled in a loop.

Limits: with a clock that never moves the wait of `CdReadSync` does
not end (the runtime's clock is the machine's). The tick's own
behaviour in the last millisecond before a vblank is not changed
here. The disc layer's own waits use the same number as a count of
calls; they end through their pending answers and were not looked at
further.
