# PS1 overlay map

Parent: [delivery plan](../../PLAN.md). Tool: `ps1/tools/pac.py`.

Everything here is static analysis of the disc files. No game was run. Link
addresses are estimates with stated evidence, not observed loads. The loader
code in the resident executable has not been read yet.

## Archive format

The 239 `*.PAC` files share one container layout:

- offset 0: little-endian u32 chunk count;
- offset 0x20: one 32-byte entry per chunk with a u32 type, a u32 size in
  bytes, and a copy of the chunk's first four bytes;
- offset 0x800 onward: chunk data in entry order, each chunk padded to a
  2,048-byte boundary.

`pac.py` rejects a file unless every entry's first-word copy matches the data
and the last chunk ends exactly at the file size. All 239 files pass.

## Code-bearing chunks

A chunk counts as code when it holds at least eight `jr ra` words. Its link
address is the base under which the most absolute call targets inside the
chunk land on function prologues inside the same chunk. Every chunk of one
type agrees on the same base.

| Link address | Chunk type | Chunks | Found in | Largest |
| --- | --- | ---: | --- | ---: |
| `0x801b0000` | `0x4` | 25 | character files, select screen | `0x1ec9c` |
| `0x801c8000` | `0x5` | 21 | second-side character files | `0x17ff4` |
| `0x801e0000` | `0xb` | 24 | character files | `0x7acc` |
| `0x801e0000` | `0x2a` | 63 | boss, demo and related files | `0x52cc` |
| `0x801e8000` | `0x6` | 20 | stage files | `0x13d1c` |
| `0x80010000` | `0x1`, `0x12`, `0x27`, `0x28` | 85 | demo, continue, select, ending files | `0x45f4c` |
| other, single use | `0x0`, `0x8`, `0xf`, `0x16`, `0x17`, `0x2b`, `0x2c` | 9 | mixed | `0x195d0` |

The resident image ends at `0x801ae900`. Types `0x4` and `0x5` sit directly
above it, `0x18000` apart, one block per player side. Each block starts with
data and holds its code further in. Types `0x16` and `0x17` have only five
agreeing call targets each, so their addresses are weak estimates.

Files without code: the remaining chunk types hold sound and image data by
their headers and sizes. That reading is unverified beyond the code test.

## Character files

- `PL##.PAC` carries the first side's block (type `0x4`). `PL##X.PAC` carries
  the same code linked for the second side (type `0x5`).
- `PL15`, `PL16` and `PL17` have no `X` twin. Each holds both sides' sound
  chunks and a single type `0x4` block. The resident reset routine special-cases
  three pairings of the two players' character numbers, which fits one shared
  file per pairing. This link between pairing and file is an inference.

## What the resident code calls

The matched `swap_kind` routine calls three addresses above the resident image.
They resolve to one routine in three placements:

| Called address | Placement |
| --- | --- |
| `0x801b5ab8` | first-side block of the two character files that the routine swaps between |
| `0x801cdab8` | the same routine in their second-side blocks |
| `0x801b5a34` | the same routine in the shared block of `PL17` |

The first instructions are identical in all three. In the two ordinary
character files, `0x801b5a34` falls in the middle of another function, so that
call is only valid when the shared file is loaded.

## Open

- Read the loader in the resident executable to confirm chunk types and
  addresses from code rather than from call-target agreement.
- Decide which overlay blocks enter the matching build, and how a build
  expresses one source linked at two addresses for the two sides.
- The four stand-alone `PS-X EXE` files under `PAC/` load at `0x80010000` and
  are not analysed.
