/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 352 bytes against the original's 384, with other register
 * assignment and instruction order. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py, with
 * func_8001606c_slot12.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills in a record of 0x40
 * bytes in a table, selected by the object's field_03, with a fixed size
 * (16 by 16, kind 2) and a length byte that depends on game_state.field_2bd
 * (0x14 when it is 0, else 0x10), and has the resident routine
 * func_80130768 set up the object's sequence from a list of steps (the
 * list at entry 0 of the table data_80028444_slot12 when field_2bd is 0,
 * else the list at entry 8). When the second argument is 1 the length byte
 * becomes 0x1d and the routine is called again with the list at entry 4.
 * The record also takes a pointer chosen by the object's field_5c, the
 * object's field_76 and field_78 minus 8 as its x and y, and the object
 * itself is moved to field_76 + 8 and field_78 - 8. A flag byte for the
 * record is set.
 *
 * Contract:
 *   Arguments: a0 = object (0x394 bytes in the test), a1 = int (only the
 *     value 1 is special). No return value.
 *   Reads: object field_03 (byte), field_5c (read as signed halfword),
 *     field_76 and field_78 (halfwords); game_state.field_2bd; the byte
 *     table data_80028500_slot12 and the pointer table
 *     data_800284ec_slot12, both at index field_5c.
 *   Writes: the record at index field_03 of data_8002bc94_slot12 (0x40
 *     bytes each): field_00, field_04, field_06, field_08 to field_0c; the
 *     flag byte data_8002bc8c_slot12[field_03]; the object's pos_x and
 *     pos_y.
 *   Valid inputs: field_03 below 8 (the flag bytes lie right before the
 *     records, and beyond 8 they would overlay record bytes); field_5c from
 *     -2 to 4 (the window the test fills; the files do not show the tables'
 *     lengths; negative values are kept to test the sign extension).
 *   Callee: func_80130768 (resident game code; replaced by a recorder, 3
 *     arguments: object, byte, list pointer). The log watches, at every
 *     call, the whole object (229 words), the record (16 words) and the
 *     flag bytes (2 words).
 *   Aliasing: object, record table, flag bytes distinct.
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Quad data_8002bc94_slot12[];
extern u8 data_8002bc8c_slot12[];
extern SequenceStep *data_80028444_slot12[];
extern u8 data_80028500_slot12[];
extern u8 *data_800284ec_slot12[];

void func_8001606c_slot12(Object *obj, int mode) {
    Slot12Rec *rec;
    SequenceStep **steps;

    rec = &((Slot12Rec *)data_8002bc94_slot12)[obj->field_03];
    rec->field_00 = 0;
    rec->field_08 = 0x10;
    rec->field_09 = 0x10;
    rec->field_0a = 2;
    if (game_state.field_2bd == 0) {
        rec->field_0b = 0x14;
        steps = &data_80028444_slot12[0];
    } else {
        rec->field_0b = 0x10;
        steps = &data_80028444_slot12[8];
    }
    func_80130768(obj, data_80028500_slot12[(s16)obj->field_5c], steps);
    if (mode == 1) {
        rec->field_0b = 0x1d;
        func_80130768(obj, data_80028500_slot12[(s16)obj->field_5c], &data_80028444_slot12[4]);
    }
    rec->field_0c = data_800284ec_slot12[(s16)obj->field_5c];
    data_8002bc8c_slot12[obj->field_03] = 1;
    rec->field_04 = obj->field_76 - 8;
    rec->field_06 = obj->field_78 - 8;
    obj->pos_x = obj->field_76 + 8;
    obj->pos_y = obj->field_78 - 8;
}
