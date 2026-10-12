## Batch 26: `func_800e80ec_slot0f` and its twin `func_8001606c_slot12` exact, plain C (2026-10-11)

- `func_800e80ec_slot0f` (384 bytes, image `slot0f`) and
  `func_8001606c_slot12` (384 bytes, image `slot12`): one function in
  two modules with other tables (one distinct function for the map, two
  placements). Both from `modules_nonmatching`. The tree is made from
  main at `cb47613`; batch 25 (pull request 304) is not in it.
- No stand-in and no label in either unit. The earlier candidates (2
  differing lines each since 2026-10-10) carried two do-while wrappers and a
  term that is always 0; none of that is left.
- What the original shows: each arm of the first test loads its
  sequence table into a saved register, passes it to the first call,
  and nothing reads the register again; the first argument's copy
  stands at the top of the second arm; the second call takes its table
  as a constant address.
- What closed it, in the order it was found:
  - Read from the compiler's scheduler trace (inferred): the order of
    the second arm is what the compiler gives when the table's
    register is set once in the function, not once per arm in one
    shared variable. So each arm has a local of its own for its table.
  - A worker of this lane, given that reading, found that such a local
    must be read in another block to get its register from the pass
    that serves values of several blocks, and wrote the second,
    conditional call into each arm with a term on the local that is
    always 0. That form was identical.
  - Reduced at the top level: the statements behind the second call
    need not stand twice. Then the twin was built with the same form
    and measured without the term: identical. Its three tables are
    entries 0, 4 and 8 of one array, and the second call's table
    (entry 4) is then the read of the arm's local that the term had
    stood in for (inferred from the result, not read in the dumps: the
    compiler forms that address from the register at first and from
    the constant in the end). The three tables of the
    `slot0f` unit lie 16 bytes apart in the same way; written as one
    array (entries 0, 4, 8 of `data_800f00fc_slot0f`), that unit is
    identical without the term as well.
- So the units' text: the first call and the second, conditional one
  written in each arm, the arm's table in a local of the arm, and the
  tables as entries of one array. Measured on the final text of each
  unit with that one change:
  - one call of each kind behind the arms, the table in one local set
    in both arms (the nonmatching files' shape): 368 bytes and 24
    differing instruction slots, in both units;
  - `slot0f`: the three tables as three arrays of their own
    (`data_800f00fc_slot0f`, `data_800f010c_slot0f`,
    `data_800f011c_slot0f`): 396 bytes, 31 slots;
  - `slot12`: the object's `field_78` read without the cast to a signed
    halfword at the last statement: 15 slots. The `slot0f` unit has no
    cast there and needs none (its record type differs).
- The symbols `data_800f010c_slot0f` and `data_800f011c_slot0f` stay in
  `symbols.ld`; no unit names them now. That the three are one array is
  inferred from this build and said so in the unit.
- The units' comments of what the function does are the nonmatching
  files', without the contracts (the `slot0f` one names the tables by
  entry now).
- Taken out for each: its files of `modules_nonmatching` (three for the
  `slot12` function, two for the `slot0f` function, which had no
  fixtures file), its lines of that folder's page (6 and 5), its alias
  in `symbols.ld`. The page's sentence that lists the functions without
  a fixtures file loses the `slot0f` function's name. A private check
  compared the page with main's minus these lines: that sentence is the
  only other difference.
- What ran: `matchbuild.py --image slot0f` and `--image slot12`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot0f_80ecx_r1`: `baseline 384 bytes at 0x800e80ec, built 384 bytes at 0x800e80ec, 0 differing instruction slots: IDENTICAL`
  - `slot12_606cx_r1`: `baseline 384 bytes at 0x8001606c, built 384 bytes at 0x8001606c, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan on the two units:
  none. The compiler's warnings on the two units: none. The records
  test: 84 run, OK. The map's render, on this tree (without batch 25):
  `overall: 5564/5600 distinct functions exact (99.4%); 13021/13072 placements (99.6%); 1778616/1809836 code bytes (98.3%)`.
  No whole build, no PC build, no differential test.
