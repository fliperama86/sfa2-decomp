# Nonmatching C: character modules

C for functions of the character modules `PL0C` to `PL17` that compiles with
the pinned toolchain and does not reproduce the original's bytes. At present
there is none: the last, `func_801b1a00_slot04_14`, became exact on 2026-10-12
and stands in `../slot04_14/slot04_14_1a00x_r1.c`; the page stays for the
next one. The PS1 build does not use any file of this folder; the raw bytes stay what the
matching build owns. What nonmatching means here, how the test works and
what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function that is later rebuilt
exactly moves to the folder of its image and leaves this one.

What these commands printed on 2026-10-10, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --all

```
```

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --control --all

```
```

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --writes --all

```
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
```
