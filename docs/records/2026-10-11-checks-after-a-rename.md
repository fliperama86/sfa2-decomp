## What is checked after a rename: the owner's word (2026-10-11)

After the first three naming changes the owner asked: "do we need to check
anything after renaming? I mean, that shouldn't impact anything". The
answer given: a rename that is done right changes nothing, so the one thing
worth checking is that it was done right, which means no missed use and no
name clash; the build and comparison of every image catches both in about
two minutes; for a function rename the replay of the fixtures is worth its
seconds too, because fixtures name callees; the rest can go. His answer:
"agreed".

`docs/efficiency.md` has a row for it now, below the row for shared files,
and a paragraph with the reason. The three changes before this one ran
more than that (a comparison of the compiler's output for the nonmatching
C, replays for field renames); later renames do not.

Nothing was run for this change: it is text.
