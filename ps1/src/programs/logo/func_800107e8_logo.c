/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800107e8_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets the program's memory
 * layout from a base address, reads the stills' file and starts graphics and
 * the stream. With base (a0), str_sector (a1), bin_sector (a2), vmode (a3)
 * and stereo (the fifth argument, on the stack):
 *   1. stores in the program's words (gp + offset): base at 0x8002eb00,
 *      base + 0x2d00 at 0x8002eb04, base + 0xa500 and base + 0xfe00 at
 *      0x8002eb08 and 0x8002eb0c, base + 0x15700 at 0x8002eb10,
 *      base + 0x1d500 at 0x8002eb14, base + 0x21100 at 0x8002eb18 and at
 *      0x8002eb48, base + 0x21140 at 0x8002eb4c, 0 at 0x8002eb1c, vmode at
 *      0x8002eb38 and stereo at 0x8002eb34;
 *   2. a loop that ends when its four steps all succeed: CdIntToPos(
 *      bin_sector, 0x8002ebb4), CdControl(0x15, 0x8002ebb4, 0) (a return of
 *      0 starts the loop again), CdRead(0x4a, p, 0x80) with p the pointer in
 *      the word at 0x8002eb18 (0 starts again), CdReadSync(0, res) with res a
 *      local (a return that is not 0 starts again);
 *   3. adds the pointer in the word at 0x8002eb18 to each of the 32 words
 *      behind that pointer (the word is read again at each step);
 *   4. calls func_80010928_logo(), func_80010a80_logo(str_sector) and
 *      func_80010fc4_logo().
 *
 * Contract:
 *   Arguments: a0 = base, a1 = str_sector, a2 = bin_sector, a3 = vmode, the
 *     fifth argument at sp + 0x10 = stereo. No result.
 *   Reads: the 32 words behind the pointer base + 0x21100 (the test makes
 *     them), the results of the callees.
 *   Writes: the twelve words of step 1, the same 32 words (step 3), the
 *     local res.
 *   Callees, all recorders in the test, never run: CdIntToPos(i, p) (two
 *     arguments; a fill stores four bytes of the setup's through p, the
 *     model of what the library writes), CdControl(com, param, result)
 *     (three arguments, the word behind the second logged), CdRead(n, buf,
 *     mode) (three arguments; a fill stores 128 bytes of the setup's through
 *     buf, the model of the data the library reads there), CdReadSync(mode,
 *     result) (two arguments; the local's address is masked to 0),
 *     func_80010928_logo(), func_80010a80_logo(sector) (one argument),
 *     func_80010fc4_logo(). The results of CdControl, CdRead and CdReadSync
 *     are scripted per call. The words 0x8002eb00 to 0x8002eb20, 0x8002eb34,
 *     0x8002eb38, 0x8002eb48, 0x8002eb4c and the first four words of the
 *     buffer are watched, so that their values at every call are compared:
 *     the relocation before the calls shows.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_800107e8_logo(u8 *base, int str_sector, int bin_sector, int vmode, int stereo) {
    u8 res[8];
    u32 *p;
    int i;

    data_8002eb04_logo = (u32 *)(base + 0x2d00);
    data_8002eb08_logo[0] = (u32 *)(base + 0xa500);
    data_8002eb08_logo[1] = (u32 *)(base + 0xfe00);
    data_8002eb10_logo = base + 0x15700;
    data_8002eb14_logo = (u32 *)(base + 0x1d500);
    data_8002eb00_logo = (u32 *)base;
    data_8002eb18_logo = (u32 *)(base + 0x21100);
    data_8002eb48_logo = (u32 *)(base + 0x21100);
    data_8002eb1c_logo = 0;
    data_8002eb38_logo = vmode;
    data_8002eb4c_logo = (u32 *)(base + 0x21140);
    data_8002eb34_logo = stereo;
    do {
        CdIntToPos(bin_sector, data_8002ebb4_logo);
    } while (CdControl(0x15, data_8002ebb4_logo, 0) == 0 || CdRead(0x4a, data_8002eb18_logo, 0x80) == 0 ||
             CdReadSync(0, res) != 0);
    p = data_8002eb18_logo;
    for (i = 0; i < 0x20; i++) {
        *p = *p + (u32)data_8002eb18_logo;
        p++;
    }
    func_80010928_logo();
    func_80010a80_logo(str_sector);
    func_80010fc4_logo();
}
