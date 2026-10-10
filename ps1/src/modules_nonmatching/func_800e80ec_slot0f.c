/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register allocation,
 * frame size and instruction order. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use this
 * file. The differential test next to it (func_800e80ec_slot0f.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): initializes an object and
 * its 0x40-byte record in the table data_800f7d50_slot0f (indexed by the
 * object's field_03). It clears the record's first word, sets three
 * bytes (8, 9, 0xa) to 0x10, 0x10, 2, and a fourth (0xb) to 0x14 or 0x10
 * according to game_state.field_2bd (zero or not), and hands the object, a
 * byte of data_800f01e0_slot0f chosen by the object's field_5c, and a
 * sequence table (data_800f00fc_slot0f for zero, data_800f011c_slot0f
 * otherwise) to func_80130768. When the second argument is 1 it sets the
 * record's byte 0xb to 0x1d and calls func_80130768 again with
 * data_800f010c_slot0f. It then stores a pointer from data_800f01cc_slot0f
 * (by field_5c) into the record, sets a flag byte for the object in
 * data_800f7d48_slot0f, copies the object's field_76 and field_78 into the
 * record (minus 8 each) and sets the object's position to field_76 + 8 and
 * field_78 - 8.
 *
 * Contract (what the code reads and writes; the roles named are inferred):
 *   Arguments: a0 = object, a1 = flag (only the value 1 is special). No
 *     return value.
 *   Reads: obj->field_03, field_5c (signed 16 bits), field_76, field_78;
 *     game_state.field_2bd; the bytes of data_800f01e0_slot0f and the words
 *     of data_800f01cc_slot0f at field_5c.
 *   Writes: the record (offsets 0 to 0xc as above, and 4 and 6), the flag
 *     byte data_800f7d48_slot0f[field_03], obj->pos_x, obj->pos_y.
 *   Callee: func_80130768 (3 arguments) is replaced by a recorder returning
 *     0, because the object and sequence state that the real one works on
 *     take a contract of their own. The log copies the whole object, the
 *     record and the flag bytes at every call, so that a store moved across
 *     a call is a difference. Watched: the object (0x394 bytes), the record
 *     (0x40 bytes), data_800f7d48_slot0f (8 bytes).
 *   Aliasing: object and records are distinct blocks; the record of the
 *     object is inside data_800f7d50_slot0f.
 *   Inputs excluded: field_03 above 7 (the record table and the flag array
 *     are tables of eight here) and field_5c outside 0 to 4 (the two tables
 *     read have five entries / are followed by other data).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Quad data_800f7d50_slot0f[];
extern u8 data_800f7d48_slot0f[];
extern SequenceStep *data_800f00fc_slot0f[];
extern SequenceStep *data_800f010c_slot0f[];
extern SequenceStep *data_800f011c_slot0f[];
extern u8 *data_800f01cc_slot0f[];
extern u8 data_800f01e0_slot0f[];

void func_800e80ec_slot0f(Object *obj, int a) {
    Slot0fRec7d50 *e;
    SequenceStep **t;

    e = &((Slot0fRec7d50 *)data_800f7d50_slot0f)[obj->field_03];
    e->field_00 = 0;
    e->field_08 = 0x10;
    e->field_09 = 0x10;
    e->field_0a = 2;
    if (game_state.field_2bd == 0) {
        e->field_0b = 0x14;
        t = data_800f00fc_slot0f;
    } else {
        e->field_0b = 0x10;
        t = data_800f011c_slot0f;
    }
    func_80130768(obj, data_800f01e0_slot0f[(s16)obj->field_5c], t);
    if (a == 1) {
        e->field_0b = 0x1d;
        func_80130768(obj, data_800f01e0_slot0f[(s16)obj->field_5c], data_800f010c_slot0f);
    }
    e->field_0c = data_800f01cc_slot0f[(s16)obj->field_5c];
    data_800f7d48_slot0f[obj->field_03] = 1;
    e->field_04 = obj->field_76;
    e->field_06 = obj->field_78;
    e->field_04 = e->field_04 - 8;
    e->field_06 = e->field_06 - 8;
    obj->pos_x = obj->field_76 + 8;
    obj->pos_y = obj->field_78 - 8;
}
