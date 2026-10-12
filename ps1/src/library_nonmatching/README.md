# Nonmatching C: the sound library

C for functions of the PS1 game's sound library (the Sony library part of the
resident executable, `SLPS_004.15`) that have no exact C: the functions listed
below. The PS1 build does not use any file of this folder; the raw bytes stay
what the matching build owns. What nonmatching means here, how the test
works and what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

The rest of the library's functions that are C are exact C under
`../sdk/libsnd` and `../sdk/libspu`. A function that is later rebuilt
exactly becomes a unit of the build and leaves this folder.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A file includes only `../game.h`,
`../protos.h` and `../externs.h`, and declares the library's variables and
records itself (local views, named as views in a comment), so that it
compiles in the repository's public builds. The port's PC build does not
compile this folder unless it is given the option `--sound-library`: like the
units under `../sdk/`, the library's C is left out otherwise (see the header of
`port/tools/hostbuild.py`). The function's name is its
address, as in the other nonmatching folders; the library's name for it is
in the header. The rows of `ps1/inventory/library.tsv` give the sizes, which
the tool reads (it looks a resident function up in `game.tsv` and in
`library.tsv` and wants it in exactly one).

## The functions and where they come from


No file here is adapted from the reference now: the one that was (the
volume function of the sequencer) is exact and stands in `../sdk/libsnd/vm_vol.c`.

## Testing one function

A function is tested alone. When one is added or changed, run its own
five runs, from the tool's folder (`../slot06_nonmatching/`), and at the
same time (see [efficient checks](../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the three blocks below. No other
function is run again for it, and the folder is not run as a whole.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../library_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../library_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
```

## The recorded lines

The three blocks are a record, not an instruction: what the commands
printed for each function when it was last run. As it stands that is
the run of 2026-10-10 in which the first four were added, and the runs of the
same day in which `func_80165d84` and `func_80166144` were added (both have
left the folder since: they are exact C in `../sdk/libsnd/sepinit.c` and
`../sdk/libsnd/sepopen.c`), each run
alone with the commands above (seed 1 for the test and
the audit; seed 7 gave `different 0` and `outside 0` as well, and the control
printed what is below). From now on a function's lines change only when that
function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
```

The control (`--cases 2000 --control FUNC`):

```
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
```

The third block is the write audit: in how many cases the original changed memory that the function's
setup did not make.
