## Batch 20: two functions of the library as assembly units written by hand (2026-10-11)

- `StopRCnt` (`0x80157750`, 52 bytes, `libapi/counter.c`) and
  `_card_clear` (`0x8016d8a0`, 48 bytes, `libcard/card.c`), the last two
  functions of the library's timer and memory card parts without a
  unit. With them the library has two functions left open, both of the
  sound part and far from a match.
- The decision: the owner asked what the "assembler behaviour" is that
  kept them open, how the reference project dealt with it, what the
  recommendation is, whether the two are Sony's code and whether the
  port needs them. The answers: the reference project did not meet the
  case (its game links another version of the library, where its one
  compiler gives these functions from C), and where its own build does
  not give a function's bytes it includes the original's assembly; the
  two are Sony's code; the port compiles none of the library. The
  recommendation was an assembly unit each. His answer: "ok, that sounds
  good to me."
- A correction of what he was told, after the review of the first head:
  "the port compiles none of the library" was too broad. The port's
  normal build with PsyZ compiles the C of the sound library
  (`sdk/libsnd` and `sdk/libspu`, and the files of
  `library_nonmatching`; `port/tools/hostbuild.py` selects them, and
  `port/README.md` has the decision that made sound part of the normal
  build). What holds, and what his question was about: the port
  compiles neither of these two units, with sound or without; they are
  of the timer and memory card parts. He was told of the correction in
  the session.
- Why not C: the reference's C for both is known and quoted in each
  file's comment. For `StopRCnt`, GCC 2.5.7 (the compiler of this
  file's `SetRCnt` and `StartRCnt` since batch 19) writes the
  original's instructions, with the two loads of variables as one line
  each; the original has those two lines as five instructions in an
  order and with a temporary register that the assembler step of this
  build does not produce (6 instruction words differ in the private
  trial). For `_card_clear`, the original keeps the parameter in its
  argument slot across the first call where every compiler tried keeps
  it in a saved register (52 or 56 bytes for 48); with the parameter's
  address taken, GCC 2.5.7 gives 48 bytes with 4 words off: the return
  address at another offset of the frame, and the stack's restore
  before the return where the build has it in the delay slot. Seven
  compiler versions (2.5.7 to 2.8.1) and eleven sets of options on the
  best form gave nothing nearer. All of this is from the private trial
  of this evening; none of its tools is in the repository.
- These are the first assembly units for code that the reference has
  as C. The assembly units before them are code that was assembly in
  the original: the library's generated stubs and the routines under
  `asm/`. The build page says so under "Assembly units".
- The units: `sdk/libapi/counter_p5.s` and `sdk/libcard/card.s`,
  `kind = "asm"`, in the form of the library's generated stubs (the
  instructions written out with the names of what they refer to, no
  bytes), each with a comment that gives the C, what the compilers
  give and what is inferred. They carry the reference's MIT line
  because their comment quotes its text.
- For the port: nothing changes. The PC program compiles neither of
  these two units (of the library it compiles the sound part only);
  `_card_clear` has a routine of the port since earlier, `StopRCnt` has
  none.
- Other files: `symbols.ld` loses `_card_clear`, which the unit defines
  now (`StopRCnt` had no line). The inventory carries the two names.
  The library page: a line in "What is here" and a paragraph. The build
  page: a paragraph under "Assembly units", and the correction that the
  review of pull request 298 noted (three units name an assembler
  version of their own, not one). The project memory page: the
  decision.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `sdk_libapi_counter_p5`: `baseline 52 bytes at 0x80157750, built 52 bytes at 0x80157750, 0 differing instruction slots: IDENTICAL`
  - `sdk_libcard_card`: `baseline 48 bytes at 0x8016d8a0, built 48 bytes at 0x8016d8a0, 0 differing instruction slots: IDENTICAL`
  The map's render: `overall: 5559/5600 distinct functions exact (99.3%); 13014/13072 placements (99.6%); 1776580/1809836 code bytes (98.2%)`; the two count as exact assembly there. The
  tree is made from main at `d55ff8a`. No whole build (two new units
  of one image, no change of a tool or of a shared declaration), no PC
  build, no differential test.
