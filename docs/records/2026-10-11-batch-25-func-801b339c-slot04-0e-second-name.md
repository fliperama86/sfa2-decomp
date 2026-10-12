## Batch 25: `func_801b339c_slot04_0e` exact, the object kept in the argument register (2026-10-11)

- `func_801b339c_slot04_0e` (180 bytes, image `slot04_0e`, and its
  placement in the image `slot05_0e`, which is built from the same
  units), from `slot04b_nonmatching`. One function for the map, two
  placements. The tree is made from main at `b28daa6`; main with batch
  24 (pull request 303, merged first) is merged in, and the builds, the
  records test and the map below ran again on that head.
- The residue: the original copies the object into the first argument
  register twice (in the delay slot of the first test, and behind the
  sound call) and its shared call of `func_801307e0` at the end has an
  empty delay slot. Every candidate with one shared call copied the
  object once, in that call's delay slot, and was one instruction
  short; the nonmatching file called `func_801307e0` in each arm and
  was longer.
- What closed it: a second name of the object, the local `p`, that is
  set behind the first call and set again behind the sound call, and
  that the shared call takes. No call lies inside its life, so the
  compiler gives it the first argument register, and its two sets are
  the original's two copies. The first set carries a term that is 0 for
  every value of the first call's result, which the local `res` holds:
  `p = (Object *)((u8 *)o + (((u32)res << 24) & 0x100));`.
  With `p = o;` there the compiler sees before the registers are given
  that `p` is the object, and builds the same code as with no second
  name; the term hides that until the instruction-joining pass and
  leaves no instruction. This is the unit's stand-in, with the general
  comment (choice of registers), read under the guide's "a local that
  is only a second name" and "an operation that has no effect and no
  instruction in the original". The reading of the compiler is
  inferred.
- Measured on the final text, each with that one change: `p = o;` at
  the first place: 176 bytes, 5 differing instruction slots. No second
  name (the calls take the object): 176 bytes, 5 slots. The second set
  taken out: 188 bytes, 19 slots. The nonmatching file's shape, a call
  in each arm: 200 bytes, 18 slots.
- The term reads an `int`, not the pointer. With the pointer in its
  place (through an unsigned integer of 64 bits, as two published units
  write it) the unit is identical too, but this compiler then warns
  ("cast from pointer to integer of different size"); with the `int`
  it does not.
- A correction that this found: the records of batch 17
  (`s124304x_r1.c`) and batch 18 (`slot04_sel_3f38x_r1.c`) say that the
  compiler gave no warning on the unit. Each of the two units gives
  that one warning, at its term on a pointer; the claim was made before
  the conversion through 64 bits came in with the review and was not
  measured again. The two units are unchanged here: their terms must
  read the pointers they read.
- For the PC program: the term is an unsigned shift by 24 of a 32-bit
  value, masked with `0x100`, so it is 0 on every compiler; it is added
  to the pointer as a count of bytes, so `p` is the object. The
  function stores, passes and returns the same values with `p = o;`.
- How it went: a worker of this lane, briefed with tonight's reading of
  the compiler's passes and a private tool that prints them, found the
  form in five minutes; two earlier notes on this function (a copy that
  the compiler deletes at the join, a call that passes one argument
  less) did not hold. The top level rebuilt it in the tree with the
  published names and measured the variants.
- The unit's comment of what the function does is the nonmatching
  file's, without the contract.
- Taken out: the function's three files of `slot04b_nonmatching`, its
  6 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image slot04_0e` and `--image slot05_0e`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot04_0e_339cx_r1`: `baseline 180 bytes at 0x801b339c, built 180 bytes at 0x801b339c, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The compiler's
  warnings on the unit: none. The records test: 84 run, OK. The map's
  render, with batch 24:
  `overall: 5564/5600 distinct functions exact (99.4%); 13021/13072 placements (99.6%); 1778208/1809836 code bytes (98.3%)`.
  No whole build, no PC build, no differential test.
