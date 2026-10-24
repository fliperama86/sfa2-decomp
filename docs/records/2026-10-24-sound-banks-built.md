## Sound banks built at start, none from the disc (2026-10-24)

Toward no disc. Every sound request already played the arcade's effect or
nothing, and the banks' samples were zeros. The rest of the sound data
(VAB headers, command lists, SEP sequences, about 363 KB the pack kept) is
now built at start from each archive's header sector alone
(`port/arcade/sounds.c`): a VAB header the library accepts (programs from
the chunk's size, one sample the size of the body, the bank's id in the
header's id field), zero command lists and bodies, and one minimal sequence
per count the header records; each chunk starts with the word the disc
stream compares. The library C runs on them unchanged. The game never waits
on a sequence's end and plays no music from sequences, so nothing else
changes.

The request table (`sounds.py`, `sounds_table.c`) is now keyed by (bank,
start), one bank per (file, slot), instead of hashes of the disc's header
and command run; a request with no cell plays nothing and prints a trace
line.

Checked on 2026-10-24: the unit tests (26), the old and new lookups equal
on all 1,654 (file, slot, start), `--arcade-check` status 0, a pack made
again and its start, a Zangief fight from it with the same sequence of
arcade codes as before the change (397 requests: 390 effects, 7 silent),
and a Gouki run from the disc (223 effects, 20 silent, none without a cell).
