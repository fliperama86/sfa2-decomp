## Batch 49: `_SsContDataEntry` exact (library, `func_80167388`) (2026-10-12)

- Function: `_SsContDataEntry` (`func_80167388`), 1504 bytes (376 instructions), image `resident`; it comes from `ps1/src/library_nonmatching/`.
- This is Sony's library code written by this project from the listing of the original: the reference only declares a function of this name and has no definition, so no reference text and no license line. The unit stands in `ps1/src/sdk/libsnd/dataentry.c` (unit `sdk_libsnd_dataentry`), modelled on `sepopen.c`.
- `ps1/src/sdk/README.md`: the paragraph on functions written by this project now names two files of their own, `libsnd/sepopen.c` and `libsnd/dataentry.c`. No count on the page changed.
- Residue: with the plain body the frame was 24 bytes smaller and one instruction was missing (1500 bytes, 34 differing slots). Closed by a local assigned under a test in case 1 of the b29 == 2 branch.
- Reading (inferred): the quotient of a signed division by a power of two leaves values that keep stack slots although the division is deleted, and the deleted branch leaves a label that makes the delay-slot pass copy an `andi` instead of moving it.
- Stand-in 1: `u = (arg2 * 100 / 8192) << 13;` under `if (arg2 > 64)`, a value nothing reads, reproduces the stack frame. Without it: 1500 bytes, 34 differing instruction slots.
- Plain C, ordinary comment: the masked value `arg2 & 0x7F` is held in a local `v` for both bytes; as one assignment to both it differs in 11 slots.
- Tried in this order: the test of case 1 as the condition (`arg2 > 64 && arg2 < 128`): 34 slots, not identical; the type of t for u with the whole value: IDENTICAL, kept.
- Also tried, not identical: the stand-in with the test `arg2 > 64 && arg2 < 128` (41 slots), `int t` (15 slots, 1508 bytes), `u16 v` (13 slots). `int u` is identical as well; `u16 u` kept as the type of t.
- The header comment's "What it does" now says that case 3 ORs a value computed from arg2 into byte 5 or byte 4 of vag (the nonmatching file said the C did not).
- Taken out of the tree: the three files `func_80167388.c`, `.fixtures.json`, `.py` of `ps1/src/library_nonmatching`, the bullet and the recorded test lines and paragraphs of that folder's README that named the function, and the line `_SsContDataEntry = 0x80167388;` of `ps1/src/symbols.ld` (the unit defines the name now; the image compare passes without it).
- For the PC program: the stand-in assigns a local that nothing reads; the port's build with the sound library compiles the units under `sdk/libsnd`, and this unit takes the place of the nonmatching file there. No PC build was run.
- What ran:
  - `fndiff --rebuild sdk_libsnd_dataentry`: `baseline 1504 bytes at 0x80167388, built 1504 bytes at 0x80167388, 0 differing instruction slots: IDENTICAL`
  - `matchbuild --image resident`: `RESULT: PASS`
- The page of `library_nonmatching` said "the four functions below"; the folder held two before this batch. The sentence now gives no count.
