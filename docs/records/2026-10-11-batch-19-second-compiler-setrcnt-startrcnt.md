
## Batch 19: a second compiler for single units; `SetRCnt` and `StartRCnt` exact (2026-10-11)

- `SetRCnt` (`0x8015764c`, 156 bytes) and `StartRCnt` (`0x80157720`, 48
  bytes), functions of the library's `libapi/counter.c` that had no
  unit. Their text is the reference's at the pinned commit, unchanged
  (compared with that commit's file).
- Why they were open: with the project's compiler, GCC 2.6.3, `SetRCnt`
  comes out 4 bytes longer than the original and `StartRCnt` has one
  instruction at another place, from the reference's text and from
  every other text tried. Earlier notes said "looks like another
  compiler version".
- The owner's decisions of this day, each asked and answered with
  "yes": first a private trial with GCC 2.7.2, then with the other
  versions that the same source offers, then, after the result, a
  second compiler for single units in the build.
- The trial (private, nothing of it in this repository): the builds of
  the project old-gcc, release 0.13, for 2.5.7, 2.6.0, 2.7.0, 2.7.1,
  2.7.2 and 2.8.1, each run on the two parts with the library's flags
  and the assembler version of `counter_p2`. Words that differ from the
  original, `SetRCnt` and then `StartRCnt` with `StopRCnt` (one part in
  the trial): 2.5.7: 0 and 6 (all six in `StopRCnt`); 2.6.0: 160 bytes
  and 7; 2.6.3: 160 bytes and 7; 2.7.0: 160 bytes and 10; 2.7.1 and
  2.7.2: 5 and 10; 2.8.1: 10 and 23 (108 bytes). Only 2.5.7 gives
  `SetRCnt` and `StartRCnt`.
- The build tool: a unit may name a compiler of its own, `cc1 = "NAME"`,
  a table `[toolchain.cc1_unit.NAME]` with the keys of `[toolchain.cc1]`.
  Every such compiler is pinned by the hash of its binary and checked
  before any unit is looked up. The unit is compiled with it; the cache
  key holds its hash and table name; `--reference` leaves it alone; the
  report names it. A name without a table, a table without a unit and
  the key on an assembly unit are configuration errors. This is the
  form of the per-unit assembler version of pull request 285. The
  reading of one compiler table moved into a function of its own, with
  no change of what it accepts.
- Seven unit cases, isolated, half a second together: how the unit's
  key is read, how the tables are read (seven wrong forms), the two
  cross errors, which compiler a unit gets (also under the reference
  configuration), the cache key (six statements, among them that the
  two tables are told apart where both pin one binary), the pin check
  with a right and a wrong hash, and what the configuration names (each
  unit compiler has a unit, pins another binary than the default, and
  the binary on this machine has the pinned hash).
- Three controls on the real build, each run once: the two units
  without their `cc1` line: `SetRCnt` 160 bytes and 5 differing
  instruction slots, `StartRCnt` 48 bytes and 2 slots; the unit
  compiler's pin with one digit changed: `FAIL: cc1_unit.gcc-2.5.7
  sha256 mismatch`, status 1; a unit naming `gcc-9`: `CONFIG ERROR ...
  is not a compiler of [toolchain.cc1_unit]`, status 2.
- The compiler: the `cc1` of `gcc-2.5.7-psx.tar.gz` of old-gcc release
  0.13, pinned in `build.toml` by its hash. The archive and the binary
  are not in this repository. It is a compatible compiler: that the two
  functions come out of it says that it generates the code the
  library's builder generated for them, not that the library was built
  with this binary.
- Still open in that file: `StopRCnt`. GCC 2.5.7's own text has its
  instructions (the `and` into the mask's register, the constant 1 in
  the return's slot), but the original has the second load's upper half
  before the first load and the destination register as the
  temporary, which the assembler step of this build does not do.
  Inferred from the compiler's text and the original's instructions.
  `_card_clear` of the memory card library is open too: with 2.5.7 and
  the parameter's address taken it is 4 words off (the return address
  at another frame offset, and the return sequence in another form).
- `GetRCnt` and `ResetRCnt` keep the default compiler: in the private
  trial both build to the same bytes with 2.5.7 (the second with its
  published cast), so nothing shows which compiler they need.
- Other files: `symbols.ld` loses the two names that the units define
  and gets two data names of the reference that they refer to
  (`D_8002D3B4`, `D_8002D3BC`, addresses read from the original's
  instructions). The inventory carries the two names. The library page:
  counts (99 C files, 45 parts, twelve made by hand) and a paragraph.
  The build page: the table, the unit key, a paragraph each under
  "Configuration" and "Compilers", the pin step and the cache key's
  list. The project memory page: the decision.
- What ran: the seven unit cases and the four of the assembler version
  (`test_matchbuild.py --only unit-cc1`, `--only unit-aspsx`): all as
  required. `fndiff.py --rebuild`:
  - `sdk_libapi_counter_p3`: `baseline 156 bytes at 0x8015764c, built 156 bytes at 0x8015764c, 0 differing instruction slots: IDENTICAL`
  - `sdk_libapi_counter_p4`: `baseline 48 bytes at 0x80157720, built 48 bytes at 0x80157720, 0 differing instruction slots: IDENTICAL`
  `matchbuild.py --image resident`: `RESULT: PASS`. One whole build,
  because the toolchain changed: `RESULT: PASS` for the resident executable and the 77 module images, 5 minutes 29 seconds with an empty object cache (the run's own count: 2 hits, 3962 misses). The map's render:
  `overall: 5557/5600 distinct functions exact (99.2%); 13012/13072 placements (99.5%); 1776480/1809836 code bytes (98.2%)`.
  That units without the new key keep their cache keys was also
  compared on the resident image: the keys of all 936 units in the
  report of the tool before this change (the tree of batch 18) and in
  this tree's report are equal (886 with a key, 50 assembly units with
  none). The build controls of the nonmatching folder, which go through
  the same functions of the tool (`test_difftest_build.py`): all cases
  as required, 10 seconds. The tree is made from main at `723efc2`.
  No PC build, no differential test of a function, no game.
