## The console's I/O page is served (2026-10-10)

- What: the page `0x1f801000` to `0x1f801fff` is closed at start in every
  build (`port_iopage_init` in the new `port/src/iopage.c`, called from
  `main.c` after the memory is mapped). An access of the game's thread,
  from the game's own compiled code, wholly inside the page, faults and is
  served by the handler of `port/src/mirror.c` on a temporary value, never
  on the page. Three addresses have models, each a plain 32-bit cell that
  reads back what was last written (0 at start): `0x1f801014`,
  `0x1f8010f0`, `0x1f8010f4`. Every other access to the page ends the
  program with `stop: crash: the game used the PS1's I/O page at 0xADDRESS
  (read|write) in NAME REASON: BYTES`. A write is checked before the
  instruction is carried out. With `--trace`, one line at exit with the
  faults served and the reads and writes of each cell.
- Why: the page lay inside the 64 KB that `memory.c` maps for the
  scratchpad, so the game's sound library could read and write the chip's
  registers there and nobody noticed. The sound chip and DMA come in later
  changes; this one only makes the access seen and models the three
  registers that the library writes when it starts.
- Code: `mirrorcore.c` has three pure functions, `port_mirror_decision_io`,
  `port_mirror_insn_reads` and `port_mirror_insn_writes`; `mirror.c` shares
  its entry checks, decoder, executor and write-back with the copy of RAM
  at address 0, and its lines for that range are unchanged. The native
  test of `mirrorcore.c` has cases for the three functions. Nothing under
  `ps1/` and nothing of PsyZ changed.
- Changed outside the files of the page: `port/tools/test_hostlaunch.py`
  names `iopage` in its list of runtime files (the runtime now has one more
  file that its programs link) and its trace case skips lines that begin
  `iopage:`, as it already skipped `mirror:` (the two layers write to the
  same trace file).
- Ran:
  - `python3 port/tools/test_hostiopage.py --cc CC`: 68 lines `ok`,
    `all 68 cases behaved as required`.
  - `python3 port/tools/test_hostmirror.py --cc CC`: 103 lines `ok` (100 on
    main; the three added are the native cases for the page's decision and
    the two questions about a decoded instruction), `all cases behaved as
    required`. `python3 port/tools/test_hostlaunch.py --cc CC`: 68 lines
    `ok` before and after, `all cases behaved as required`.
  - Each rule of the page has a change that fails it: 16 changes made to
    copies of the runtime's folder, each run under a time limit (a
    one-off run, the table is in the working notes and not in the
    repository). One change is not caught by any case: checking the write
    after the instruction instead of before. The stop line is the same
    either way and the program ends at once, so no run shows the order.
  - `tools/ai_workflow/tests/test_workflow.py`: 39 tests, `OK`.
  - The build without `--psyz` and the build with `--psyz DIR` linked and
    verified. Starts with the disc, 25 seconds, offscreen, one at a time
    (one-off runs with a private input, no command of the repository repeats
    them): with `--psyz DIR` and without the option, no line that begins
    `refused` or `stop`, ended by the time limit (status 124). The start with the disc
  of the build with `--sound-library --psyz DIR` now ends at the I/O page,
  `stop: crash: ... I/O page at 0x1f801daa (write) in func_8016b788 ...`,
  status 10, where it ended at `DMACallback` (status 4) before.
- Did not run: the refusal to start when the page cannot be closed (no
  case makes the system refuse the call). Nothing sounds; the chip and DMA
  are not modelled.
