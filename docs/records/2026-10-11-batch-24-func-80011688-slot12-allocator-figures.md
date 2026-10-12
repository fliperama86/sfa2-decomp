## Batch 24: `func_80011688_slot12` exact, a register order read from the allocator's figures (2026-10-11)

- `func_80011688_slot12` (260 bytes, image `slot12`), from
  `modules_nonmatching`. The tree is made from main at `b28daa6`.
- The residue of every pass since 2026-10-10 (best 6 differing lines):
  the blue target and the green target of the palette step in each
  other's registers (`a3` and `t1`), nothing else.
- What closed it was read from the compiler's dumps, not searched. The
  pass that gives registers to values of more than one block takes them
  in the order of a figure: the count of uses (times its base-2
  logarithm, rounded down) over the count of instructions the value
  lives across; each then gets the lowest free register. Both counts
  are taken before the pass that joins instructions and are not made
  again. In the earlier candidate the figures of the three values
  concerned were 0.875 (green target), 0.868 (the palette pointer) and
  0.857 (blue target), so the registers `a3`, `t0`, `t1` went out in
  that order; the original needs blue target, pointer, green target.
  The reading is inferred; the figures are from the private tool that
  prints the pass's own dump.
- Two changes follow from it, each measured on the final text with that
  one change:
  - The statement that steps the red channel (`red_stepped = red + 2;`)
    stands before the test of the green channel, not behind it. The
    original has its instruction in that test's delay slot either way;
    in the source this place ends the red channel's life earlier, so
    its figure stays above the others'.
  - **The stand-in**: one statement between the first test and the two
    steps, `changed |= ((u32)changed << 24) & 0x100;`, which ORs to the
    function's result flag a term that is 0 for every value of it. It
    is instructions when the figures are taken and none afterwards. It
    lies inside the lives of the pointer and of the green and red
    targets and behind the last use of the blue target, so it lowers
    the first three figures and leaves the fourth: the order becomes
    the original's. Without it: 9 differing instruction slots. With the
    red step behind the green test: 14 slots with the statement, 6
    without (the earlier candidate). The statement behind the green
    step instead of before it: identical too; behind the red step: 6
    slots. It carries the general comment (choice of registers), read
    under the guide's "an operation that has no effect and no
    instruction in the original"; the term is the one of the units
    `s124304x_r1.c`, `slot04_sel_3f38x_r1.c` and `s152124x_r1.c`.
- A second stand-in was in the candidate already and stays: the
  do-while wrapper around the statement that takes the green target. It
  carries the comment of the order kind: without it the block's
  instructions come in another order (the current blue channel is
  taken before the two targets) and 13 slots differ. Its counted
  reference takes part in the register order as well (the green
  target's figure is 0.778 with it and would be 0.667 without, below
  the red target's 0.706), which the order comment does not say; the
  13 slots are the measure of both effects together. With neither
  stand-in: 13 slots.
- For the PC program: the term is an unsigned shift by 24 of a 32-bit
  value, masked with `0x100`, so the statement ORs 0 to `changed` on
  every compiler. The function returns and stores the same values with
  and without it.
- The unit's comment of what the function does is the nonmatching
  file's, without the contract; its declarations and names of data are
  that file's. The locals have that file's names where it has one.
- Taken out: the function's three files of `modules_nonmatching`, its
  6 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image slot12`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `slot12_1688x_r1`: `baseline 260 bytes at 0x80011688, built 260 bytes at 0x80011688, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The compiler's
  warnings on the unit: none. The records test: 84 run, OK. The map's
  render:
  `overall: 5563/5600 distinct functions exact (99.3%); 13019/13072 placements (99.6%); 1777848/1809836 code bytes (98.2%)`.
  No whole build, no PC build, no differential test.
