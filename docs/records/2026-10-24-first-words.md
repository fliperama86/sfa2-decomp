## Every streamed chunk starts with its header's word (2026-10-24)

The game's disc stream compares the first word of every archive chunk it
reads (tables 0 to 5, except table 0 records of kind 0xffff) with the word
its header record holds, and on a mismatch reads the file again; the error
is only cleared by that seek, so a built chunk that starts with another word
makes the game read forever (a blanked SELECTA page ran the sound slots out
and crashed). `first_words()` in `port/src/arcade.c` now checks, at every
start with `arcade_assets`, what the game reads at each chunk's start
against its header record as the game reads it; where a built chunk
differs, the record's word is made the chunk's for the game's reads (a
game-only range, as the full animation sets and the talk pages already
did), and the start is refused if any difference remains. The rule and its
evidence in the game's C are in the function's comment.

Checked on 2026-10-24: a start from the disc (1,499 chunks checked, none
differing with the day's builders), and the Zangief and Gouki sessions.
