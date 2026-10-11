## SpuVmAlloc of the sound library exact (2026-10-11)

- `SpuVmAlloc` (636 bytes at 0x8015ffc0, the sound library part of the
  resident executable; the published readable C called it
  `func_8015ffc0`; inferred: it picks the voice that a new note takes):
  unit `sdk_libsnd_vm_alloc`, file `ps1/src/sdk/libsnd/vm_alloc.c`,
  `baseline 636 bytes at 0x8015ffc0, built 636 bytes at 0x8015ffc0, 0
  differing instruction slots: IDENTICAL`. No wrapper, no helper, no
  stand-in. "Lines" are instruction lines by the private check on the
  published unit.
- Where it comes from: adapted by hand from `SpuVmAlloc` of the
  reference (sotn-decomp, `src/main/psxsdk/libsnd/vmanager.c`, MIT), as
  the readable C was; the file keeps the reference's license line. It
  stands in a file of its own, not in a part of `vmanager.c`, because
  it declares its own view of this game's voice record (0x30 bytes) and
  includes the game's headers, as `sepopen.c` does; `sdk/README.md`
  says so. It defines the library's name: the two exact units that call
  it (`vmanager_p1.c`, `vmanager_p2.c`) declare `u8 SpuVmAlloc(s32
  arg0);`, and the definition agrees.
- What made it exact against the readable C, each figure measured on
  the published unit and commented at its place:
  - The second kept value is a 16-bit local; as `int`: 21 of 159
    lines, 1 instruction fewer.
  - The first loop's test reads the voice's field into a 16-bit local
    inside the condition; the plain test of the field: 22 lines, 1
    instruction fewer. The comment says the local is only there for
    the bytes. Inferred from the compiler's source, not shown by a run
    of its own: it is one more instruction in the loop, at which the
    loop pass no longer moves the load of the voice count out of it.
  - The pointer local of the readable C is gone.
- The 16-bit local and the PC program. The bytes keep one stored value
  sign-extended in its register where the type says unsigned. That
  store runs only when the value is greater than the kept one, which
  is never negative, so the value is positive and both readings agree
  on every input. Also run, on a host build: the exact C against the
  variant with an `int`, 2,002,000 cases with many ties and values
  around 0x7fff and 0x8000: 0 differences. So the unit needs no second
  C file in the port. (A note of mine had said the opposite; pull
  request 254 was opened on it and closed.)
- It leaves `ps1/src/library_nonmatching/` (three files, its six lines
  of recorded output, its entry in the list of the functions; the page
  says four functions now and names one adapted file), its two lines
  leave `symbols.ld` (the name `func_8015ffc0` and the alias
  `SpuVmAlloc`: the unit defines the latter), and the sweep gives its
  row of `ps1/inventory/library.tsv` the name.
- The PC program compiles the unit as part of the sound library. One
  build of it from this branch: `sound library: 47 units of sdk/libsnd,
  22 of sdk/libspu, 4 of library_nonmatching`, `units: 3980 compiled,
  115 of them nonmatching, 0 failed`, `functions overridden in C: 15`,
  linked and verified. One start of it without a window, 45 seconds, the
  sound written to a file: no `stop` or `refused` line, no `sound
  output open` line, the file holds sound. The rule for an exact unit
  asks for neither the build nor the start; they are not run for the
  next library units.
- What ran besides: `fndiff.py --rebuild` on the unit (the line above)
  and on its caller's unit `sdk_libsnd_vmanager_p1` (`baseline 13656
  bytes at 0x8016023c, built 13656 bytes at 0x8016023c, 0 differing
  instruction slots: IDENTICAL`); `matchbuild.py --image resident`:
  `RESULT: PASS`; the map's render:
  `overall: 5477/5600 distinct functions exact (97.8%); 12911/13072 placements (98.8%); 1725204/1809836 code bytes (95.3%)`. No whole PS1 build, no differential test.
