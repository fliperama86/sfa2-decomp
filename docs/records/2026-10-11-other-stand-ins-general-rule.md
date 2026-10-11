## A general rule for compiler-steering stand-ins in exact units (2026-10-11)

- The owner's instruction in chat, after a look at how another project
  for the same console handles constructs that only steer the compiler:
  "loosen it up a bit for what you consider good enough, then commit and
  merge DIRECTLY in main." So this change went to main as one commit,
  without a pull request. The line it draws is the project's reading of
  his words, not his text.
- What was read: the reference checkout of the decompilation of
  Castlevania: Symphony of the Night (`xeeynamo/sotn-decomp`, commit
  `62d0326` of 2026-07-05). Its Makefile fetches a compiler package named
  `cc1-psx-26`. Its style page has one rule for such code: "A fake should
  be commented with an `// !FAKE:` above or to the side of the code that
  you think it's fake." There
  `grep -rnE '(//|/\*).*FAKE' src --include='*.c' --include='*.h' | wc -l`
  prints 35. Forms seen in its PS1 source, one file each: a `volatile`
  local that nothing uses (`src/st/sel/stream.c`), a block under `if (0)`
  (`src/st/cat/e_hellfire_beast.c`), an `if` with an empty body
  (`src/dra/5D5BC.c`), a value copied through a second local
  (`src/weapon/w_037.c`), a constant in a local (`src/weapon/w_053.c`), a
  `do { } while (0)` around statements (`src/dra/5087C.c`). Not every
  such form there carries the comment: the wrapper of `5087C.c` has none.
  Nothing of that project's code was taken.
- The guide's change (`ps1/docs/matching-guide.md`, "What is allowed"):
  the paragraph that forbade every dummy operation now forbids what is
  not plain C and what changes the function's behaviour. A new part,
  "Other stand-ins", gives one rule for a steering construct that no
  earlier paragraph names: plain C, no effect on what the function does,
  exact with it and not without it, the last resort and the plainest
  found, under a comment that begins "Stand-in, not recovered source:"
  and ends with the measured figure. It lists the forms it admits (an
  assignment that nothing reads, a local that is only a second name, an
  operation without effect, a test that the original's instructions do
  not show, `volatile` or `register` on a local) and what stays not
  allowed. The stand-ins that the guide already named keep their own
  rules and comments. A form that the rule admits needs no new paragraph
  of the guide any more.
- What it replaces: the owner's approval comment of pull request 83
  (2026-10-07) kept candidates with an assignment that nothing reads
  parked "under the current guide". The guide is changed, so such a
  candidate may now be published under the new comment.
- Two sentences of "What this compiler does" that called a measured form
  "not allowed" now call it a stand-in. One of them said that
  `func_80010840_slot27` stays parked; its published unit
  (`ps1/src/slot27/slot27_0840x_r1.c`) does not have the mask.
- What ran: `python tools/ai_workflow/tests/test_workflow.py`, for this
  record's form. No unit was changed and none was rebuilt. No parked
  candidate was tried against the new rule yet, so how many functions it
  frees is not known.
- Not done: the private briefs and search helpers of the lanes still
  state the earlier ban.
