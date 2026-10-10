## The harness that runs one function of the game as compiled for the PC (2026-10-10)

What was added: `port/replay/harness.c`, `port/tools/hostreplay.py`,
`port/tools/test_hostreplay.py` and `port/replay/README.md`. Together
they build a small Windows program around ONE function of the game's C,
compiled by the port's compiler with `hostbuild.py`'s flags and rename,
and run it on described cases: memory put at the PS1's addresses (RAM
and the scratchpad page, 0xA5 where a case says nothing), eight
argument words, stand-ins at the addresses of the functions it calls,
watched blocks. The program reports how the call ended, what it
returned, the calls it made with their arguments, and every byte of
the two regions it changed. It needs the cross compiler and nothing
private: no game image, no PS1 compiler. `hostbuild.py` is used as a
module and is unchanged.

How it keeps to the function: the two regions are mapped without
execute permission except the pages that hold a stand-in, so a call of
an address that has no stand-in is a caught fault that names the
address. A read or a write outside the regions, a division by zero and
a stack overflow are caught too, and the next case runs in the same
process. A process that ends or does not answer is reported for its
case, and the rest run in a new one.

Why: `docs/efficiency.md`, "Fixtures for nonmatching functions": the
same fixtures can test a function as compiled for the PC, with nothing
private, which is also the only unit test of what the PC program runs.
This is the first half. The second half, reading `FUNC.fixtures.json`
and comparing as `difftest.py --replay` does, is not in this change.

What ran: `python port/tools/test_hostreplay.py --cc i686-w64-mingw32-gcc`,
"all 91 tests passed" in about 11 seconds (one-off timing), on made-up
C functions with every expected value written out by hand; 29 mutants
of the driver and of the harness in bounded runs, each failing a named
test (a private script); `python port/tools/test_hostbuild.py --cc ...`,
"all cases behaved as required". The review command passes the compiler
to the new test file as it does to the port's other tests
(`tools/ai_workflow/review.py`, with its test).

Limits, stated: the program is a Windows program like the port; a page
that holds a stand-in is executable as a whole, so a jump to another
address of that page is not caught; an argument that is an address of
the program's own stack is logged as 0 with a mark, since its value
means nothing; the stand-ins are capped at 100,000 calls a case.

Not done and not shown: no function of the game was run in it; no
fixtures file is read; nothing is compared with expected values beyond
the tests.
