## Batch 13: the last five functions of the library's graphics file exact (2026-10-11)

- `PutDispEnv` (`0x80158470`, 1060 bytes), `_clr` (`0x801591a4`, 544),
  `_dws` (`0x801593c4`, 604), `_drs` (`0x80159620`, 668) and `_addque2`
  (`0x801599f0`, 764). With them every function of the library's
  `libgpu/sys.c` that the coverage list showed open this afternoon has
  a unit: nine functions in batches 12 and 13. The tree is made from
  main at `2c86c47`, which has batch 12.
- The text has the three layers that the library page names since the
  review of pull request 291: the reference's file at the pinned commit
  (MIT, notice kept), this project's adapted copy of it, and this
  batch's changes. What the adapted copy had changed before this batch,
  from a comparison of each function with the pinned commit's file:
  `_addque2` is another body; `_clr`, `_dws` and `_drs` take the upper
  limits of their rectangle from the library's state where the pinned
  text has constants; `PutDispEnv` reads four things as fields of the
  library's state where the pinned text has a variable for each.
- `_addque2`, at the top level, nothing else read: three casts to
  `volatile` on the adapted copy's text, one stand-in at three places:
  the store that sets the pending flag, the read of the queue switch
  in the next statement, and the store of the first of the call's
  three words. The read of the queue switch has the same instructions
  with and without its cast; the cast keeps it behind the store before
  it (without it the build loads the switch before that store). With
  all three taken out: 756 bytes, 18 differing instruction slots. Each
  alone, in that order: 12, 16 and 11 slots. Forms that were tried and
  are not in the unit: a local for the interrupt mask, stored after the
  flag (5 slots with two of the casts); the flag's store through a
  struct type (no gain).
- `_clr`, `_dws` and `_drs`, by two workers of this lane within a few
  minutes each, briefed with the levers of batch 12: the rectangle is
  limited in place (the adapted copy limited a copy of it), from 1
  where the adapted copy had 0, each upper limit read once into a local
  inside the conditional, and through the cast: one stand-in at two
  places in each function. Both casts taken out: `_clr` 520 bytes and
  13 slots, `_dws` 580 and 13, `_drs` 644 and 12. One alone (the width,
  the height): 8 and 7, 8 and 7, 7 and 7. Measured at the top level on
  the final text, with the function named to the tool.
- `PutDispEnv`, by a third worker: three ordinary changes of code
  (the type tested against 1 and 2 where the adapted copy tested it
  against 0; the mask 0x3FF for 0x1FF in the other form; the first
  line adds 0x13 when the environment's `pad0` is not 0, and 0x10
  otherwise) and two reads through the cast, of the debug level and of
  the reverse flag: one stand-in at two places. Both taken out: 1048
  bytes, 22 slots; one alone: 19 and 6. A cast on the type, which the
  worker's first form had, was not needed and is not in the unit.
- What the workers read: for `_clr`, `_dws`, `_drs` and `PutDispEnv`
  the same functions in the PSY-Q decompilation that PsyZ carries
  (`decomp/src/libgpu/sys.c`, MIT, named on the library page since
  batch 11), for the limiting in place and the two values of the first
  line. The units keep this folder's names and declarations.
- The units: `sdk/libgpu/sys_p14.c` (`_addque2`), `sys_p15.c` (`_clr`,
  `_dws`, `_drs`, one run of 1816 bytes) and `sys_p16.c`
  (`PutDispEnv`, with 24 bytes of read-only data at `0x8016dc88`, its
  message), made by hand in the form of the importer's parts by a
  private generator from `sys_p3.c` and the texts above. The comments
  are the top level's, in the guide's form.
- Other files: `symbols.ld` loses `_addque2` and gets two data names of
  the adapted copy that the parts refer to, `D_intr_addque2`
  (`0x80181aa0`) and `D_80037E20` (`0x8018d27c`), their addresses read
  from the original's instructions. The inventory carries the five
  names. The library page: counts (97 C files, 43 parts, ten made by
  hand) and the paragraphs on the three parts with their layers.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `sdk_libgpu_sys_p14`: `baseline 764 bytes at 0x801599f0, built 764 bytes at 0x801599f0, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p15`: `baseline 1816 bytes at 0x801591a4, built 1816 bytes at 0x801591a4, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p16`: `baseline 1060 bytes at 0x80158470, built 1060 bytes at 0x80158470, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the part with that
  change, the part put back and built again. The map's render:
  `overall: 5548/5600 distinct functions exact (99.1%); 13000/13072 placements (99.4%); 1772612/1809836 code bytes (97.9%)`.
  No whole build, no PC build, no differential test.
