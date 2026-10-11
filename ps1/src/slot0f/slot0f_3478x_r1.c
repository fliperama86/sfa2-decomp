/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Second names of the option block at 0x8016e664 in the views this function needs: externs.h declares data_8016e685 and data_8016e688 as bytes. */
extern Slot0fOpts opts_8016e685;
extern s8 flag_8016e688;
extern Slot0fRecf32c data_800df32c_slot0f;
extern s8 data_800f8554_slot0f[];
extern s8 data_800f8564_slot0f[2][8];
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

/*
 * What it does (inferred, not an original name): turns the saved option
 * block (opts_8016e685) into the selections of a menu page. It copies
 * option bytes into the array data_800f8554_slot0f (index 0 from field_00;
 * index 1 the position of field_02 among five bytes of data_800df32c_slot0f,
 * or 4 when the byte flag_8016e688 is not zero, otherwise left as it was;
 * indices 8, 2, 3, 4, 5 from field_01, 04, 05, 07, 06), calls func_80120374
 * with field_01 as a signed byte, copies field_0a and field_0b into indices
 * 6 and 7, and copies 16 bytes of table_8016e664 into
 * data_800f8564_slot0f. It then sets byte 0xb of three runs of cells of
 * data_800e953c_slot0f to 0x16 up to and including the selection and 0x1b
 * past it, and sets six pointers into tables of 12 and 8 byte records,
 * indexed by the selections 1, 2, 6, 7, 5 and 8.
 */
void func_800e3478_slot0f(void) {
    Slot0fRecf32c tbl = data_800df32c_slot0f;
    int i;
    int j;
    s8 *q;
    data_800f8554_slot0f[0] = opts_8016e685.field_00;
    for (i = 0; i < 5; i++) {
        if ((s8)tbl.b[i] == opts_8016e685.field_02) {
            data_800f8554_slot0f[1] = i;
            break;
        }
    }
    q = &flag_8016e688;
    if (*q != 0) {
        data_800f8554_slot0f[1] = 4;
    }
    data_800f8554_slot0f[8] = opts_8016e685.field_01;
    data_800f8554_slot0f[2] = opts_8016e685.field_04;
    data_800f8554_slot0f[3] = opts_8016e685.field_05;
    data_800f8554_slot0f[4] = opts_8016e685.field_07;
    data_800f8554_slot0f[5] = opts_8016e685.field_06;
    func_80120374((s8)opts_8016e685.field_01);
    data_800f8554_slot0f[6] = opts_8016e685.field_0a;
    data_800f8554_slot0f[7] = opts_8016e685.field_0b;
    {
        /* Stand-in, not recovered source: the address of the 16 bytes written as flag_8016e688 - 0x24 reproduces the original's choice of registers. Without it this function differs from the original in 13 instruction slots. */
        /* The 16 bytes are the first two rows of table_8016e664, another object, which lies 0x24 bytes below the flag. The figure is measured with table_8016e664 written directly. Inferred: the original reuses the register that holds the flag's address as the base. */
        u8 (*src)[8] = (u8 (*)[8])((u8 *)q - 0x24);
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 8; j++) {
                data_800f8564_slot0f[i][j] = src[i][j];
            }
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
