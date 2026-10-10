## The movie decoder's core for the PC program (2026-10-10)

Why: by the owner's word of 2026-10-10 the logo and the attract movie
are to play in the PC program. The three programs the game starts from
the disc for them are movie players; a private study of the logo's
program read every function of it. The library the port uses in Sony's
place has only empty stand-ins for the movie decoder. This change is
the first piece: the decoder itself, with nothing calling it yet.

What was added: `port/src/mdec.c` and `mdec.h`, pure C with three entry
points (a frame's bitstream to the chip's run-level codes; the chip's
tables; run-level codes to pixels of 24 or 15 bits, macroblock after
macroblock), `port/tools/test_hostmdec.py`, and the section "The movie
decoder" of `port/README.md`. The review command gives the new test the
compiler.

The source is the public psx-spx document: its chapter on the decoder
chip for the commands, the run-level rule, the transform, the colour
conversion and the tables' order, and its chapter on the disc's file
formats for the frame's bitstream. Where the documents leave a point
open the header of `mdec.h` says what this project chose and that it is
a choice: the form of the transform, the rounding of divisions, the
integer constants of the colour conversion, the layout of a 15-bit
pixel, the quantisation tables at reset (the MPEG-1 standard's default
intra matrix with its first entry 2, the same for both tables).

Nothing is compared with a console's output; the project has none. What
was compared with what: hand-worked cases, exactly; the transform with
the mathematical inverse transform, within 2, a bound the test argues
from the documents; the colour conversion with the documents' decimals,
within 1; the bitstream reader with an encoder in the test whose table
was typed from the document a second time, as a round trip.

What ran: `python port/tools/test_hostmdec.py --cc i686-w64-mingw32-gcc`,
"96 cases", "all 96 cases behaved as required", about 8 seconds
(one-off); 83 mutants of the code in bounded runs with a private
runner, 82 failing a named case and one that changes nothing that can
be seen (a wrap to 10 bits before a mask to 10 bits);
`python tools/ai_workflow/tests/test_workflow.py`, OK.

One-off private checks, on the user's disc and on the private study,
which no command of this repository repeats: three frames of the logo's
movie (320 by 240, bitstream version 2, 4 or 5 sectors a frame) decode
with no error to 300 macroblocks each, and the picture of one, looked at
by the top level, shows the company's logo. The 64 bytes of the reset
table, in the order the chip takes them, occur twice in the logo's
program; by the study's reading of that program they stand there as the
two halves of the data that follows the chip's set-table command word,
which its library reset sends. So the table and its use for both halves
agree with what that program carries. That is a reading of the program,
not a run of it.

Not done and not shown: Sony's decoder calls on top of the core, the
stream's sectors, drawing, sound; the monochrome depths; that the
pixels are the console's.
