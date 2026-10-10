# Native replay: the harness

`harness.c` is the generic part of a program that runs ONE function of the
game's C, as compiled for the PC, on described cases. `port/tools/hostreplay.py`
builds the program and runs cases in it. `port/tools/test_hostreplay.py` tests
both with made-up functions; it needs the cross compiler and nothing private.

This is the first half of the native replay of fixtures. The second half,
reading a `FUNC.fixtures.json` and comparing, is `port/tools/hostfixtures.py`
(see "Replaying fixtures" below); its tests are `port/tools/test_hostfixtures.py`.

## How a program is made

`hostreplay.build_program(source, name, config, build, cc, timeout)` compiles
`source`, which defines the function `name`, with `hostbuild`'s flags and rename
(the definition is `impl_NAME`, the references are plain names). Every name the
object refers to that the symbol file or the functions of the configuration give
an address becomes `ps1_NAME` at that address, the references are renamed to
match, and the link uses `hostbuild`'s link flags so the PS1's addresses are
free. A name the object defines and refers to, with no address, is the host's
copy (as in `hostbuild`). A name with no address that nothing in the link
provides is an error that names it. The program is `BUILD/NAME.exe`; it is not
built again when it is newer than the source, the files the source includes, the
configuration, the symbol file, the harness, the driver and `hostbuild.py`.
The files the source includes are what the compiler lists for it. When that
listing fails or does not end (a header that was deleted makes it fail), what
the source depends on is not known, and the program is built whatever is on
disk; the build then says what is wrong. An input with the very time of the
program is not older than it, and the program is built again. A program is
also what it was made from: the build leaves a note beside it that names the
source file, the configuration and the compiler, and a program without that
note, or with another, is built whatever the files' times; a build that fails
takes the note away. The configuration, the symbol file and the fields file are
read once for each state of the three (path, time and size), not once for each
program: reading them costs more than building one function.

The program is a Windows program, like the port. Everything that knows the
system (the mapping of memory, the pages without execute permission, the
handler for faults, the call and the return from a fault) is in the last part of
`harness.c`; the rest is generic.

## What the program does for a case

1. Fills RAM (0x80000000 to 0x801fffff) and the scratchpad page (0x1f800000 to
   0x1f800fff) with 0xA5, then applies `memory` in order.
2. Takes a copy of both regions. Maps both readable and writable without execute
   permission, except the pages that hold a stub. At each stub's address it
   writes a five-byte call into a common entry.
3. Zeroes the program's own stack below the call, 64 KB of it (the console's
   test starts a case with a zeroed stack, and a local the function never stores
   reads 0 there). Calls the function with eight words (`args`, missing ones 0), as a C call
   (cdecl; a function that takes fewer ignores the rest).
4. Catches without ending the process: a fault of execution (the function
   called an address that has no stub), a fault of a read or a write, and any
   other fault (division by zero, stack overflow). After one, the next case runs
   as if nothing had happened.
5. Writes the result: how the call ended, the stand-ins' log, and every byte of
   the two regions that differs from the copy of step 2, except the stubs' own
   five bytes.

A case is a dictionary:

- `memory`: a list of `("fill", address, length, byte)` and
  `("bytes", address, bytes)`.
- `args`: up to eight words.
- `stubs`: a list of `{at, arguments, masks, pointees, returns, stores, counts,
  ends_at}`, the stand-in for the function at `at`. Called, it logs its first
  `arguments` words (each and-ed with its mask, if `masks` has one for its
  index), the words behind each pointer argument named in `pointees`
  ({index: words}) and every watched block; then adds 1 to each word of
  `counts`; then, at its n-th call, stores the word of each `stores` entry
  `(n, address, value)`; then, when this is call `ends_at` (0: never), ends the
  run; else returns the next value of `returns` (the last again when they are
  used up, 0 when there are none).
- `watch`: a list of `(address, words)`.

A result is `{end, calls, changes}` plus `result` (a returned word), `fault`
(`{kind, address, pc}`) or `status` (a death):

- `end`: `returned`, `stub` (ended by `ends_at`), `fault`, `died` (the process
  ended without finishing the case; `status` is its exit status) or `timeout`
  (no answer within `timeout` seconds; the process is stopped by its PID). After
  `died` or `timeout` the remaining cases run in a new process. These two have
  no calls and no changes.
- `fault.kind`: `exec` (address and pc are the address called), `read`,
  `write` (address and the address of the instruction), `other` (address is the
  exception code, for example 0xc0000094 or 0xc00000fd), or `calls` (the
  stand-ins were called more than 100000 times in one case).
- `calls`: `{at, args, stack_args, pointees {index: bytes}, watched [bytes]}`.
  An argument that is an address of the program's own stack, between the
  stack pointer at the call and the stack's base, is logged as 0 and its index is
  in `stack_args`; a pointee behind it is still read. A pointee that cannot be
  read is a fault of the case, and then that call is not logged.
- `changes`: `{address: byte}`, the byte after the call.

## The text formats

The program reads cases on standard input and writes results on standard
output. Numbers are hexadecimal without a prefix, except in a result the count of
arguments of `call`, the index of `stack` and `pointee`, and the length of
`change`, which are decimal. A line
that is not understood ends the program with a line `error TEXT` and status 2.

Input, one directive a line; the lines after `stub` that name a stub belong to
the last stub:

    case
    fill ADDRESS LENGTH BYTE
    bytes ADDRESS HEXDIGITS
    args WORD ...
    stub AT ARGUMENTS ENDS_AT
    mask INDEX MASK
    pointee INDEX WORDS
    return VALUE
    store N ADDRESS VALUE
    count ADDRESS
    watch ADDRESS WORDS
    go

Output, one block per case:

    begin
    end returned|stub|fault
    result WORD                        (returned only)
    fault KIND ADDRESS PC              (fault only)
    call AT NARGS ARG ...              (one per call, in order)
    stack INDEX                        (belong to the last call)
    pointee INDEX HEXBYTES
    watched HEXBYTES
    change ADDRESS LENGTH HEXBYTES     (each maximal run of changed bytes; RAM first)
    done

`hostreplay.format_cases`, `parse_cases`, `format_result` and `parse_results`
write and read these; the test file has the round trips.

## Where this differs from `CallLog.replace`

`ps1/src/slot06_nonmatching/contracts.py` does the same on the console's side.
The differences:

- The stand-in is C. A call is a C call: all arguments are words on the stack,
  where the console passes four in registers; the result is the word in `eax`.
- A mask is any 32 bits here, at most 16 there. A mask of 0 for the address of
  a local is not needed: such an argument is logged as 0 with a mark.
- There is no `result` (use `returns` with one value), no `tail`, and no log
  with a room of its own: the log is the program's, and the case ends as a fault
  of kind `calls` after 100000 calls.
- `ends_at` ends the case with `end` `stub` here; there the contract's setup
  says the function does not return.
- A pointee or a watched block need not be word aligned here; stores and counts
  must be.
- What memory holds that a case does not state is 0xA5 here. How the console's
  state is filled is its own.
- A pointer that cannot be read is a fault of the case here; there it is
  undefined.
- The PC function runs with the PC's stack. The address of a local means
  nothing in either code, which is why it is logged as 0.

## Replaying fixtures

`port/tools/hostfixtures.py` takes the published `FUNC.fixtures.json` files
(format 1, as `ps1/src/slot06_nonmatching/difftest.py --record` writes them),
builds each function's C as the port's compiler builds it with
`hostreplay.build_program`, runs the fixtures with `hostreplay.run_cases` and says
per function whether the PC build does what the original did on the recorded
inputs. It needs the cross compiler and the published tree and nothing private; it
does not import `difftest.py`, whose header is the definition of the format.
A failure here is a finding about the port, not about the fixture.

    python port/tools/hostfixtures.py --config ps1/src/build.toml --cc CC [--build DIR] [--jobs N] [--folder DIR ...] [--overrides DIR] [--timeout SECONDS] [FUNC ...]

Without `--folder`: the `*_nonmatching` folders beside the configuration except
`library_nonmatching` (the PC build does not compile Sony's library C), and
`port/overrides`. Without `FUNC`: every function that has a fixtures file in them.
A building of one function takes about a second and a run well under one;
`--jobs N` (default 4) does N functions at a time and the lines come in the order
of the names. Each function is built in its own folder of `--build` (a temporary
folder without it).

**A file.** A file that is not JSON, has another format than 1 or 2, is for another
function, has no fixtures, or has a fixture with a missing or malformed field is
refused with the file's name on standard error and status 2, before anything is
built. A file of format 2 (calls of original callees) is not replayed: its function
gets the line `not replayed: its fixtures hold calls of original callees (format 2)`
and is counted apart. A format 1 file with a call marked `original` is refused.

**A fixture becomes a case** of the harness. Memory: the bytes of `reads`, over them
those of `same`, and each byte of `writes` that neither names as its expected value
with every bit flipped (so a missing store shows whatever its value); every other
byte is the harness's 0xA5. There is no zeroed stack and no zeroed arena: natively
the function's stack is the program's own. The arguments are the fixture's `args`
as the first four words (a missing one 0). The fifth to eighth words are what the
fixture states as memory at 0x801ff010 and upward, 4 bytes each: that is where the
console's code finds its fifth and later arguments (above the 16 bytes of the
call's home area), and a C call takes them as arguments; a byte there that the
fixture does not state is 0xA5, as in memory. One stand-in per recorder at its `at`, with its
`arguments`, `masks`, `pointees`, `stores`, `counts` and `ends_run_at` (as
`ends_at`), and as `returns` the `returned` values of that callee's calls in order.
`watch` is the fixture's.

**What is compared**, in this order (the first lines that differ; a failed fixture
prints at most three):

1. How it ended. A fault (with its kind, address and instruction), a death or a
   timeout is one line and nothing else is compared. `ends_in_callee` true needs
   the end `stub`, false needs `returned`; a mismatch is one line and nothing else
   is compared.
2. The calls, in order: the callee (by the label of the recorder at the address
   called), their number, each argument; the first difference only. An argument
   that the harness marked as an address of the program's own stack is equal when
   the fixture's value lies in the PS1's stack region [0x801fe000, 0x801ff010) and
   a difference otherwise (`argument K is an address of the PC's stack, the
   original gave X`). The function's line counts the arguments taken as equal
   (`stack arguments not compared S`, when S is above 0; the ones counted are those
   compared before the first difference of their fixture).
3. The words behind pointer arguments and the watched blocks at each call (only
   when the calls are equal): the expected bytes are the case's own input memory
   with the fixture's changes over it, and every byte of the block is compared;
   the first differing byte is named. Behind an argument that is a stack address
   lies a local of the function: the console's test starts a case with a zeroed
   stack, so the fixture's changes there are changes against zero, and the
   expected block is zero with those changes over it, every byte compared. The
   harness zeroes the PC's stack below the call before each case for that reason
   (64 KB of it), so a local that the function never stores reads 0 on both
   sides.
4. The result, when the fixture has one and the run returned, under the width of
   the return type as the definition in `FUNC.c` states it (the text before the
   name on its definition line): `u8`, `s8`, `char` and `signed`/`unsigned char`:
   the low 8 bits; `u16`, `s16`, `short` (also `unsigned`/`signed`): the low 16;
   anything else, a pointer included: the whole word. The line says when it was
   compared in part. A definition that returns `void` while the fixture has a result
   is its own line (`the C returns nothing, the original's result was X`). A
   definition that cannot be found is `not built`.
5. The writes, exactly: every byte of `writes` has that value in the end, and no
   other byte of RAM or the scratchpad changed (the stand-ins' own five bytes
   excepted). A byte of `writes` that did not change is a `missing write` and says
   the value it was left with, a byte with another value is a `wrong value`, and a
   changed byte the fixture does not list is an `extra write`.

Not compared natively: the saved registers and the stack pointer (the
fixture's `registers`), which do not exist in the PC program.

**Known limits.** A function that reaches RAM through its mirror at the bottom
of the address space (an address with the top byte cut off, as the console's
link words hold it) cannot be replayed here yet: the PC program serves those
accesses with `port/src/mirror.c`, the harness does not have that part, and the
accesses go to the program's own low memory. One published function does this
(`func_80119694`), and its replay fails for that reason. A file of format 2 is
not replayed. Both are work to be done in the harness, not in the functions.

**Which C.** A function of a folder that has a file `FUNC.c` in the overrides folder
(`port/overrides`, or `--overrides DIR`) is replayed with the override's C, which is
what the PC program runs; the function gets a line saying so first. (`hostbuild`
refuses an override of a nonmatching function, so the real tree has no such
function; the rule is tested on made-up ones.) A function of the overrides folder
itself is replayed from its own file.

**The lines.**

    FUNC native replay: fixtures K, passed P, failed F[, stack arguments not compared S]
      fixture N (case C, NOTE):         one per failed fixture (N is the place in the file)
        up to three lines of what differed first
    FUNC native replay: not built: FIRST ERROR LINE
    FUNC native replay: not replayed: its fixtures hold calls of original callees (format 2)
    native replay: functions replayed R, all passed A, with a failure F, not built B, not replayed N

`not built` is also the line for a C file that cannot be read, a definition whose
return type cannot be read, a program for which the harness gave fewer results than
cases, and cases the harness refuses; B counts them all. R is the functions that
were run (A plus F). Status: 0 when nothing failed and everything was built (a
format 2 function does not change it), 1 otherwise, 2 for a file that cannot be
read, a name that no folder has, a function in two folders or a missing
configuration.
