## Batch 23: `func_8013f3e8` exact, the ring push written once per side (2026-10-11)

- `func_8013f3e8` (140 bytes, resident), from `resident_nonmatching`.
  The tree is made from main at `e63e132`; main with batch 22 (pull
  request 301, merged first) is merged in, and the checks below ran on
  that head.
- No stand-in and no label: the unit is plain C. The stack space that
  the original reserves and never uses (16 bytes) comes with this text;
  the earlier candidates had an unused local for it.
- The residue of the earlier passes since 2026-10-10: every candidate
  took the ring through one pointer that an `if` and `else` set. The
  compiler turns that into a default and an override (the pointer for
  one side loaded before the branch), five instructions short of the
  original, which loads the ring's address in each arm and jumps to a
  shared run of instructions. A form that held the arms apart (the
  tested variable as the pointer) reached two lines, with the sum of
  the store in the other register.
- What closed it, read from the compiler (GCC 2.6.3 source and this
  function's pass dumps; the reading is inferred):
  - The unused stack space. The pass that joins instructions makes a
    sign-extending load out of a halfword load and the two shifts that
    extend it; when the plain halfword is still needed as well, it
    leaves two loads and does not clear the count of uses of the value
    between the shifts. That value has no instruction any more and
    still gets a stack slot. One such slot is 8 bytes of frame here.
    The original has 16: two of them. One read of the head into a
    signed halfword, used as the index and again for the step, gives
    one; so the read stands twice.
  - Two arms that end in the same instructions are joined by the last
    jump pass, after the registers are given. That is the original's
    shape: an address load per arm, then one run. So the read and the
    store are written once per side, on the ring's name, and the step
    and the wrap behind them once.
  - With the read and the store alone in an arm, the arm has exactly
    three values of its own (the address, the index, the stored value).
    For exactly three the pass that gives block-local registers serves
    them in the order they are born (an oddity of its three-element
    sort, read in `local-alloc.c`), so the address gets `v0`, which the
    original has.
- Measured on the final text, each with that one change:
  - the ring through one pointer chosen by a conditional, write-back
    through it (the nonmatching file's body): 84 bytes, 31 differing
    instruction slots;
  - one pointer set by `if` and `else`, the write-back per side as in
    the unit: 128 bytes, 24 slots;
  - the step `head = (head + 1) & 0xff;` moved into each arm: 124
    bytes, 20 slots;
  - the head as `u16` with a signed cast at the index: 136 bytes, 8
    slots; as `int` with a signed cast at the read: 128 bytes, 13
    slots.
- How it went: the three points above were read first; a grid of
  per-side forms scored with the build's judge then narrowed the place
  of the step (in the arm it stood 5 lines off), and the form with the
  step behind the arms was identical at the first build.
- The unit's comment of what the function does is the nonmatching
  file's, without the contract; one sentence on the index is added.
- Taken out: the function's three files of `resident_nonmatching`, its
  6 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `s13f3e8x_r1`: `baseline 140 bytes at 0x8013f3e8, built 140 bytes at 0x8013f3e8, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The private
  check of resident functions against the declared tables that hold
  them: 0 differ. The compiler's warnings on the unit: none. The
  records test (`tools/ai_workflow/tests/test_workflow.py`): 84 run,
  OK (it failed on this branch before main was merged in, as on main
  then, for the 27 older records that batch 22 corrected). The map's
  render, with batch 22:
  `overall: 5562/5600 distinct functions exact (99.3%); 13018/13072 placements (99.6%); 1777588/1809836 code bytes (98.2%)`.
  No whole build, no PC build, no differential test.
