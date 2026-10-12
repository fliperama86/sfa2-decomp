# Nonmatching C: the other modules

C for functions of the overlay modules that have no folder of their own
here (the stage modules are in
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md), and
[`../slot04b_nonmatching/`](../slot04b_nonmatching/README.md) holds the
character modules its page names; every other module's functions come
here) that compiles with the pinned toolchain and does not reproduce the
original's bytes. A file's name ends with the name of its image;
`../build.toml` says which file of the disc's archives each image is.
The PS1 build does not use any file of this folder; the raw bytes stay
what the matching build owns. What nonmatching means here, how the test
works and what a pass does not show are on the page of the tool's
folder, [`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a
pass is evidence for the tested inputs of a function's contract, not
equivalence.

Each function is a pair: `FUNC.c` with its contract in the header
comment, and `FUNC.py`, the contract as code. A function that is later
rebuilt exactly becomes a unit of the build and leaves this folder.

## Testing one function

A function is tested alone. When one is added or changed, run its own
five runs, from the tool's folder, and at the same time (see
[efficient checks](../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the three blocks below, in place of
its old ones or at its place in the order of names. No other function is
run again for it, and the folder is not run as a whole.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

Functions of this folder without a fixtures file, by reason:

None at present: every function of this folder has a fixtures file.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
```

## The recorded lines

The three blocks are a record, not an instruction. They are what the
commands printed for each function when it was last run. That is one run
of the whole folder on 2026-10-10, with `--all` in place of FUNC; the page keeps those lines as printed. From now on a function's lines
change only when that function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
```

The control (`--cases 2000 --control FUNC`):

```
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
