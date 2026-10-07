/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_80120028(Object *o);

extern u8 data_800175e0_slot12[];
extern u8 data_80017630_slot12[];
extern SequenceStep *table_80017870_slot12[];

void func_80011d6c_slot12(Object *obj) {
    obj->field_01 = 1;
    obj->field_0c = 1;
    obj->field_90 = (void *)0x80040000;
    obj->field_98 = data_800175e0_slot12;
    obj->field_9c = data_80017630_slot12;
    obj->field_0e = 0;
    obj->field_24 = 0;
    obj->field_0d = 0;
    obj->field_0b = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_04++;
    if (obj->field_03 == 0) {
        obj->field_09 = 9;
        obj->pos_x = 0x90;
        obj->pos_y = 0x38;
        func_80130768(obj, 2, table_80017870_slot12);
    } else if (obj->field_03 == 1) {
        obj->field_09 = 0xb;
        obj->pos_x = 0;
        obj->pos_y = 0x28;
        func_80130768(obj, 0, table_80017870_slot12);
    } else if (obj->field_03 == 2) {
        obj->field_09 = 0xb;
        obj->pos_x = 0xc0;
        obj->pos_y = 0x28;
        func_80130768(obj, 1, table_80017870_slot12);
    }
}

void func_80011e58_slot12(Object *obj) {
    if (game_state.field_ab != 0 && obj->field_03 == 0) {
        obj->field_04++;
    }
    func_80120028(obj);
}
