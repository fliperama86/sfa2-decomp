## Fixtures for nonmatching functions: record and replay (2026-10-10)

- Why: the owner's decision of 2026-10-10 that a nonmatching function
  comes with a few fixtures for its unit test and that this is all its
  test needs (`docs/efficiency.md`).
- Added to `ps1/src/slot06_nonmatching/difftest.py`: `--record FUNC`
  and `--replay FUNC`. A fixture is one case of the function in terms
  of what it does: the arguments; every byte the original function's
  own instructions read; the calls it made, with the arguments the
  stand-ins logged, the values they returned and, for each watched
  block and each block behind a pointer argument, the bytes that had
  changed at that call against the case's input state; every byte it
  wrote; its result. The file is `FUNC.fixtures.json` beside the C.
- `--record` makes the wide comparison first (it must pass) and then
  keeps a few cases on purpose, twelve unless told otherwise: the
  cases that execute instruction slots no kept case executed; for each
  constant of the `--edges` sweep the first case that notices the
  alteration; one case for each result value not yet kept. What no
  kept case covers is written into the file as uncovered.
- `--replay` builds the C and runs it on each fixture in memory that
  holds only what the fixture says (a filler byte elsewhere, or the
  game's image when the fixture says a callee ran as original code),
  with a stand-in at each callee that returns the recorded values. It
  requires the same calls in the same order with the same changes seen
  at each call, the result, and the writes exactly. It never runs the
  original code. A fixture that the C no longer satisfies fails;
  nothing regenerates fixtures silently.
- First use: five functions of the resident folder, `func_8012fd80`,
  `func_80119694`, `func_8011cf98`, `func_8011a880` and
  `func_801189c4`. Their replay together printed 34 fixtures passed
  and none failed, in under three seconds by the shell's clock,
  compile included (one-off timing, on the tree with main merged in).
  One deliberate error in the C of each, in a scratch copy, made its
  replay fail (for `func_8012fd80` the reach `0x58` made `0x59`: the
  case that the wide comparison had missed that morning).
- Limits: a pass is evidence for the recorded inputs; it is not the
  wide comparison and not equivalence. Replay still needs the private
  inputs of the configuration and the PS1 compiler; a replay of the C
  as compiled for a PC is not built. The memory hook of the emulator
  ended some runs wrongly (a load or store in the delay slot of a
  branch that is not taken, followed by a jump), so the reads are
  found by decoding each executed instruction; a test covers that
  case. Edges that no kept case covers are listed in each file, not
  repaired.
- Ran: the tool's two test files (`all cases behaved as required`),
  with a new group of cases on made-up functions; mutants of the new
  code, each failing named cases, run under a time and a memory
  limit; the three old commands for one function before and after,
  identical; the five records twice, byte-identical files.
