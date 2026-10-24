## Arcade loader: a faster start instead of a cache (2026-10-23)

The owner allowed a startup cache only if its invalidation is certain.
The loader's built bytes also depend on the disc's or the pack's bytes,
so a cache would need their digest as a key. The start was made faster
instead, so that no cache is needed and nothing can go stale.

- The screen lists builder keeps each arcade tile's class (its pixels up
  to a flip) from one build to the next, between
  `arcade_screen_lists_begin` and `_end`. Its unit test builds every
  case again with the kept classes, twice, and requires the same bytes.
- The screens' chunks and the fighters' sprite chunks are packed on all
  processors (`port/src/parallel.c`). The disc's reads stay on the main
  thread before, and the ranges are served in the same order after.
- With a pack, the loader looks up each recorded range once rather than
  for every byte, and no longer reads the pack's bytes it does not need.

Measured on 2026-10-23 on this machine (32 processors): the loader's
part of the start fell from about 2.1 seconds to 0.5 seconds with the
pack, and to 0.55 seconds with the disc. Opening the arcade set takes
another 0.2 seconds. `--arcade-check` gives status 0 and the same
numbers as before (coverage 45,450,606 bytes); a start from the pack
agrees with the pack's digests, so it serves the same bytes.
