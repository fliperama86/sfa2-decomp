# Native replay: the harness

`harness.c` is the generic part of a program that runs ONE function of the
game's C, as compiled for the PC, on described cases. `port/tools/hostreplay.py`
builds the program and runs cases in it. `port/tools/test_hostreplay.py` tests
both with made-up functions; it needs the cross compiler and nothing private.

This is the first half of the native replay of fixtures. Reading a
`FUNC.fixtures.json` and comparing as `difftest.py --replay` does is a later
package.

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
3. Calls the function with eight words (`args`, missing ones 0), as a C call
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
