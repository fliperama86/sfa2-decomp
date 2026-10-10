## The published fixtures replayed on the functions as compiled for the PC (2026-10-10)

What was added: `port/tools/hostfixtures.py` with its tests
(`port/tools/test_hostfixtures.py`) and a section "Replaying fixtures" in
`port/replay/README.md`. The tool reads the published
`FUNC.fixtures.json` files of format 1, builds each function's C with
the port's compiler on the harness of `port/replay/`, runs the fixtures,
and says per function whether the PC build does what the original did on
the recorded inputs: how the call ended, the calls and their arguments,
the words behind pointer arguments and the watched blocks at each call,
the result under the width of the function's return type, and the writes
exactly. It needs the cross compiler and the published tree, nothing
private; it does not import `difftest.py`, whose header is the
definition of the format. The saved registers and the stack pointer are
not compared: they do not exist in the PC program.

Two changes to the harness's driver (`port/tools/hostreplay.py`) came
with it. A program on disk is reused only for what it was made from (a
note beside it names the source, the configuration and the compiler; a
failed build takes the note away): before, another source file for the
same name got the first file's program when it was older. And the
configuration, the symbol file and the fields file are read once for
each state of the three instead of once for each program: a run over
the published functions took about a minute with that reading in every
build, and takes about six seconds without (one-off timings, eight
functions at a time, a fresh build folder and a built one alike).

The first run over the published tree,
`python port/tools/hostfixtures.py --config ps1/src/build.toml --cc CC --jobs 8`,
prints as its last line
`native replay: functions replayed 99, all passed 98, with a failure 1, not built 0, not replayed 32`.
The 99 are the functions with a file of format 1 outside
`library_nonmatching` (the PC build does not compile Sony's library C);
the 32 are the files of format 2 (calls of original callees), which need
two additions to the harness and are not replayed yet.

What the first runs found, and what became of it:
- Three functions failed because the tool did not hand them their fifth
  argument (`func_80152124`, `func_80012990_slot01`,
  `func_801e0ce8_slot0b`): the console passes it on the stack, a fixture
  states it as memory read at 0x801ff010, and a C call takes it as its
  fifth word. The tool now passes the words stated there as the fifth to
  eighth arguments. A gap of the tool, not of the functions.
- One function fails and is left failing, with its reason on the page:
  `func_80119694` reaches RAM through the mirror at the bottom of the
  address space (it cuts the top byte off an address). The PC program
  serves such accesses with `port/src/mirror.c`; the harness does not
  have that part. A limit of the harness, not a difference of the PC
  program that this run shows.
So this run shows no function whose PC build differs from the original
on its recorded inputs, among the 98; it does not show that for the
functions it cannot replay.

Tests: `python port/tools/test_hostfixtures.py --cc i686-w64-mingw32-gcc`,
"all 166 tests passed"; `python port/tools/test_hostreplay.py --cc ...`,
"all 103 tests passed"; mutants in bounded runs, each failing a named
test: 39 of the reader, the case builder and the comparator (a fortieth
was dead code, which was removed), 6 of the fifth-argument rule, 6 of
the note of origin, 6 of the configuration read once.

Not done and not shown: format 2 is not replayed; the mirror is not in
the harness; a pass is evidence for the recorded inputs, not
equivalence; nothing here runs the PC program itself.
