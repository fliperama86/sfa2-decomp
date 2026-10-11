## The wrapper as a barrier for the order of instructions; func_801254f4 exact with it (2026-10-11)

- A proposal for the owner's decision, not a rule in force until he
  says so: the matching guide gets a second use of the accepted
  `do { ... } while (0)` wrapper, with a comment of its own. The first
  use changes which register holds which value (the compiler counts a
  reference inside the wrapper twice). The second fixes the order of
  instructions: the compiler's two scheduling passes do not move an
  instruction across the wrapper's ends. Read in the compiler's source
  (`sched.c`, `sched_analyze_insn`: with a loop note in the middle of
  a block the instruction after it is made to depend on every earlier
  one and every later one on it, "we must be sure that no instructions
  are scheduled across it"), and measured in the one function below.
- The guide's new text: the paragraph in "What is allowed" with the
  fixed comment for this use (it says "order of instructions" where
  the first says "choice of registers", and "emits no instruction of
  its own" where the first says "adds no instruction", because the
  count of instructions can differ without it), and three sentences in
  "What this compiler does".
- `func_801254f4` (400 bytes, resident; inferred: it decodes a code
  word of the player's input log): unit `d254f4_r1`, `baseline 400
  bytes at 0x801254f4, built 400 bytes at 0x801254f4, 0 differing
  instruction slots: IDENTICAL`. "Lines" are instruction lines by the
  private check on the published unit.
  - The wrapper stands around the two output stores (the store of the
    code word's low byte and the store of its top nibble). The choice
    of the code word by the side stands before it, outside: changed on
    review, because the two stores are the smallest group that belongs
    together and is exact. Without the wrapper: 26 of 100 lines, 2
    instructions fewer: the load that reads the first word back stands
    before the two stores and fills a delay slot, where the original
    has it after them. Three other placements were built: around the
    choice and the two stores together it is exact too; around the
    choice and the first store: 25 lines; around the choice alone: 35.
  - What was looked for before the stand-in, each built: the read back
    as the first statement of the blocks that follow (26 lines), the
    stores moved into the two arms (26), the value taken from the local
    without the read back (20: the original does read it back), a
    `switch` on the read value (64), the two words as elements of one
    array (29 and 36, in a test copy only).
  - Besides: each arm of the chain stores its constant itself (one
    store after the chain through a local: 29 lines); the byte 0 has a
    test of its own (without it: 19). Both commented. The failure tail
    is reached by `goto` from two places, as in the published readable
    C.
- If the proposal is declined, the function goes back to nonmatching C
  and the guide's text is taken out; nothing else depends on it.
- It leaves `ps1/src/resident_nonmatching/` (three files and its six
  README lines), its line leaves `symbols.ld`, and its row of
  `ps1/inventory/game.tsv` gets its name (the sweep).
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image resident`: `RESULT: PASS`; the private check
  of resident table entries: `548 functions in a declared table
  compared, 0 differ`; the unit compiled with `-Wreturn-type` and
  `-Wuninitialized`: 0 warnings (it is `void`); the map's render:
  `overall: 5473/5600 distinct functions exact (97.7%); 12907/13072 placements (98.7%); 1722712/1809836 code bytes (95.2%)`. No whole build, no differential test.
- After the merge of main at `61db0e1` (it brought pull request 253,
  `func_8011db78`): the unit rebuilt, the same IDENTICAL line;
  `matchbuild.py --image resident`: `RESULT: PASS`; the map's render:
  `overall: 5474/5600 distinct functions exact (97.8%); 12908/13072 placements (98.7%); 1723676/1809836 code bytes (95.2%)`.
