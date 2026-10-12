/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Quad data_800f7d50_slot0f[];
extern u8 data_800f7d48_slot0f[];
/* The three sequence tables that the function passes (at 0x800f00fc,
   0x800f010c and 0x800f011c, four pointers each) are taken as one array:
   entries 0, 4 and 8. Inferred from the build: with three arrays of their
   own the unit differs from the original, see the comment in the function;
   the twin of this function in the image slot12 has its tables in one
   array too. */
extern SequenceStep *data_800f00fc_slot0f[];
extern u8 *data_800f01cc_slot0f[];
extern u8 data_800f01e0_slot0f[];

/* What it does (inferred, not an original name): initializes an object and
   its 0x40-byte record in the table data_800f7d50_slot0f (indexed by the
   object's field_03). It clears the record's first word, sets three
   bytes (8, 9, 0xa) to 0x10, 0x10, 2, and a fourth (0xb) to 0x14 or 0x10
   according to game_state.field_2bd (zero or not), and hands the object, a
   byte of data_800f01e0_slot0f chosen by the object's health, and a
   sequence table (entry 0 of data_800f00fc_slot0f for zero, entry 8
   otherwise) to func_80130768. When the second argument is 1 it sets the
   record's byte 0xb to 0x1d and calls func_80130768 again with the table
   at entry 4. It then stores a pointer from data_800f01cc_slot0f
   (by health) into the record, sets a flag byte for the object in
   data_800f7d48_slot0f, copies the object's field_76 and field_78 into the
   record (minus 8 each) and sets the object's position to field_76 + 8 and
   field_78 - 8. */
void func_800e80ec_slot0f(Object *obj, int a) {
    Slot0fRec7d50 *e = &((Slot0fRec7d50 *)data_800f7d50_slot0f)[obj->field_03];

    e->field_00 = 0;
    e->field_08 = 0x10;
    e->field_09 = 0x10;
    e->field_0a = 2;
    /* The first call and the second, conditional one stand in each arm, with the arm's sequence table in a local of its own. With one call of each kind behind the arms and the table in one local set in both this function differs from the original in 24 instruction slots; with the three tables as three arrays of their own, in 31. */
    if (game_state.field_2bd == 0) {
        SequenceStep **p = &data_800f00fc_slot0f[0];

        e->field_0b = 0x14;
        func_80130768(obj, data_800f01e0_slot0f[(s16)obj->health], p);
        if (a == 1) {
            e->field_0b = 0x1d;
            func_80130768(obj, data_800f01e0_slot0f[(s16)obj->health], &data_800f00fc_slot0f[4]);
        }
    } else {
        SequenceStep **p = &data_800f00fc_slot0f[8];

        e->field_0b = 0x10;
        func_80130768(obj, data_800f01e0_slot0f[(s16)obj->health], p);
        if (a == 1) {
            e->field_0b = 0x1d;
            func_80130768(obj, data_800f01e0_slot0f[(s16)obj->health], &data_800f00fc_slot0f[4]);
        }
    }
    e->field_0c = data_800f01cc_slot0f[(s16)obj->health];
    data_800f7d48_slot0f[obj->field_03] = 1;
    e->field_04 = obj->field_76;
    e->field_06 = obj->field_78;
    e->field_04 = e->field_04 - 8;
    e->field_06 = e->field_06 - 8;
    obj->pos_x = obj->field_76 + 8;
    obj->pos_y = obj->field_78 - 8;
}
