
## `ResetRCnt` exact: a unit may name the assembler version of its object (2026-10-11)

- `func_80157784` (52 bytes, the library's `ResetRCnt`, inferred from the
  reference's `libapi/counter.c`) is exact. It was one instruction away:
  its `li v0,1` is an `addiu` in the original, where the build made an
  `ori`, as it does for every such constant of the game and of the rest
  of the library. maspsx turns a small positive `li` into an `ori` for
  assembler versions before 2.56 and leaves it to the assembler from
  2.56 on.
- The build tool. A unit of `build.toml` may have `aspsx_version`, C
  units only. maspsx is run for that unit with it; every other unit
  takes `[toolchain]`'s as before. The cache key holds the version that
  the unit is converted with, so the keys of all other units are what
  they were; the report names the version of a unit that has one.
  `ps1/docs/matching-build.md` describes it. This follows the owner's
  answer of this day for the first case of the kind ("patch": an
  assembler behaviour is fixed in the toolchain, the function is not
  left nonmatching); it is this lane's reading that the answer covers a
  second case, and the pull request says so.
- What the line claims and what it does not. The build shows that the
  unit's bytes come out with that version. It does not show which
  assembler made the object, or that 2.56 is the number: 2.67 and 2.81
  gave the same bytes in a private trial. `GetRCnt` of the same file
  (`sdk_libapi_counter_p1`, exact before) builds to the same bytes with
  either version and is left as it is.
- The unit, `sdk/libapi/counter_p2.c`, is the second part of the
  reference's `libapi/counter.c`, made by hand in the form of the
  importer's parts, with the reference's MIT line and the reference's
  declarations (the same as in part 1). One place differs from the
  reference's text and is a labelled stand-in (order of instructions):
  the store goes through a cast that takes `volatile` off the pointer,
  `((Counter*)D_8002D3B8)[i].rootCounter = 0;`. With the reference's
  plain store the unit builds to 56 bytes and 4 differing instruction
  slots (14 instructions against 13). The first version of the pull
  request declared the pointer without the qualifier in this part
  only; the review pointed out that part 1 declares the same object
  with it, that the two declarations are then incompatible, and that
  the cast at the one use builds to the same bytes. Measured on that
  first version and still true of the declaration: `GetRCnt` in part 1
  needs the qualifier (7 slots without it). What the original source
  had is not known.
- The other three functions of that file are not exact. `StopRCnt`
  (`0x80157750`) in a trial unit: 6 slots with the toolchain's version
  and 4 with 2.56 up to 2.86; the original indexes the mask table
  through the destination register where the build goes through `$at`.
  `SetRCnt` and `StartRCnt` in a private trial outside the build, with
  2.56: one instruction of `StartRCnt` at another place and several of
  `SetRCnt`, also with the compiler's scheduling passes switched off;
  with the toolchain's version they differ in more. The object looks
  like the work of another compiler version; that is a guess.
- The unit defines `ResetRCnt`; its line leaves `symbols.ld`, and
  `func_80157784` stays there as the name its caller in the game uses.
  The inventory sweep now carries the name, and it marks the function
  before it (`0x80157750`) as a library function by its place.
- What ran, on the tree with main at `c774d77` merged in unless said:
  - `fndiff.py --rebuild sdk_libapi_counter_p2`:
    `baseline 52 bytes at 0x80157784, built 52 bytes at 0x80157784, 0 differing instruction slots: IDENTICAL`;
    without the unit's `aspsx_version`: 1 differing instruction slot.
  - `matchbuild.py --image resident`: `RESULT: PASS`.
  - The whole build (the default tag, every image), once, before main
    was merged in, because the tool's conversion step and cache key
    changed: `RESULT: PASS`, resident `functions exact: 1812/1812` and 77
    module images, 4 minutes 44 seconds. 3000 of its 3981 units missed
    the cache, so it does not show that keys stayed what they were; the
    unit case on the key does.
  - `test_matchbuild.py --only unit-aspsx-version`: four new cases, `all
    cases behaved as required`, under a second. `--only
    unit-maspsx-patch`: the seven cases of the patch list, the same.
  - `test_matchbuild.py --only cache-key` (the two controls that build
    with real inputs; one of them is changed here, because a unit with a
    version of its own keeps its key when the toolchain's version
    changes): `cache-key-inputs` and `cache-key-function` behave as
    required, `all cases behaved as required`, 18 minutes 23 seconds,
    on the pull request's first head (the tool, its controls and the
    configuration's version lines are the same in the final one).
  - The map's render:
    `overall: 5521/5600 distinct functions exact (98.6%); 12968/13072 placements (99.2%); 1757312/1809836 code bytes (97.1%)`.
  The whole control set of the build tool was not run. No PC build, no
  differential test, no game.
