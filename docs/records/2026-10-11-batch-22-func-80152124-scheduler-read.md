## Batch 22: `func_80152124` exact, from the scheduler's rules (2026-10-11)

- `func_80152124` (124 bytes, resident), from `resident_nonmatching`.
  The tree is made from main at `e63e132`.
- The residue of every earlier pass since 2026-10-10, after sweeps of
  statement order and of the forms of the locals: the same instructions
  as the original in another order and with one value in another
  register. The original computes the low nibble first and keeps it in
  `v0`, loads the buffer pointer and the halfword, computes the high
  nibble into `a1`, and stores the low nibble before the high one.
- What closed it was read from the compiler, not searched. Two passes
  were read in its source (`sched.c` and `local-alloc.c` of GCC 2.6.3)
  and the reading checked on this function's pass dumps. The reading,
  stated loosely and inferred, not a description the compiler's authors
  give:
  - The instruction scheduler runs before and after the registers are
    given. In its first run it moves an instruction that sets a value
    with one assignment down to directly before the value's first
    reader (a load one place earlier). An assignment to a value with
    two assignments is not moved that way. In its second run nothing is
    moved that way: it sorts by a priority (an instruction that waits
    for a load's result comes later), puts stores and loads behind
    other instructions of the same priority, and keeps the first run's
    order for the rest.
  - A value that is used in one block gets the first free register
    there. A value of more than one block gets its register in a later
    pass, which prefers the register a copy connects it with.
- The text that follows from it, each point measured on the final text
  of the unit with that one change:
  - The halfword `data_8018d208` is read into a local before the stores
    (as the three sibling functions of `s150c6c_r8.c` do). Written at
    its store: 9 differing instruction slots.
  - The low nibble is a local that is set and then shifted in place, two
    statements. As one expression: 3 slots (at the store or in the
    local).
  - The high nibble is kept in the function's second parameter, whose
    incoming value takes no part in what the function does, with a term
    ORed to it that is 0 for every value of the parameter:
    `b = (c & 0xf0) | (((u32)b << 24) & 0x100);`. The term reads the
    incoming value, so the parameter is a value of two blocks when the
    registers are given, and it leaves no instruction. With
    `b = c & 0xf0;`: 7 slots. With a local in place of the parameter and
    no term: 7 slots. With a local and the term: 14 slots and 136 bytes.
    This is the stand-in of the unit, labelled with the general comment
    (choice of registers), read under the guide's "an operation that has
    no effect and no instruction in the original"; the term has the
    spelling of the two units that carry it already
    (`s124304x_r1.c`, `slot04_sel_3f38x_r1.c`), without the conversion
    through 64 bits because the operand is an `int` here, not a pointer.
  - All three written plainly (the nonmatching file's body): 15 slots.
- Other places of the term, measured the same way: ORed into a statement
  of the low nibble (`lo |= ...;` behind the shift, and
  `b = c & 0xf0;`) the unit is identical too; the form in the unit has
  one statement less. In the function's first test: 5 slots (inferred
  reason: there the compiler removes the parameter's incoming copy
  together with the term, and the assignment is then the only one).
- The call keeps the form `func_8015bf34((int)a, cmd++)` and the store
  of the stepped pointer behind it. With `cmd` passed and `cmd + 1`
  stored: 2 slots.
- For the PC program: the term is `((u32)b << 24) & 0x100`, an unsigned
  shift of a 32-bit value whose low 24 bits are then 0, so it is 0 on
  every compiler; the caller (`x23_r1.c`) passes the second argument,
  so reading it is defined. The function stores and passes the same
  values with and without the term.
- The unit's comment of what the function does is the nonmatching
  file's, without the contract. The signature is the one `protos.h`
  declares.
- Taken out: the function's three files of `resident_nonmatching`, its
  6 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `s152124x_r1`: `baseline 124 bytes at 0x80152124, built 124 bytes at 0x80152124, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The private
  check of resident functions against the declared tables that hold
  them: 0 differ. The compiler's warnings on the unit: none. The map's
  render:
  `overall: 5561/5600 distinct functions exact (99.3%); 13017/13072 placements (99.6%); 1777448/1809836 code bytes (98.2%)`.
  No whole build, no PC build, no differential test.
- Also in this pull request, in a commit of its own: 27 records of this
  lane's batches of 2026-10-11 began with an empty line, and the records
  test (`tools/ai_workflow/tests/test_workflow.py`) refuses a record
  whose first line is not the heading: one failing test on main at
  `e63e132`. The empty line is taken out of each; nothing else in them
  changes. This record has its heading on the first line. The test:
  84 run, OK.
