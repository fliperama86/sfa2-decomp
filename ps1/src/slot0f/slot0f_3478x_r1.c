/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A second name for the saved block at 0x8016e664 as one object: externs.h declares its parts apart (table_8016e664 for the rows, data_8016e685 and the bytes after it for the options). With the rows read through table_8016e664 this function differs from the original in 13 instruction slots. Inferred from the original's instructions: it reaches the rows from the address of the option byte at 0x8016e688 (the same register, offset -0x24), as the compiler does for two addresses in one object. */
typedef struct {
    u8 rows[4][8];
    u8 field_20;
    Slot0fOpts opts;
} Slot0fSaved;
extern Slot0fSaved saved_8016e664;
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
 * block (saved_8016e664) into the selections of a menu page. It copies
 * option bytes into the array data_800f8554_slot0f (index 0 from field_00;
 * index 1 the position of table_match_value among five bytes of data_800df32c_slot0f,
 * or 4 when its option byte field_03 is not zero, otherwise left as it was;
 * indices 8, 2, 3, 4, 5 from field_01, 04, 05, 07, 06), calls set_sound_mono_or_stereo
 * with field_01 as a signed byte, copies field_0a and field_0b into indices
 * 6 and 7, and copies the block's first two rows into
 * data_800f8564_slot0f. It then sets byte 0xb of three runs of cells of
 * data_800e953c_slot0f to 0x16 up to and including the selection and 0x1b
 * past it, and sets six pointers into tables of 12 and 8 byte records,
 * indexed by the selections 1, 2, 6, 7, 5 and 8.
 */
void load_options_from_saved_data_slot0f(void) {
    Slot0fRecf32c tbl = data_800df32c_slot0f;
    int i;
    int j;
    data_800f8554_slot0f[0] = saved_8016e664.opts.field_00;
    for (i = 0; i < 5; i++) {
        if ((s8)tbl.b[i] == saved_8016e664.opts.table_match_value) {
            data_800f8554_slot0f[1] = i;
            break;
        }
    }
    if (saved_8016e664.opts.field_03 != 0) {
        data_800f8554_slot0f[1] = 4;
    }
    data_800f8554_slot0f[8] = saved_8016e664.opts.field_01;
    data_800f8554_slot0f[2] = saved_8016e664.opts.field_04;
    data_800f8554_slot0f[3] = saved_8016e664.opts.field_05;
    data_800f8554_slot0f[4] = saved_8016e664.opts.field_07;
    data_800f8554_slot0f[5] = saved_8016e664.opts.field_06;
    set_sound_mono_or_stereo((s8)saved_8016e664.opts.field_01);
    data_800f8554_slot0f[6] = saved_8016e664.opts.field_0a;
    data_800f8554_slot0f[7] = saved_8016e664.opts.field_0b;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            data_800f8564_slot0f[i][j] = saved_8016e664.rows[i][j];
        }
    }
    for (i = 0; i < 8; i++) {
        if (i <= data_800f8554_slot0f[0]) {
            data_800e953c_slot0f[i].cell_value = 0x16;
        } else {
            data_800e953c_slot0f[i].cell_value = 0x1b;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i <= data_800f8554_slot0f[3]) {
            data_800e953c_slot0f[8 + i].cell_value = 0x16;
        } else {
            data_800e953c_slot0f[8 + i].cell_value = 0x1b;
        }
    }
    for (i = 0; i < 8; i++) {
        if (i <= data_800f8554_slot0f[4]) {
            data_800e953c_slot0f[12 + i].cell_value = 0x16;
        } else {
            data_800e953c_slot0f[12 + i].cell_value = 0x1b;
        }
    }
    data_800e9688_slot0f = &data_800e94b0_slot0f[data_800f8554_slot0f[1]];
    data_800e9698_slot0f = &data_800e94ec_slot0f[data_800f8554_slot0f[2]];
    data_800e96a8_slot0f = &data_800e951c_slot0f[data_800f8554_slot0f[6]];
    data_800e96b8_slot0f = &data_800e951c_slot0f[data_800f8554_slot0f[7]];
    data_800e96c8_slot0f = &data_800e950c_slot0f[data_800f8554_slot0f[5]];
    data_800e96d8_slot0f = &data_800e952c_slot0f[data_800f8554_slot0f[8]];
}
