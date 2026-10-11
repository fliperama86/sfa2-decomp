## func_801b2e8c_slot04_0e exact, with one labelled stand-in (2026-10-11)

- `func_801b2e8c_slot04_0e` (204 bytes, module `slot04_0e`, second placement
  `slot05_0e`; inferred: a character action handler that, depending on the
  object's height against its rest height and on its facing, either calls
  `func_801b2f58_slot04_0e`, calls `func_80130efc`, or advances its state,
  clears two motion words and starts a sequence): unit
  `slot04_0e_2e8cx_r1`, file `ps1/src/slot04_0e/slot04_0e_2e8cx_r1.c`,
  `baseline 204 bytes at 0x801b2e8c, built 204 bytes at 0x801b2e8c, 0
  differing instruction slots: IDENTICAL`. `matchbuild.py --image slot04_0e`
  and `--image slot05_0e`: `RESULT: PASS` each.
- One stand-in under the guide's general comment: the one 8-bit local `a`
  that holds the field `field_0b`, then the facing (1 or 2), then the
  sequence number. The plain form with a local of its own for the facing
  and one for the sequence number (the readable C that this replaces)
  differs in 9 instruction slots. The same single local as an `int`: 2
  slots; with the `u8` and without the `(s8)` cast at the comparison: 7
  slots; as `s8`: 6 slots and 8 bytes more. All by `fndiff.py --rebuild` on
  the final text, each put back. The `(s8)` cast is not a stand-in: the
  original's listing has the sign extension (`sll`, `sra`) of the facing
  before the compare against the byte field, and the value 1 or 2 is
  unchanged by it. Inferred from the compiler's rules (`combine.c` keeps
  the extension when a local has a set from a byte load), not shown by a
  run of its own.
- No shared declaration changes. The unit uses the field names of the
  table of this day (`state_3`, `vel_y`, `accel_y`); the unit was found
  with the earlier names and rewritten with `renamefields.py`.
- It leaves `ps1/src/slot04b_nonmatching/` (its three files and its six
  lines of that folder's page) and its alias line in `symbols.ld`.
- What ran: `fndiff.py --rebuild slot04_0e_2e8cx_r1` (the line above);
  `matchbuild.py --image slot04_0e` and `slot05_0e`; the byte-offset scan;
  the map's render. No whole build, no PC build, no differential test.
