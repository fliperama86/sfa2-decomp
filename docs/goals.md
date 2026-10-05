# Goals and resumption point

Read with [the plan](../PLAN.md), [requirements](requirements.md), and
[project memory](project-memory.md). These documents carry project state across
sessions; there is no separate active goal-tool state that needs to be recreated.

## Long-term outcome

Understandable SFA2 source for the selected PS1 revision, with readable gameplay
C, explicit assembly exceptions, reproducible exact builds of scoped executable
payloads, accurate source coverage, and retained human/AI refinements.

## Completed evidence

- PS1/Windows baseline audits and target comparison.
- Windows three-function behavioral pilot, without exact-byte success.
- Compatible PS1 compiler discovery and exact three-function C reconstruction.
- Rebuild, byte-diff, layout, bounded execution, and mutation evidence for that
  PS1 family. See the detailed reports rather than extrapolating completion.
- Copy-only migration, fresh environment, relocated rebuild/test checks, and
  Ghidra project reopening. Project knowledge now lives in this repository.
- Disc baseline manifest: all 260 files pinned by hash, tool to pin and verify.
- [Matching build](../ps1/docs/matching-build.md): range ownership, whole-image
  link at original addresses, fail-closed comparison with negative controls.
  The rebuilt executable is byte-identical to the baseline.
- Five more functions matched from C: four setup helpers and the smaller of
  the family's two callers. Eight functions, 1,228 bytes in total.
- Native local compiler, checked against saved reference outputs. The pinned
  reference compiler rebuilds the same image.
- The object reset routine and three adjacent helpers matched from C. It is
  the second caller of the first family and calls most of the earlier group.
  Twelve functions, 2,324 bytes in total.
- Static [overlay map](../ps1/docs/overlays.md): archive format, every
  code-bearing chunk on the disc, estimated link addresses.
- First parallel round: five units matched side by side and merged
  mechanically. 33 more functions. 45 functions, 5,548 bytes in total.
- Second parallel round: five more units, 25 more functions. 70 functions,
  9,836 bytes in total. Six functions are parked and stay raw.
- Third parallel round: 21 more functions. 91 functions, 13,316 bytes in
  total. Fifteen functions are parked and stay raw.
- Fourth round, a sweep over small functions: 99 more. 190 functions, 22,572
  bytes in total. 28 functions are parked and stay raw.
- Fifth round, second sweep: 132 more. 322 functions, 35,268 bytes in total.
  46 functions are parked and stay raw.
- Sixth round, third sweep: 179 more. 501 functions, 49,596 bytes in total.
  55 functions are parked and stay raw.
- Seventh round, fourth sweep: 138 more. 639 functions, 63,900 bytes in
  total. 79 functions are parked and stay raw.
- Eighth round, fifth sweep: 130 more. 769 functions, 74,216 bytes in total.
  151 functions are parked and stay raw.
- Ninth round, sixth sweep, plus a retry of every parked function: 152 more.
  921 functions, 87,852 bytes in total. 162 functions are parked and
  stay raw.
- Tenth round, last sweep of small functions: 106 more. 1,027 functions,
  96,944 bytes in total, close to 30 percent of the resident code bytes.
  173 functions are parked and stay raw.
- Eleventh round, first pass over functions of 200 to 300 bytes: 39 of 70
  matched, plus three recovered from the parked pool. 1,069 functions,
  106,776 bytes in total. 201 functions are parked and stay raw.
- Twelfth round, second pass over functions of 200 to 300 bytes: 42 of 70
  matched. 1,111 functions, 117,020 bytes in total. 229 functions are
  parked and stay raw.
- Thirteenth round: the last functions of 200 to 300 bytes and a first 20 of
  300 to 500 bytes. 39 of 63 matched, and five came back from the parked
  pool. 1,155 functions, 128,896 bytes in total, plus the first jump table.
  259 functions are parked and stay raw.
- Fourteenth round, functions of 300 to 500 bytes in game code: 31 of 54
  matched, and one more came back from the parked pool. 1,187 functions,
  141,124 bytes in total, about 43 percent of the resident code bytes.
- Rounds fifteen and sixteen, the rest of the game functions under 1,000
  bytes: 50 of 78 matched. First pass over the functions of 1,000 bytes and
  more, one per agent: 3 of 16. The permutation search and retries of the
  parked pool brought back 18. 22 library functions came in from a public
  reconstruction of the Sony SDK, built unchanged. 1,274 functions, 176,328
  bytes in total, about 54 percent of the inventoried resident code bytes.
- The build tool owns initialised data, bss and assembly units, and rejects a
  symbol file that reassigns a symbol a unit defines. With it the last
  identified SDK files came in: five C files with data of their own, and 42
  BIOS and system call stubs as assembly units. 57 more functions. 1,331
  functions exact: 1,289 from C, 179,396 bytes, and 42 from assembly, 672
  bytes, plus 236 bytes of read-only data and 4,356 bytes of initialised
  data. About 55 percent of the inventoried resident code bytes.
- Parts of SDK files that match the reference only in part: every run of
  functions that is exact as the reference has it came in as a unit of its
  own, 13 units from four files. 26 more functions. 1,357 functions exact:
  1,315 from C, 187,008 bytes, and 42 from assembly, plus 392 bytes of
  read-only data. About 57 percent of the inventoried resident code bytes.
- First header adapted to this SDK version: one struct of the sound library
  is four bytes smaller here than in the reference. With it, 39 functions of
  the largest partly identified file compile exact from unchanged reference
  code. 28 more functions. 1,385 functions exact: 1,343 from C, 200,332
  bytes, and 42 from assembly. About 61 percent of the inventoried resident
  code bytes.
- Reference files adapted to this SDK version by agents, one library area
  each, against a checker that compares every function with the image. 34
  more functions, and seven more files that are whole. 1,419 functions
  exact: 1,377 from C, 214,556 bytes, and 42 from assembly, plus 1,456
  bytes of read-only data and 5,084 bytes of initialised data. About 65
  percent of the inventoried resident code bytes.
- Round seventeen: the six game functions of 1,700 bytes and more, one per
  agent, 2 exact. Fourth permutation sweep, still running at this snapshot:
  16 matches from the first 62 parked functions. Second pass over the
  adapted SDK files: 4 more. 22 more functions in all. 1,441 functions
  exact: 1,399 from C, 223,596 bytes, and 42 from assembly. About 67
  percent of the inventoried resident code bytes. Every game function of
  the resident function inventory has now been attempted. That says nothing
  about overlays or a whole-game inventory.
- The fourth permutation sweep finished: 30 matches from 151 parked
  functions, 14 of them after the last record. The program entry routine and
  ten small stubs are assembly units. 25 more functions. 1,466 functions
  exact: 1,413 from C, 225,816 bytes, and 53 from assembly, 1,040 bytes.
  About 68 percent of the inventoried resident code bytes.
- Second attempt on the parked pool: agents retried 80 parked game
  functions from their earlier candidates, 20 exact. Three more by hand.
  A fifth permutation sweep is running: 2 matches so far. 25 more
  functions. 1,491 functions exact: 1,438 from C, 231,308 bytes, and 53
  from assembly. About 70 percent of the inventoried resident code bytes.
- Third retry wave and the end of the fifth permutation sweep: 9 more
  functions. 1,500 functions exact: 1,447 from C, 233,184 bytes, and 53
  from assembly. 1,152 of the 1,302 inventoried game functions are exact.
  The by-area count of 1,153 game functions has one more: an exact 8-byte
  function in the game area that the inventory does not list.
- The source form for the high byte taken with two shifts was found, in
  units the search had already matched. With it and a sixth permutation
  sweep: 15 more functions. 1,515 functions exact: 1,462 from C, 236,724
  bytes, and 53 from assembly. 1,167 of the 1,302 inventoried game functions.
- Library functions that already compiled exact were not imported, because
  each shared a message string with a neighbour or sat in a file whose
  function order differs here. With that solved, and two functions written
  that the reconstruction lacks: 30 more functions. 1,545 functions exact,
  1,492 from C, 243,804 bytes, and 53 from assembly. 315 of the 347
  inventoried library functions are exact.
- The CD file of the library is whole, with its data. A wave of small
  packages, one function each, made seven more library functions exact,
  four of them without any source to start from and one of 4,480 bytes.
  1,554 functions exact: 1,501 from C, 254,744 bytes, and 53 from
  assembly. 324 of the 347 inventoried library functions are exact.
- The same one-function packages on the parked game pool, in order of the
  search's score: 20 of 50 exact in five waves. With a seventh sweep and
  three search results cleaned by hand: 28 more game functions. 1,582
  functions exact: 1,529 from C, 262,676 bytes, and 53 from assembly.
  1,195 of the 1,302 inventoried game functions are exact.
- The source form for the last residual kind without one was found, in a
  function that was already exact: the same call written once per path,
  where the compiler merges only the call instruction. A comparison of
  every parked function with every exact one found more such twins. With
  both, 50 more one-function packages and an eighth sweep: 25 more game
  functions. 1,607 functions exact: 1,554 from C, 268,396 bytes, and 53
  from assembly. 1,220 of the 1,302 inventoried game functions are exact.
- First one-function attempts on the far and the large parked functions,
  a ninth sweep, and three more source forms, all about locals that are
  assigned more than once: 9 more game functions. 1,616 functions exact:
  1,563 from C, 271,324 bytes, and 53 from assembly. 1,229 of the 1,302
  inventoried game functions are exact.
- Shared struct layouts now come from a field table, and unit work
  directories merge with a tool. See the
  [matching build](../ps1/docs/matching-build.md) and the
  [matching guide](../ps1/docs/matching-guide.md).

## Current checkpoint

This independent repository is the active workspace. Documents, goals, lessons,
and safe tools are publishable; private working artifacts remain local and
ignored. The 68k/CPS2 implementation stays behind. The
[migration record](migration.md) tracks the completed checks and exclusions.

## Next implementation package

The sweep of small functions is done. What remains in the resident image:
See the project memory for the current counts of unattempted and parked
functions; they change with every round.

1. The rest of the library. A first pass of adaptation is done. Open there:
   23 inventoried functions, 9 of them parked, each with a candidate that
   still differs, and four questions that need a
   decision, listed in the project memory: an object that looks assembled
   by another assembler, struct copies that the image does through a call,
   loads that only a volatile field reproduces, and a delay instruction
   that the assembler emulation does not produce.
2. The parked pool: 73 game functions are parked, each with a candidate.
   One function per agent is the method: 44 of 147 packages so far, and the
   rate is falling: 6 of the last 47. All but seven parked game functions
   have had such a package; six of the seven are above 1,500 bytes. What is
   left differs mostly in which register a value gets. The working model
   for that, from a register allocation dump: locals that die once inside
   one block get registers first, the others later by rank, and a local
   that is assigned more than once loses what the compiler knew about its
   upper bits. The forms that follow from it made two functions exact and
   brought nine others closer, two of them to 4 and 12 differing slots.
   Next: apply the model function by function at the top level, with the
   allocation dump, before more waves.
3. Every game function of the resident inventory has been attempted. Four
   of the six largest are parked with candidates that have the right size or
   nearly.
4. Replace `func_<address>` names where several units agree on a role, turn
   the recorded casts and wrapper structs into proper members, and move
   repeated prototypes into shared headers. Every such change must rebuild
   exact.
5. Data is owned per unit. Game data tables still have no owner. The four
   words after the program entry routine, a table it reads, have none either.

## Overlays

The [overlay map](../ps1/docs/overlays.md) records the archive format, every
code-bearing chunk on the disc and its estimated link address, and what the
three out-of-image calls in the matched code resolve to. It is static analysis.
Still open: confirming the map from the resident loader code, and how the
matching build should own overlay blocks, including one source linked at two
addresses for the two player sides.

Do not make game booting, runtime injection, rebuilt-disc packaging, or a new
emulator integration a prerequisite for accepting exact code. Use runtime
observation only when it answers a specific remaining question.

## Not yet accomplished

Whole executable/overlay source inventory, a whole image built from source
rather than mostly retained bytes, ownership of game data, assembly owners
beyond the SDK stubs, overlay images, complete gameplay reconstruction, SDK exception accounting, final
reproducible delivery, and source-publication policy review. Existing private source is not
implicitly cleared for Git publication by the repository migration.

Windows VC5 matching remains optional comparison work, not a blocker for PS1.
The generic 68000 backend and CPS2 reconstruction remain in the original repo,
outside this project's current execution plan.
