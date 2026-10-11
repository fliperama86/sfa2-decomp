## func_801189c4, the game's main, exact (2026-10-11)

- `func_801189c4` (844 bytes, resident; inferred: the game's main, the
  start-up and the frame loop): unit `d189c4_r1`, `baseline 844 bytes
  at 0x801189c4, built 844 bytes at 0x801189c4, 0 differing instruction
  slots: IDENTICAL`. No wrapper, no helper, no stand-in. "Slots" are
  instruction slots by `fndiff.py --rebuild` on the unit, each variant
  built in the tree and taken out again.
- The form: one function with two labels, `session` and `display`,
  reached by `goto` from the frame loop (the soft reset rebuilds the
  display, the interrupt's flag starts the session again), where the
  published readable C had a helper and nested loops. The original
  jumps between those two places; the readable C's header said so.
- Three declarations decide the rest, each measured:
  - The interrupt's counter `data_801ac310` is `volatile`, as the
    readable C declared it. Without it: 5 slots.
  - `data_801ac314` is `u16` in `externs.h` now (it was `short`): main
    reads it with `lhu` at three places. With `short`: 3 slots, those
    three loads. The one other unit that names it, `y1189c4_r2`, only
    stores to it and stays IDENTICAL (696 bytes). An exact build with
    `u16` shows that this declaration is compatible with the bytes of
    both, not what the original declared.
  - `func_80118fc8` (unit `s118900_r2`) takes two parameters now, a
    pad's buffer and its number, and uses neither: main sets both
    argument registers before each of its two calls. With the calls
    written without arguments main is 20 bytes shorter (10 slots). This
    is the guide's form for a callee that is passed what it does not
    use; main is its only caller. The unit stays IDENTICAL (104 bytes).
    The readable C had recorded the callee with no argument.
- One statement stores the root counter's value into both halves of
  `data_801903b0` (`a[0] = a[1] = f();`); as two statements: 4 slots.
  The variable for the reset is set to 0 in the loop (set only at its
  declaration: 11 slots, 4 bytes more). Each has a comment.
- The function's last 28 bytes are an epilogue after the endless loop
  that nothing reaches; the compiler emits it and the unit owns it (the
  inventory already had the function at 844 bytes).
- It leaves `ps1/src/resident_nonmatching/` (three files and its six
  README lines), its line leaves `symbols.ld` (the entry code's `jal`
  now resolves to the unit), and its row of `ps1/inventory/game.tsv`
  gets its name (the sweep).
- The PC program compiles the exact unit from now on, in place of the
  readable C. One build of it from this branch: `units: 3980 compiled,
  128 of them nonmatching, 0 failed`, `functions overridden in C: 15`,
  linked and verified. One start of that program without a window, 60
  seconds, with the sound written to a file: no `stop` or `refused`
  line, no `sound output open` line, the modules load, a sound stream
  starts, and the file holds sound (peak 28006 of 32767).
- What ran besides: `fndiff.py --rebuild` on `d189c4_r1`, `y1189c4_r2`
  and `s118900_r2` (IDENTICAL each); `matchbuild.py --image resident`:
  `RESULT: PASS`; the private check of resident table entries: `546
  functions in a declared table compared, 0 differ`; the unit compiled
  with `-Wreturn-type` and `-Wuninitialized`: 0 warnings; the map's
  render on this branch, which does not hold pull request 243: `overall: 5464/5600 distinct functions exact (97.6%); 12897/13072 placements (98.7%); 1717860/1809836 code bytes (94.9%)`.
  No whole PS1 build, no differential test.
