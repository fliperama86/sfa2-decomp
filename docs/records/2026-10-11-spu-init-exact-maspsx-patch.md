## `_spu_init` exact: the build can patch its export of maspsx, and one patch does (2026-10-11)

- `_spu_init` (1464 bytes at 0x8016b788, the sound library part of the
  resident executable; the reset of the sound chip) is byte-identical:
  unit `sdk_libspu_spu_init`, file `ps1/src/sdk/libspu/spu_init.c`,
  `baseline 1464 bytes at 0x8016b788, built 1464 bytes at 0x8016b788, 0
  differing instruction slots: IDENTICAL`. The unit owns the 16 bytes of
  its message at 0x8016e574 (`rodata`).
- What stood in the way. The C was exact except for one nop. The
  original has `lhu v0,396(v1); nop; li v0,0xffff` at 0x8016bb58
  (`key_off[0] |= 0xFFFF` on the volatile register block: the value
  read is not used). The compiler prints `#nop` there and leaves the
  choice to the assembler; the pinned maspsx gives a nop after a load
  only when the next instruction reads the loaded register, or, for
  assembler versions before 2.30, loads into it. An `li` into it is
  neither. No C makes that nop without other instructions.
- The owner's decision of today, asked whether to patch the helper or
  to publish the function as nonmatching: "patch".
- The build tool. `[toolchain] maspsx_patches` in `build.toml` lists
  patch files; `matchbuild.py` applies them with `git apply` to its
  export of the pinned commit, in order, never to the checkout; a file
  that does not exist, one listed twice or one that does not apply is
  an error; the report lists each patch with its SHA-256 and the hashes
  are part of the object cache's key (so every unit was built again
  once). `ps1/docs/matching-build.md` describes it.
- The patch,
  `ps1/tools/patches/maspsx/0001-nop-before-li-into-loaded-register.patch`
  (five lines of code against maspsx commit `e85ecb37`, MIT, named in
  the file; `ps1/tools/patches/maspsx/LICENSE` is maspsx's license file
  of that commit, copied unchanged, added after the review of this pull
  request): for assembler versions before 2.30 a load followed by an
  `li` into the loaded register gets the nop. It is not offered to the
  maspsx project yet; that is a separate step for the owner to decide.
- The unit. Written by this project from the listing (the readable C
  that leaves `library_nonmatching`); the reference's `_spu_init` is of
  another version of the library. Its idle loop is the reference's
  `waste_time`, as in `spu_p1.c`, so the file carries the reference's
  license line; `ps1/src/sdk/README.md` says so. The function is defined
  under the library's name `_spu_init`, as the library's parts declare
  and call it (`s32 _spu_init(s32)`, returning 0). Its two alias lines
  leave `symbols.ld` and the unused declaration `void
  func_8016b788(int a);` leaves `protos.h`: nothing in the tree or in
  the port calls that name.
- What ran.
  - The whole build with the patched helper, before the unit was added:
    `matchbuild.py` without `--image`, every unit compiled again:
    `RESULT: PASS` for the resident executable (1809 of 1809 functions
    exact) and for each of the 77 images, in 8 minutes 23 seconds. So
    the patch changes no other unit. This is the one whole build of
    this change: a change of the toolchain, not of a unit.
  - With the unit: `matchbuild.py --image resident`: `RESULT: PASS`;
    `fndiff.py --rebuild sdk_libspu_spu_init` (the line above); both again
    after main at `ae125bc` (pull request 271) was merged in, with the
    same lines.
  - `test_matchbuild.py --only unit-maspsx-patch`: seven new cases, one
    second together, `all cases behaved as required`: a patch is
    applied to the export and the checkout stays as it was; no patch
    gives the commit's text; a patch that does not apply is an error
    that names it; two patches apply in the listed order and fail in
    the other; the list's reading (eight inputs); the cache key changes
    with a patch's content and not with its name; the configuration's
    own patch applies to the pinned commit and gives one nop for an
    `li` into the loaded register with assembler 2.21, none for another
    register, none with assembler 2.34, none for an `addu` that only
    writes the register. Three mutants of the patch (the rule for every
    assembler version; also for `addu`; for any register) each fail
    that last case.
  - `test_matchbuild.py --only maspsx` (the seven and the two older
    controls of the pinned commit): `all cases behaved as required`;
    `--only cache-key` (the two controls of the key's inputs, one of
    which lists every input and now has the patches): see the pull
    request.
  - The map's render: `overall: 5510/5600 distinct functions exact (98.4%); 12956/13072 placements (99.1%); 1750316/1809836 code bytes (96.7%)` (with main at `f9c69b1` merged in, which has batch 6; it was `5509/5600`, `12952/13072`, `1749380/1809836` on main at `7a1d6ab` and at `f4a543b`).
  No PC build, no differential test.
