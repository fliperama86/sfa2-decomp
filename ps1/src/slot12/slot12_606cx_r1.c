/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Quad data_8002bc94_slot12[];
extern u8 data_8002bc8c_slot12[];
extern SequenceStep *data_80028444_slot12[];
extern u8 data_80028500_slot12[];
extern u8 *data_800284ec_slot12[];

/* What it does (inferred, not an original name): fills in a record of 0x40
   bytes in a table, selected by the object's field_03, with a fixed size
   (16 by 16, kind 2) and a length byte that depends on game_state.field_2bd
   (0x14 when it is 0, else 0x10), and has the resident routine
   func_80130768 set up the object's sequence from a list of steps (the
   list at entry 0 of the table data_80028444_slot12 when field_2bd is 0,
   else the list at entry 8). When the second argument is 1 the length byte
   becomes 0x1d and the routine is called again with the list at entry 4.
   The record also takes a pointer chosen by the object's health, the
   object's field_76 and field_78 minus 8 as its x and y, and the object
   itself is moved to field_76 + 8 and field_78 - 8. A flag byte for the
   record is set. It is the twin of func_800e80ec_slot0f, with that
   unit's form. */
void func_8001606c_slot12(Object *obj, int mode) {
    Slot12Rec *rec = &((Slot12Rec *)data_8002bc94_slot12)[obj->field_03];

    rec->field_00 = 0;
    rec->field_08 = 0x10;
    rec->field_09 = 0x10;
    rec->field_0a = 2;
    /* The first call and the second, conditional one stand in each arm, with the arm's list in a local of its own. With one call of each kind behind the arms and the list in one local set in both this function differs from the original in 24 instruction slots. */
    if (game_state.field_2bd == 0) {
        SequenceStep **steps = &data_80028444_slot12[0];

        rec->field_0b = 0x14;
        func_80130768(obj, data_80028500_slot12[(s16)obj->health], steps);
        if (mode == 1) {
            rec->field_0b = 0x1d;
            func_80130768(obj, data_80028500_slot12[(s16)obj->health], &data_80028444_slot12[4]);
        }
    } else {
        SequenceStep **steps = &data_80028444_slot12[8];

        rec->field_0b = 0x10;
        func_80130768(obj, data_80028500_slot12[(s16)obj->health], steps);
        if (mode == 1) {
            rec->field_0b = 0x1d;
            func_80130768(obj, data_80028500_slot12[(s16)obj->health], &data_80028444_slot12[4]);
        }
    }
    rec->field_0c = data_800284ec_slot12[(s16)obj->health];
    data_8002bc8c_slot12[obj->field_03] = 1;
    rec->pos_x = obj->field_76;
    rec->pos_y = obj->field_78;
    rec->pos_x = rec->pos_x - 8;
    rec->pos_y = rec->pos_y - 8;
    obj->pos_x = obj->field_76 + 8;
    /* The halfword is read signed here, as the original reads it (`lh`); without the cast this function differs from the original in 15 instruction slots. */
    obj->pos_y = (s16)obj->field_78 - 8;
}
