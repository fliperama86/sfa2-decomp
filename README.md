# Street Fighter Zero 2: rebuilt source code

This project writes the source code of **Street Fighter Zero 2** (released as
Street Fighter Alpha 2 outside Japan) from the game itself. The version it
works on is the Japanese PlayStation disc. Nobody outside the original
developer has that source, so it is recovered one function at a time: write
readable C, compile it with the compiler of the time, and check that the
result is the same bytes as the original game's code.

None of the game's art, music or data is in this repository. You bring your own
disc image.

## Where it stands

- **All of the game's code is rebuilt.** Every function of the main program and
  of the extra code that the disc loads for each character, stage and screen
  (about 5,600 different functions) has C source that compiles back to the
  original bytes, at the original addresses.
- **The game's data is not.** Tables, text and similar data are still copied
  from the original files when the build runs. About two fifths of the main
  program's bytes are such data.
- **The names are ours.** The original names were never published, so every
  function, field and character or stage name is inferred from what the code
  does. Most functions have one (see the second picture).
- **It runs on a PC.** A port built from this same C starts the game from your
  disc image on Windows and on Macs with Apple Silicon: title, menus, fights and
  sound. It has not been shown to play to the end, and it does not run on Linux
  yet.
- **You cannot yet clone and build in one step.** Building needs your own disc
  files and a copy of the old compiler, which are not included.

The first picture has one square per function, green where the build owns it.
The second shows how many functions, fields and data symbols have a name
instead of a placeholder.

[![Coverage map: one square per function, green where the build owns it](https://fliperama86.github.io/sfa2-decomp/completion-map.svg)](https://fliperama86.github.io/sfa2-decomp/)

[![Names given: the share of functions, fields and data symbols that have a name instead of a placeholder](https://fliperama86.github.io/sfa2-decomp/names.svg)](ps1/docs/names.md#how-many-have-a-name)

Both are redrawn on every push to `main`. The
[completion map](docs/completion-map.md) explains what the numbers mean.

## Finding your way around

The source is in [`ps1/src/`](ps1/src/README.md), sorted by what it is:

- `resident/` is the main program that is always in memory, split into
  subsystems such as fighters, menus, sound and input.
- `overlays/characters/ryu/` (and one folder per character) is the code the
  game loads when that character is on screen.
- `overlays/stages/` is the same for each stage, and the other folders under
  `overlays/` are single screens such as the character select and the ending.

Each file is named after the first function in it. The
[source map](ps1/docs/source-map.tsv) lists every file's earlier name.

Everything else:

- [`ps1/`](ps1/README.md): the PlayStation work: build tools, notes on how the
  game is put together, and the source.
- [`port/`](port/README.md): the PC port (Windows, macOS on Apple Silicon).
- [`windows/`](windows/README.md): research on the Windows release, kept for
  comparison only.
- [`docs/`](docs/): plans, rules, records of changes and the
  [completion map](docs/completion-map.md).
- [`research/`](research/target-investigation.md): why this game version was
  chosen.
- [`tools/`](tools/): shared analysis scripts.

## Building and checking

The build compares what it compiles with your copy of the game, byte by byte.
It needs your own disc files, the old compiler and a few tools. The list and
the commands are in the [PlayStation notes](ps1/README.md) and the
[matching build guide](ps1/docs/matching-build.md). The first rule is: a
function counts as done only when its bytes match.

The repository's own checks need Python 3.12 and a MIPS binutils install:

```sh
python3.12 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python tools/verify_workspace.py
```

## What never goes in this repository

Game files, anything extracted from them, compiler binaries, other projects'
headers under their own license, and credentials. Those live in ignored
folders. All source written here is published.

## Working on it

Read the [project instructions](AGENTS.md), then
[current state](docs/current-state.md), [goals](docs/goals.md), the
[plan](PLAN.md), [requirements](docs/requirements.md) and
[efficient checks](docs/efficiency.md) before running anything. Decisions and lessons are in
[project memory](docs/project-memory.md); the
[AI workflow](docs/ai-workflow.md) and [agent worktrees](docs/agent-worktrees.md)
describe how automated helpers are used.
