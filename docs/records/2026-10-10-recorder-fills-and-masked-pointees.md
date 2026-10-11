## The test tool: a stand-in callee that writes through an argument, and a pointee behind a local's address, in fixtures (2026-10-10)

What was added to the differential test tool
(`ps1/src/slot06_nonmatching/contracts.py`, `difftest.py`):

- Fills. A contract can say, as data, that a callee's stand-in writes
  given bytes through one of its four register arguments:
  `CallLog.replace(..., fills=((call, argument, offset, data), ...))`,
  at every call (0) or at the N-th. The stand-in does it after the
  call's entry is written to the log (a pointee in the entry shows the
  caller's memory as it was when the call was made), after `stores`
  and `counts`, before it returns. A fixtures file carries the fills
  in the recorder's entry and the replay rebuilds them. A recorder
  without fills builds to the same machine code as before, word for
  word. The data is the contract's own; it is not computed from the
  call, and arguments on the stack are not served.
- A pointee behind an argument that is masked to 0. A function hands a
  callee the address of one of its own locals, which the original and
  the C place differently, so the contract masks the argument's value
  out of the log; if the contract also wants the bytes behind it
  compared, the fixtures had no address to describe them against, and
  the record failed. Now the record looks at the argument's real value
  at the stand-in's entry: when the pointee's block lies wholly in the
  stack region of a case (which starts as zeros and is not input), it
  is described against zeros; anywhere else the record refuses the
  function with a line that says why. The replay compares against
  zeros and also looks at the build's real pointer: outside the stack
  the fixture fails with a line. The limit to the stack is a rule of
  publication: behind a pointer into the game's image a zero base
  would put the image's bytes whole into the file, and no warning
  would count them, because the stand-in reads them and not the
  function.
- The global pointer of a program's function is resolved once per
  function, not once per case (the owner's reviewer asked for it on
  the change that added it).

Why: a function that hands a callee a local and reads what the callee
put there could only be tested with a `tail` (machine code of the
contract's own), and a function with a tail cannot have fixtures.
Several functions of the logo's program are of that kind (a callee
turns a sector number into a position in a local; a callee hands back
two pointers through two locals).

With it: `func_80167388` (the sound library's function whose contract
had two tails, each storing fixed bytes through an argument) uses
fills and has its fixtures: the record kept 12 of 2,000 cases with no
warning, the replay `fixtures 12, passed 12, failed 0`; its five runs
print the lines its page had (as fills its two stand-ins return 0
where the tails left the caller's value; its header says so).
`func_800e0f2c_slot0f` stays without fixtures: its stand-ins return
one of their arguments and copy a string, which fills cannot say.

The native replay (`port/tools/hostfixtures.py`) reports a function
whose fixtures have a recorder with fills, or with a pointee behind an
argument masked to 0, as not replayed, with the reason in its line:
its stand-ins cannot write through an argument, and its rule for an
address of the PC's stack does not give this meaning.

What ran: `python -B test_difftest.py`, `all cases behaved as
required`, 472 cases (446 before); `python -B test_difftest_build.py
--config ../build.toml`, the same line; `python
port/tools/test_hostfixtures.py --cc CC`, `all 180 tests passed`
(171 before); `python tools/ai_workflow/tests/test_workflow.py`, OK.
The replay lines of three functions whose fixtures have `stores` or
`counts` (`func_801189c4`, `func_80151324`, `func_801545cc`) are the
same before and after. Mutants in bounded runs by a private script,
the whole list on the final tests, by kind: of the tool 76, each
failing a named case; of the native reader 15, each failing a named
case; a change of nothing passed in each group; none ended a tester
without a name, none hit a time limit. Three lines that no input
could tell from their absence were taken out instead of tested.

A finding of the owner's review, corrected in this change: the
stand-in loads the number of a call that a contract names as a 16-bit
immediate, which the processor sign-extends, and nothing refused a
larger number: a fill declared for call 65537 ran at call 1. The same
held, from before this change, for the call of a `stores` entry and
for `ends_run_at`, and a pointee of more than 8,191 words would have
wrapped its copy's end in the same way. All four are now refused
above what can be loaded (a call's number up to 32767, a pointee up
to 8,191 words), by the contract's call and, for fills, by the
fixtures' reader, each with a line that names the number. One new
case: a fill at call 32767 is accepted and does not run at calls 1
and 2; 32768, 65535, 65536, 65537 and a number beyond 32 bits are
refused for a fill, 32768 and 65537 for a store and for the run's
end, 8,192 words for a pointee, and each limit itself is accepted;
the reader refuses a fixtures file whose fill names call 32768 or
65537. Ten mutants of the four limits in bounded runs (unbounded, one
too high, one too low, the limit doubled), each failing a named case;
a change of nothing passed.

Limits, stated: fills serve the four register arguments only; a
callee whose result is one of its arguments, or that copies from one
argument to another, still needs a tail. A null argument is not a
fault in the tool's memory (address 0 is mapped): a fill through 0
writes there. A mask other than 0 on an argument with a pointee still
takes the masked value as the address, as before. `docs/efficiency.md`
and `port/replay/README.md` got the sentence each needs.
