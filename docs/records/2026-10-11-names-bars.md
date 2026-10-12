## The count of names in the README becomes three bars (2026-10-11)

The owner asked for the count of names (pull request 307) to be easier to
see: "make it more visible, add perhaps a progress bar".

`ps1/tools/namestats.py --svg` now draws one bar per kind (functions, fields,
data symbols) in place of one line of text. A full bar is every thing of its
kind; the filled part is the percentage, and it is drawn at least two units
wide once anything has a name, so that a small count can still be seen. The
percentage and the count stand beside each bar. The printed lines and the
JSON are unchanged, and so is the workflow.

What ran: `python3 ps1/tools/test_namestats.py`, seven cases, all as
required. The case for the files now checks the widths of the three bars
against the percentages, and a new case checks a count under one percent
(drawn two wide), a kind with no name (an empty bar) and a count above a
thousand (written with a comma). Three copies of the tool, each wrong in one
place, each failed a case: the least width left out, a bar twice its
proportion, the least width given to an empty kind.

Not shown by this change: how the picture looks on the README's page. No
program that draws an SVG is installed where this was written; the picture
was checked as text and by the cases above.
