/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and instruction
 * order (988 bytes against 1008). The PS1 build keeps the original
 * bytes of the resident executable for it and does not use this file. The
 * differential test next to it (difftest.py with func_80134234.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): queues the pieces of a
 * screen for drawing. It clears data_80188d04, and returns at once, doing
 * nothing else, when data_8019016c is not 0, when data_801a6938 is 0, when
 * the first byte of data_801ac6a8 is 0, or when data_8018db10 is not 0.
 * Otherwise, with sel the low 16 bits of data_801a27d0 as a signed number
 * (a buffer selector) and base = data_801ac6a8, it hands a series of
 * blocks to func_8015bf34 (a library call, first argument a pointer into
 * the block at data_801987c8, second the address of a record), in this
 * order: six records of base at 0xc4, 0x124, 0xe4, 0x144, 0x104, 0x164
 * (each plus 16 * sel) with the pointer data_801987c8 + 0x20; two at 4 and
 * 0x24 with data_801987c8 + 0x38; the seven calls of func_801347b4 with
 * the table of words data_801ac86c to data_801ac884 and constants; two
 * records at 0x44 and 0x64 with data_801987c8 + 0x3c; the calls of
 * func_80153154 and func_80153f88; then, with data_801987c8 + 0x38, two
 * records of data_80188d6c at 56 * sel, data_8019808e records of strips
 * from row sel, data_80198422 records of data_80190014 from row sel, two
 * records of strips2 at 56 * sel when data_80198098 is not 0, two records
 * of data_80188e4c at 56 * sel when data_8019842c is not 0; and at the
 * end func_80135c88.
 *
 * Contract (the roles named for the fields are inferred):
 *   No argument, no return value.
 *   Reads: the flags and counters named above, data_801987c8 (a pointer
 *     only added to, never read through), the words data_801ac86c to
 *     data_801ac884 (only passed on), data_801a27d0; all addresses of
 *     records are computed, none is read.
 *   Writes: data_80188d04 (halfword, set to 0).
 *   Calls, replaced by recorders in the test, both runs alike, each
 *     returning 0: func_8015bf34 (2 arguments), func_801347b4 (6, the last
 *     two on the stack), func_80153154 and func_80153f88 and func_80135c88
 *     (none). What the callees do is outside the test.
 *   Watched by the test at every call: the word holding data_80188d04. No
 *     pointer given to a callee points at memory this function writes.
 *   Excluded: none. The selector may be any 32-bit value; only its low
 *     16 bits count, as a signed number, and the addresses made from it
 *     are passed on without being read, so all stay harmless.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations that the published headers lack (inferred types, not
   original declarations). */
extern u8 data_8019016c;
extern u16 data_80188d04;
extern u8 data_8019808e;
extern u8 data_80198422;
extern u8 data_801ac6a8[];
extern u32 data_801ac86c;
extern u32 data_801ac874;
extern u32 data_801ac87c;
extern u32 data_801ac880;
extern u32 data_801ac884;
void func_80153154(void);
void func_80153f88(void);
void func_80135c88(void);

void func_80134234(void) {
    int obj = (int)data_801987c8;
    u8 *base = data_801ac6a8;
    u8 *rec = (u8 *)data_80188d6c;
    s16 sel;
    Strip1c *strip;
    int i;

    data_80188d04 = 0;
    if (data_8019016c != 0) return;
    if (data_801a6938 == 0) return;
    if (base[0] == 0) return;
    if (data_8018db10 != 0) return;

    sel = data_801a27d0;
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0xc4]);
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0x124]);
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0xe4]);
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0x144]);
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0x104]);
    func_8015bf34(obj + 0x20, (Cmd *)&base[sel * 16 + 0x164]);
    func_8015bf34(obj + 0x38, (Cmd *)&base[sel * 16 + 0x4]);
    func_8015bf34(obj + 0x38, (Cmd *)&base[sel * 16 + 0x24]);
    func_801347b4(0, data_801ac86c, 0xea, 0x20, 0xe, 0xd);
    func_801347b4(1, data_801ac874, 0x1a, 0x20, 0xe, 0xd);
    func_801347b4(2, data_801ac870, 0xd8, 0xe2, 8, 0);
    func_801347b4(3, data_801ac878, 0x18, 0xe2, 8, 0);
    func_801347b4(4, data_801ac87c, 0x88, -0x11, 0x11, 0x10);
    func_801347b4(5, data_801ac880, 0xd9, -0x11, 0x11, 0xb);
    func_801347b4(6, data_801ac884, 0x89, 1, 0x11, 0x19);
    func_8015bf34(obj + 0x3c, (Cmd *)&base[sel * 16 + 0x44]);
    func_8015bf34(obj + 0x3c, (Cmd *)&base[sel * 16 + 0x64]);
    func_80153154();
    func_80153f88();
    func_8015bf34(obj + 0x38, (Cmd *)(rec + sel * 56));
    func_8015bf34(obj + 0x38, (Cmd *)(rec + sel * 56 + 0x1c));
    strip = strips[sel];
    for (i = 0; i < data_8019808e; i++) {
        func_8015bf34(obj + 0x38, (Cmd *)&strip[i]);
    }
    strip = data_80190014[sel];
    for (i = 0; i < data_80198422; i++) {
        func_8015bf34(obj + 0x38, (Cmd *)&strip[i]);
    }
    if (data_80198098 != 0) {
        func_8015bf34(obj + 0x38, (Cmd *)((u8 *)strips2 + sel * 56));
        func_8015bf34(obj + 0x38, (Cmd *)((u8 *)strips2 + sel * 56 + 0x1c));
    }
    if (data_8019842c != 0) {
        func_8015bf34(obj + 0x38, (Cmd *)((u8 *)data_80188e4c + sel * 56));
        func_8015bf34(obj + 0x38, (Cmd *)((u8 *)data_80188e4c + sel * 56 + 0x1c));
    }
    func_80135c88();
}
