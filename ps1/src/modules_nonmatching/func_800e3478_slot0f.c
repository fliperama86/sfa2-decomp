/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in how it addresses the
 * option block and in instruction order. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (func_800e3478_slot0f.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): turns the saved option
 * block data_8016e685 into the selections of a menu page. It copies
 * option bytes into the array data_800f8554_slot0f (index 0 from field_00;
 * index 1 the position of field_02 among five bytes of data_800df32c_slot0f,
 * or 4 when the byte data_8016e688 is not zero, otherwise left as it was
 * (also when field_02 is not among the five bytes);
 * indices 8, 2, 3, 4, 5 from field_01, 04, 05, 07, 06), calls func_80120374
 * with field_01 as a signed byte, copies field_0a and field_0b into indices
 * 6 and 7, and copies 16 bytes of table_8016e664 into
 * data_800f8564_slot0f. It then sets byte 0xb of three runs of cells of
 * data_800e953c_slot0f (8 cells up to index 0, 4 cells from cell 8 up to
 * index 3, 8 cells from cell 12 up to index 4) to 0x16 for positions 0 up
 * to and including the selection (signed byte), counted from the start of
 * each run, and 0x1b past it, and sets six pointers
 * (data_800e9688_slot0f and five more) into tables of 12 and 8 byte
 * records, indexed by the selections 1, 2, 6, 7, 5 and 8.
 *
 * Contract (what the code reads and writes; the roles named are inferred):
 *   No arguments, no return value.
 *   Reads: data_8016e685 (bytes 0, 1, 2, 4, 5, 6, 7, 0xa, 0xb),
 *     data_8016e688 (byte 3 of the same block), data_800df32c_slot0f
 *     (5 bytes), table_8016e664 (16 bytes), and
 *     data_800f8554_slot0f[1] when it stays.
 *   Writes: data_800f8554_slot0f (indices 0 to 8), data_800f8564_slot0f
 *     (16 bytes), byte 0xb of cells 0 to 19 of data_800e953c_slot0f, and
 *     the six pointer words data_800e9688_slot0f, data_800e9698_slot0f,
 *     data_800e96a8_slot0f, data_800e96b8_slot0f, data_800e96c8_slot0f,
 *     data_800e96d8_slot0f. The pointers are computed, never dereferenced.
 *   Callee: func_80120374 (1 argument) is replaced by a recorder returning
 *     0: it reaches the sound routines. The log copies at every call
 *     data_800f8554_slot0f and data_800f8564_slot0f (8 words), the cells
 *     (80 words) and the six pointer words (21 words), so a store that
 *     moves across the call is a difference.
 *   Aliasing: data_8016e688 is byte 3 of the option block data_8016e685
 *     (the setup writes it after filling the block); the others are
 *     separate globals.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRecf32c data_800df32c_slot0f;
extern s8 data_800f8554_slot0f[];
extern u8 data_800f8564_slot0f[2][8];
extern Cell16 data_800e953c_slot0f[];
extern Slot0fRec94b0 data_800e94b0_slot0f[];
extern Slot0fRec94ec data_800e94ec_slot0f[];
extern Slot0fRec94ec data_800e950c_slot0f[];
extern Slot0fRec94ec data_800e951c_slot0f[];
extern Slot0fRec94ec data_800e952c_slot0f[];
extern Slot0fRec94b0 *data_800e9688_slot0f;
extern Slot0fRec94ec *data_800e9698_slot0f;
extern Slot0fRec94ec *data_800e96a8_slot0f;
extern Slot0fRec94ec *data_800e96b8_slot0f;
extern Slot0fRec94ec *data_800e96c8_slot0f;
extern Slot0fRec94ec *data_800e96d8_slot0f;

void func_800e3478_slot0f(void) {
    int i;
    int j;
    Slot0fOpts *opts = (Slot0fOpts *)&data_8016e685;

    data_800f8554_slot0f[0] = opts->field_00;
    for (i = 0; i < 5; i++) {
        if ((s8)data_800df32c_slot0f.b[i] == opts->field_02) {
            data_800f8554_slot0f[1] = i;
            break;
        }
    }
    if (data_8016e688 != 0) {
        data_800f8554_slot0f[1] = 4;
    }
    data_800f8554_slot0f[8] = opts->field_01;
    data_800f8554_slot0f[2] = opts->field_04;
    data_800f8554_slot0f[3] = opts->field_05;
    data_800f8554_slot0f[4] = opts->field_07;
    data_800f8554_slot0f[5] = opts->field_06;
    func_80120374((s8)opts->field_01);
    data_800f8554_slot0f[6] = opts->field_0a;
    data_800f8554_slot0f[7] = opts->field_0b;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            data_800f8564_slot0f[i][j] = table_8016e664[i][j];
        }
    }
    for (i = 0; i < 8; i++) {
        if (i <= data_800f8554_slot0f[0]) {
            data_800e953c_slot0f[i].field_0b = 0x16;
        } else {
            data_800e953c_slot0f[i].field_0b = 0x1b;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i <= data_800f8554_slot0f[3]) {
            data_800e953c_slot0f[8 + i].field_0b = 0x16;
        } else {
            data_800e953c_slot0f[8 + i].field_0b = 0x1b;
        }
    }
    for (i = 0; i < 8; i++) {
        if (i <= data_800f8554_slot0f[4]) {
            data_800e953c_slot0f[12 + i].field_0b = 0x16;
        } else {
            data_800e953c_slot0f[12 + i].field_0b = 0x1b;
        }
    }
    data_800e9688_slot0f = &data_800e94b0_slot0f[data_800f8554_slot0f[1]];
    data_800e9698_slot0f = &data_800e94ec_slot0f[data_800f8554_slot0f[2]];
    data_800e96a8_slot0f = &data_800e951c_slot0f[data_800f8554_slot0f[6]];
    data_800e96b8_slot0f = &data_800e951c_slot0f[data_800f8554_slot0f[7]];
    data_800e96c8_slot0f = &data_800e950c_slot0f[data_800f8554_slot0f[5]];
    data_800e96d8_slot0f = &data_800e952c_slot0f[data_800f8554_slot0f[8]];
}
