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
- Repeated prototypes moved into one shared header: 781 functions, one
  prototype each, chosen by rebuilding. The units lost 2,039 prototype
  lines and stay exact. No count of exact functions changed.
- Data externs that all units declare the same way moved into a second
  shared header: 594 symbols. The units lost 1,218 extern declarators and
  stay exact.
- Five function pointer typedefs and 154 callback symbols followed into
  the same header. The units lost 68 typedef lines and 257 declarators and
  stay exact.
- Of the 49 data symbols that units declared differently, 37 have one
  declaration that keeps every unit exact, found by rebuilding, and moved
  into the header too. 12 have none.
- The reconstructed PS1 source is published in `ps1/src/`: 760 files. The
  SDK headers of another project, which it needs to build, are not.
- One more game function, done at the top level with the compiler's
  dumps instead of an agent package: a 16-bit local scaled in place with
  a compound shift. 1,617 functions exact: 1,564 from C, 271,532 bytes,
  and 53 from assembly. 1,230 of the 1,302 inventoried game functions
  are exact.
- The overlay map is confirmed against the resident loader: an exact
  function indexes destination tables by two fields of an archive entry,
  and for all 16 code-bearing slots the table's address equals the
  address estimated from the code. A word-by-word comparison of the two
  sides' blocks supports, without proving, that a second-side block is its
  first-side block linked at a fixed distance.
- An inventory of the functions inside the overlay modules, estimated by a
  static sweep and not yet rebuilt: 11,220 functions in the 77 distinct
  contents of 16 slots. With link addresses set aside 3,749 of them are
  distinct, 677,624 bytes. The four modules with one content hold 959
  functions, 129,348 bytes. Against the resident inventory the same sweep
  finds all 1,649 starts and 1,643 sizes, and reports 202 functions that
  the inventory does not list.
- Shared struct layouts now come from a field table, and unit work
  directories merge with a tool. See the
  [matching build](../ps1/docs/matching-build.md) and the
  [matching guide](../ps1/docs/matching-guide.md).

- A round on the game functions that the first inventory had missed, the
  owner's choice of 2026-10-06 for the next item: 118 of the 133 are
  exact, 11,784 bytes, and 15 are parked with candidates. 1,735 functions
  exact: 1,682 from C, 283,316 bytes, and 53 from assembly. By area the
  build has 1,349 of the 1,436 game functions that the sweep counts. Of
  the 1,302 inventoried game functions 1,230 are exact, as before.

- Second attempts on the 15 functions that round had parked, one function
  per agent: 11 are exact, 2,156 bytes, and 4 stay parked. 1,746 functions
  exact: 1,693 from C, 285,472 bytes, and 53 from assembly. By area the
  build has 1,360 of the 1,436 game functions that the sweep counts.

- Nonmatching C, by the owner's decision of 2026-10-09: the functions of
  the stage modules and of the character modules `PL0C` to `PL17` that
  stayed short of exact are published in
  [`ps1/src/slot06_nonmatching/`](../ps1/src/slot06_nonmatching/README.md)
  and
  [`ps1/src/slot04b_nonmatching/`](../ps1/src/slot04b_nonmatching/README.md),
  each with its contract and a differential test against the original code
  under an emulator; the two pages hold the test's lines. They are not in
  the build and no count above changes. A pass of the test is evidence
  for the tested inputs, not equivalence. The same is in work for the
  resident image and the other modules, because the port needs C for
  every function.

## Current checkpoint

This independent repository is the active workspace. Documents, goals, lessons,
tools and the reconstructed PS1 source are published; inputs, binaries and
working artifacts remain local and ignored. The 68k/CPS2 implementation stays behind. The
[migration record](migration.md) tracks the completed checks and exclusions.

## Port

The owner started a port on 2026-10-06. It is a lane of its own beside the
matching work; its decisions, its state and what is not decided are on the
[port page](../port/README.md). Done so far: PsyZ is pinned and its own
tests were run, a tool compares the library functions that the game
calls with what PsyZ has, another compiles the game's C units with a
PC compiler and counts what does not carry over, and a third lists what
the compiled units need and none defines. One workflow runs the compile
trial and another PsyZ's own tests on runners for Linux, macOS and
Windows. The owner's rule of 2026-10-09: no change to the game's source
until the port runs, data from the user's disc image, the proof on Linux
or Windows first. On the same day the owner settled how it runs before
every function is C: it does not; a function without C gets nonmatching
C first, and the PC program stops with the function's name where C is
missing. The first piece of the PC program exists since then: a tool
links the game's unchanged C for 32-bit Windows with every name at its
PS1 address, and the program loads the game's program from a disc image
and stops at the first function without C, the game's `main` in the
published tree. Next: the library, the modules, and C for the functions
that have none.

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
2. The parked pool: 69 game functions are parked, each with a candidate:
   65 of the inventory, to which the rest of this item refers, and 4 from
   the round on the functions outside it. A round with four source forms
   found on the overlay modules, the owner's choice of 2026-10-06, made 7
   of the 76 that were parked then exact; the other 69 kept their
   candidates, some of them closer. A round on all of them with the
   compiler's pass dumps (2026-10-07, see the matching guide) made 4 more
   exact, and one parked library function: 65 game functions are parked
   now.
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
   allocation dump, before more waves. Started: the closest function
   became exact that way, the second closest did not. The thirty-eighth
   group of the project memory has what the dumps showed.
3. Every game function of the resident inventory has been attempted. Four
   of the six largest are parked with candidates that have the right size or
   nearly.
4. Replace `func_<address>` names where several units agree on a role and
   turn the recorded casts and wrapper structs into proper members. Every
   such change must rebuild exact. Prototypes are done: one shared header
   for 781 functions. 85 functions keep prototypes in their units, 66 of
   them because the callers need one signature and the definition has
   another; the list is a work item of its own, and was worked through
   on 2026-10-09: 222 declarations in resident units differed from a
   definition and 1 still does, and the 22 functions that the shared
   header declared without a prototype have one (the project memory
   has the account). The dispatch tables of the resident image followed
   the same day: of 539 functions that stand in a declared table, 150
   differed from the table's entry type and none does now. The
   prototypes that units still carried at their top then moved into the
   shared header wherever every declaring unit agrees, so that the
   compiler compares them in every unit; the few functions that stay in
   units, and why, are in the project memory. Data externs
   are done
   where all units agree: 594 symbols in a shared header. 138 symbols stay
   in units at that step. The callback tables followed: five function
   pointer typedefs have one shared definition and 154 callback symbols
   moved. Then 37 of the 49 symbols on which units disagreed moved, each
   with a declaration that keeps every unit exact. 18 data symbols and 1
   function pointer symbol are still declared only in units. 12 of them
   have no common declaration: for 7 the compiler rejects some unit with
   every candidate, for 3 every candidate gives nonmatching code in some
   unit, 2 show both. These 12 are worth reading one by one.
   Turning wrapper structs into proper members is a separate step that
   these counts say nothing about; names are untouched.
5. Data is owned per unit. Game data tables still have no owner. The four
   words after the program entry routine, a table it reads, have none either.

## Overlays

This is the current focus, by the owner's decision of 2026-10-05.

The [overlay map](../ps1/docs/overlays.md) records the archive format, what
the exact loader functions do with an archive entry, the loader's destination
tables, every code-bearing chunk on the disc with its link address, how
the second-side blocks differ from the first-side ones, and an estimated
inventory of the functions inside the modules with what they share. No game
was run.

Next, in this order:

1. A proposal for how the matching build owns an overlay block: a baseline
   that is a chunk of an archive instead of the executable, and, as the
   proposed treatment of the two sides, one source linked at two addresses.
   That treatment is an inference from the comparison until both links of
   a block rebuild exactly. All seven decisions of the
   [proposal](../ps1/docs/overlay-build-proposal.md) are settled: the owner
   accepted the design as written on 2026-10-06 and decided that module
   source is published. The tool changes come in four steps; the first,
   module images with a chunk baseline, is in the
   [matching build](../ps1/docs/matching-build.md).
2. Done: a fast loop for one unit, approved by the owner on 2026-10-06.
   `fndiff.py --rebuild` rebuilds a single unit and compares it, so that a
   try no longer pays for the whole build. The whole build stays the gate.
3. Done: `fndiff.py` compares a unit of a module image with its own
   image, and control cases show that `mergeunits.py` carries the `image`
   key and treats the image tables as base.
4. Done: names across images. Every link is given the declared functions
   of the other images, so module code and resident code can call each
   other by name.
5. Done: second links, the last tool step of the proposal. A module image
   may be like another: the same units linked again at its own address and
   compared with its own chunk.
6. Done: a pilot, one overlay module, or part
   of one, rebuilt exactly. First target, the module of slot `0x2a`: all
   28 functions that the sweep counts are exact, 6,052 bytes.
   Two of the three that were parked became exact with the bit-field form
   of the tag word, and the last with a store to another field between
   two stores to one field.
   Second target: the module of slot `0x16` is declared, and all 11
   functions that the sweep counts in it are exact, 984 bytes. Its second
   side, slot `0x17`, is declared as a second link of it. The same 11
   functions, linked again at the other address, are exact against the
   second chunk. For these 11 the treatment of one source and two links
   has passed its test; no other block has been tried.
   The pilot has shown what it was for: module code rebuilt exactly inside
   the build, and both sides of a block from one source. It ends here, with
   the three parked functions carried over to the search.

After the pilot, in this order, by the owner's decisions of 2026-10-06:

1. Done: the resident functions that the sweep found and the inventory
   does not list are sorted. They are 203, one more than counted before:
   the code goes on for one function after the inventory's end. 134 start
   in the game area and 69 in the library area; the build already has 1
   and 62 of them. The resident image has 1,852 functions by the sweep,
   1,436 game and 416 library, and the build has 1,231 and 386 of them.
   The 203 are counted as candidates: a boundary from the sweep and an
   approximate split by area, each established only when it is rebuilt.
   See the [overlay map](../ps1/docs/overlays.md).
2. A tool that labels each game function by the family of Sony library
   functions it calls, directly or through other functions: graphics,
   sound, disc, pads. It tells code that touches hardware from game logic
   before proper names exist, and prepares naming.
   Done: `families.py` and the
   [family page](../ps1/docs/library-families.md). In the resident image
   91 of 1,436 game functions call the library directly, and 1,117 reach
   no library function. In the overlay modules, each content taken alone,
   121 of 11,220 functions call it directly and 8,105 reach none. Open:
   116 library functions have no family yet, and calls through a register
   are not followed.

The list above is done. Of three candidates for what comes next the owner
chose the first on 2026-10-06:

- Done: a round on the 133 game functions of the resident image that the
  inventory did not list, and second attempts on what it parked. 129 are
  exact and 4 parked.
- Done as far as automatic means go, the owner's choice of 2026-10-06 for
  the item after that: identifying the library functions without a family. They were 116. 32
  now come from the reference's source for the same function, with its
  name. 28 more have a name by reference: units built from the
  reference's source call them under it, and `librefs.py` lists that from
  the build. 21 more got a family by their place between two functions
  of one reference file, an inference. Then 17 more were taken from the
  reference's source, placed by what they call. Now 384 of the 416 library
  functions have a family and 364 have a name. 32 are left without a
  family: 24 that the build has under a placeholder name and 8 that it
  does not have.
- In progress, the owner's choice of 2026-10-06 once the automatic ways
  of naming library functions were used up: more overlay modules as images
  of the build. The first of them is the one content of slot `0xb`, a
  module of 61 functions that 24 archives carry: 56 are exact, 5,652 of
  7,960 bytes, and 5 are parked with candidates. The
  second is the one content of slot `0x12`, a module of 187 functions that
  42 archives carry: 178 are exact, 26,224 of 29,928 bytes, and 9 are
  parked with candidates. Four source forms were found on the way
  and are in the matching guide: the tag word of a primitive is written
  with bit-fields; a pointer held in two variables keeps a store and
  a load in source order; the order of statements inside a block is not
  the listing's; and a narrow parameter that callers do not mask is an
  `int` copied into a narrow local. Tried on the functions parked before
  they were known, they made 5 of 12 module functions and 7 of 76 game
  functions of the resident image exact. The third is the pair of slots
  `0x0` and `0x8`, one character's extra module at two addresses, 112
  functions each: 109 are exact, 12,240 bytes, in the first and, linked a
  second time, in the second; 3 are parked with candidates. The fourth
  is the pair of slots `0x2b` and `0x2c`, another character's extra
  module, 107 functions each: 102 are exact, 9,052 bytes, in the first
  and, linked a second time, in the second; 5 are parked with candidates.
  The fifth is the one content of slot `0xf`, from `DEMO.PAC`, a module of
  228 functions: 215 are exact, 32,712 of 36,912 bytes, and 13 are parked
  with candidates. 63 of its functions are the same code, apart from
  addresses, as functions that were already exact, 37 of them in the
  module of slot `0x12`; their source was copied from those.
  The sixth to the twenty-fifth are the 20 stage modules, the contents
  of slot `0x6` in `STAGE00.PAC` to `STAGE13.PAC`, 874 functions by the
  sweep: 845 are exact, 139,908 bytes. Of the 29 that are not, most are
  the function that draws the tiles of an object, one per stage: it has
  a candidate and is not exact yet. The functions that draw the tile
  layers, flat and in perspective, are exact in every stage that has
  them. The stage files are the work
  of a second session that runs beside the first, by the owner's
  decision of 2026-10-06.
  The twenty-sixth is the one content of slot `0x27`, from `SELECTA.PAC`, a
  module of 212 functions: 208 are exact, 30,184 of 31,684 bytes, and 4
  are parked with candidates. 46 of its functions are the same code,
  apart from addresses, as functions that were already exact, 45 of them
  in the resident image.
  The next eleven are first-side character blocks, the contents of slot
  `0x4` in `PL0C.PAC` to `PL17.PAC` (`PL13.PAC` has the bytes of
  `PL11.PAC`), 2,333 functions by the sweep: 2,311 are exact, 277,928
  bytes, and five blocks have every function exact. The second sides,
  slot `0x5` of the `X` files, are the same units linked a second time
  and exact against their own chunks; `PL15`, `PL16` and `PL17` have
  none. Two units of `PL11` stay raw on the second side: they call that
  character's extra module, which the second side has at another
  address, and a second link cannot be given another image's moved
  functions yet. The functions that are not exact are parked with
  candidates. These blocks are the second session's work too.
  The thirty-eighth is the one content of slot `0x28`, from `END00.PAC`, a module
  of 683 functions: 677 are exact, 83,960 of 85,408 bytes, and 6 are
  parked with candidates. It repeats itself: its 683 functions are 319
  apart from addresses. One of each kind was written first, and 363 of
  the rest were then written by a helper that copies an exact function's
  source and maps its addresses; the build found 358 of those exact as
  written.
  The thirty-ninth is the one code-bearing content of slot `0x1`, from
  `CDEMO00.PAC`, a module of 124 functions: 116 are exact, 16,100 of
  20,432 bytes, and 8 are parked with candidates.
  Then the first-side blocks of twelve of the 24 character files,
  `PL00.PAC` to `PL0B.PAC`: 1,562 of their 2,116 functions are exact,
  197,460 of 257,648 bytes. 534 of the rest are the same code as
  functions of the other twelve files, which the second session writes,
  and wait for that source; 20 are parked. Eleven of the twelve are also
  linked at the second address and are exact there.
  Then the content of slot `0x4` of `SELECT.PAC`, a module of 91
  functions: 89 are exact, 27,092 of 27,984 bytes, and 2 are parked with
  candidates.
  Then third attempts on the 73 functions that these rounds had parked
  in the modules of this session, with the compiler's pass dumps (the
  matching guide says how they are read): 15 are exact now, one or two
  in each of `slot0b`, `slot12`, `slot2b`, `slot0f`, `slot28` and
  `slot01` and seven in the character blocks.
  Then the functions of the twelve character blocks that are the same
  code as functions of the other twelve files, once that source was in
  the tree: a helper wrote 530 of them from it and 529 are exact as
  written. The twelve blocks have 2,098 of their 2,116 functions now,
  252,908 of 257,648 bytes.
  Then the second side of `PL06.PAC`, which the second links had left
  out: it is four bytes shorter than the first side, and is built as an
  image of its own from copies of the first side's units, 228 of 231
  functions exact.
  Then two more functions copied from twins that had become exact
  meanwhile, one in `slot28` and one in `slot04_02`, and the figures in
  the comments of this session's units measured again.
  Then a search over the integer types of the locals of every parked
  candidate of this session, each variant compiled alone and scored by
  its bytes: 2 of 132 are exact, one in `slot12` and one in `slot0f`,
  and 42 more are closer than they were. The counts of the steps above
  are those of their day; the README has the current ones.
- Not decided: finishing the library by hand, which means adapting the
  reference's files one by one. Some of it waits for rulings on four
  questions that the project memory lists.
- Open, and the model for it is item 2 above: most of the 65 parked game
  functions differ in which register a value gets. The compiler's pass
  dumps now show, for each, which pass decides and what it ranks by. For
  4 functions that was enough to find a form; for the others the
  candidates carry what the allocation or the scheduler would need, and
  no source form gives it yet.

Still open in the map itself: how the loader treats the entries without a
table destination, what owns 11 data symbols above the stage blocks, and a
call into the middle of a function in one character block.

Open in the resident image: 65 game functions that the sweep counts are
not in the build, all of them parked with a candidate. The 15 functions
outside the inventory that have no reference that the sorting counts are
all exact now; what reaches them is still unknown.

Do not make game booting, runtime injection, rebuilt-disc packaging, or a new
emulator integration a prerequisite for accepting exact code. Use runtime
observation only when it answers a specific remaining question.

## Not yet accomplished

Whole executable/overlay source inventory, a whole image built from source
rather than mostly retained bytes, ownership of game data, assembly owners
beyond the SDK stubs, the source of the overlay modules beyond the parts of
the images `slot2a`, `slot0b`, `slot0f`, `slot12`, `slot16`, `slot27`, `slot28`, `slot01`, `slot00`, `slot2b`, the 20 stage images `slot06_00` to `slot06_13` , the 12 character images `slot04_00` to `slot04_0b` , the 11 character images `slot04_0c` to `slot04_17`, `slot04_sel` and `slot05_06` that are rebuilt so far
(`slot17`, `slot08`, `slot2c`, the eleven images `slot05_00` to `slot05_0b` without `slot05_06` and the eight images `slot05_0c` to `slot05_14` are linked from the source of `slot16`, `slot00`, `slot2b` and the character images), every other module as an image of the
build,
complete gameplay reconstruction, SDK exception accounting, final
reproducible delivery, a license for the published source, and a build path
from a fresh clone.

Windows VC5 matching remains optional comparison work, not a blocker for PS1.
The generic 68000 backend and CPS2 reconstruction remain in the original repo,
outside this project's current execution plan.
