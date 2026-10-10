## A stand-alone program of the disc in the tree and in the test tool (2026-10-10)

What was added: a place and a declaration for the C of a stand-alone
program of the disc (the game starts three with `Exec`; the first is
the logo's, `PAC/LOGO.EXE`), and support for such a program in the
differential test tool, so that a function of it is tested against the
program's original code with a contract and fixtures, as a function of
the game is. And the first function of the logo's program.

- `ps1/src/programs/programs.toml` declares a program: its name (the
  image part of its functions' names, `func_ADDRESS_NAME`), the private
  file (the user's own copy, taken out of the user's disc image), the
  file's path on the disc and its SHA-256, the address its body loads
  at, the functions that have C, and `[program.symbols]`, names with
  their addresses inside this program. `programs.py` beside it reads
  the file and names every refusal; `discfile.py DISC PATH OUT` takes
  one file out of a disc image (`.bin` of raw sectors, or the `.cue`
  that names it). Both have tests on made-up inputs.
- `ps1/src/slot06_nonmatching/difftest.py`: a function whose name's
  image part is a program of the declaration takes its original from
  the address in its name with the size the declaration gives; a
  case's memory is the resident executable with the program's body over
  it at the program's address (the file's hash and header are checked
  first); its C is linked with the program's names, which win over the
  tree's symbol file for this program's functions and for no other, and
  the link's placement of each such name is verified. All modes work.
  A replay needs no program file. For the functions of the game nothing
  changes: for `func_8012fd80` the comparison's line and the replay's
  line were the same from main's copy of the tool and from this one.
- `ps1/src/programs/logo/`: the folder of the logo's program with its
  page, and `func_80011758_logo`, the lookup in the program's table of
  the disc's files, with contract and fixtures. Its contract makes the
  table entry it reads, so no case reads a byte of the program's data.
- `tools/ai_workflow/review.py`: a change of a function's file under
  `ps1/src/programs/NAME/` selects that function's checks as in a
  nonmatching folder; a change of the declaration or of one of the two
  small tools selects every function of every program and the two
  tools' tests, and a change of the reader, which the test tool
  imports, the test tool's own tests as well.

Why a file of its own and not `ps1/src/build.toml`: the build
configuration knows the resident executable and the modules of the
archives; a program is a third kind of thing, and the matching build
does not build it. The folder is not a `*_nonmatching` folder, so the
PC build and the workflow tool's ledger do not see it yet. Bringing a
program into the PC program is a later change.

What ran, on the branch with main merged in:
`python -B test_difftest.py`, "all cases behaved as required", 442
cases; `python -B test_difftest_build.py --config ../build.toml`, the
same line, 11 cases; `test_programs.py`, the same line, 29 cases;
`test_discfile.py`, the same line, 19 cases;
`python tools/ai_workflow/tests/test_workflow.py`, 51 tests, OK. The
five runs of `func_80011758_logo` with `--folder ../programs/logo` and
2,000 cases: `different 0` and `discarded 0` with both seeds, 15 of 15
instruction slots executed; the control `different 2000 of 2000`; the
write audit `outside 0` with both seeds. Its record kept 12 cases and
printed no warning; its replay: `fixtures 12, passed 12, failed 0`.

Mutants, in bounded runs by a private script, counted by kind. Of the
test tool's new lines: 20, each failing a named case of group P; a
twenty-first that changes nothing passed, as it must. Of the reader:
29, of which 28 failed a named case and 1 passed (it lets a boolean
count as a number; the range and alignment checks refuse the same
inputs, so no input tells the two apart). Of the disc file tool as first written: 17,
each failing a named case. Of the review tool's new lines: 10, each
making the workflow's test file fail (that script prints the counts of
failures, not the tests' names), and one more by hand on the reader's
rule, which failed the named test of the declaration's selection. None
ended a tester without a result and none ran into a time limit.

Two checks of the publication rule, both by a private script and not
repeated by a command of this repository: with the line of the
contract that makes the table entry taken out, the record warned of 96
bytes read that the setup did not make (so the warning does cover a
program's body); and of the 96 bytes that the committed fixtures file
holds at addresses inside the program's body, none equals the
program's byte at its address.

A finding of the owner's review, corrected in this change: the disc
file tool wrote its output with a plain write, so an output path that
was the disc image itself, the cue, or a symbolic link or a second
name of either replaced the user's input with the extracted file. The
tool now makes its output with exclusive creation, which the system
refuses for any path at which anything is, so it cannot write over a
file under whatever name; before anything is read it refuses an output
that is one of its inputs, or that exists at all, with a line that
says which; and it removes nothing, also not a part left by a write
that failed. Eight new cases: the image and the cue as output, by
their own names, by a symbolic link and by a second name, with the
image given directly and through a cue; a file, a folder and two kinds
of link at the output path; a second run to the same path; a broken
image with an existing output (the refusal is about the output, so
nothing was read first); the check before the read taken out (the
creation alone refuses); a write that fails part of the way. Each
compares everything in the folder before and after, byte for byte.
Mutants of the repair, in bounded runs by a private script: 11, each
failing a named case, and a change of nothing passing.

Limits, stated: the test tool compares behaviour on a contract's
inputs, not bytes; no tool of the tree compares a program function's
bytes with the original's, so nothing is claimed about them. The
reader checks a function against the body's real size only when the
file is read; the declaration alone has no size of the body. A broken
`programs.toml` stops the tests of module functions too, since a name
with an image part is looked up in it; resident functions do not read
it. `discfile.py` takes the first file a cue names and refuses Form 2
sectors.

Not done and not shown: no other function of the program; nothing of
the program in the PC build; no run of the program anywhere.
