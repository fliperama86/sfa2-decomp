
## Batch 17: `func_80124304` exact, a stepped pointer kept apart from its copy (2026-10-11)

- `func_80124304` (928 bytes, resident), from `resident_nonmatching`.
  The tree is made from main at `5fcfeab`.
- The residue, named by every pass since 2026-10-10: before its second
  call of `func_801246cc` the original steps the pad record pointer in
  its saved register and then copies it to the second argument register
  (`addiu s0,s0,3`, and `move a1,s0` in the call's delay slot). The
  build made one instruction of the two (`addiu a1,s0,3`) and was one
  instruction short; the other lines that differed were branch targets
  moved by those 4 bytes.
- The reading of the compiler, from the notes of the earlier passes:
  the pass that joins instructions joins the step with the copy unless
  the stepped value is still used after the copy when that pass runs.
  The original's instructions show no such use.
- What closed it: a use that the same pass then removes. The first test
  behind the two calls reads `game_state.field_17`; with a term ORed to
  it that is 0 for every value of the pointer, `(((u32)t << 24) &
  0x100)`, the step and the copy stay two instructions, and the term
  leaves no instruction (the pass reduces it to 0 after it has left the
  step alone). Read under the guide's "an operation that has no effect
  and no instruction in the original", with the general label (choice
  of registers) and a comment of its own. Without the term, on the
  final text: 924 bytes, 5 differing instruction slots.
- Where the term may stand was measured before the unit was written
  (private scoring tool, the candidate's own form): in a statement of
  another block (inside the `if`) it stays as instructions; added to an
  address it turns the later accesses register-relative; assigned to
  the function's zero local it swaps two saved registers (6 lines); as
  `(u32)t & 0` it is folded early and has no effect. In the test of the
  same block it is exact, as an OR and as the compared value, with a
  shift of 24 and of 16 (the last two also built in the unit:
  identical). The OR form is published: it reads as "or nothing".
- A second stand-in was in the candidate already: the local `blank`
  holds the constant 0 that the loop stores, under the guide's comment
  for a local that holds a constant. Without it (0 written in the
  loop): 916 bytes, 14 slots.
- For the PC program: the term is 0 for every value, with an unsigned
  shift, so the test is the plain one on any compiler.
- Against the candidate, to stand on the published headers: the input
  block is taken through a cast of the declared array, and
  `func_801246a4` gets its first argument through a cast to the
  parameter type of its definition. Same instructions. The unit's
  comment and its declarations are the nonmatching file's (one
  prototype that `protos.h` has was dropped).
- How it went: a worker reported that the cast of lever 41 was no help
  here and that the 14 differing lines were one difference plus branch
  targets. The top level then tried the reducible term of the notes of
  two twin functions at 27 zero stores and in the test; the test was
  exact at the first try. The form was sent to the worker on the
  sibling `func_801b3f38_slot04_sel`, which has the same residue.
- Taken out: the function's three files of `resident_nonmatching`, its
  6 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `s124304x_r1`: `baseline 928 bytes at 0x80124304, built 928 bytes at 0x80124304, 0 differing instruction slots: IDENTICAL`
  The figures of the unit's variants: the same command on the unit with
  that change, the unit put back. The byte-offset scan: none. The
  private check of resident functions against the declared tables that
  hold them: 0 differ. The compiler's warnings on the unit: none. The
  map's render:
  `overall: 5554/5600 distinct functions exact (99.2%); 13009/13072 placements (99.5%); 1775936/1809836 code bytes (98.1%)`.
  No whole build, no PC build, no differential test.
