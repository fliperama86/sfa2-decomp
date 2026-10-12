/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800175e0_slot12[];
extern u8 data_80017630_slot12[];
extern SequenceStep *table_80017870_slot12[];

void func_80011d6c_slot12(Object *obj) {
    obj->field_01 = 1;
    obj->field_0c = 1;
    obj->field_90 = (void *)0x80040000;
    obj->data_ptr_a = data_800175e0_slot12;
    obj->data_ptr_b = data_80017630_slot12;
    obj->field_0e = 0;
    obj->table_index_2 = 0;
    obj->image_row = 0;
    obj->facing_side = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->state_0++;
    if (obj->table_index == 0) {
        obj->ot_depth = 9;
        obj->pos_x = 0x90;
        obj->pos_y = 0x38;
        func_80130768(obj, 2, table_80017870_slot12);
    } else if (obj->table_index == 1) {
        obj->ot_depth = 0xb;
        obj->pos_x = 0;
        obj->pos_y = 0x28;
        func_80130768(obj, 0, table_80017870_slot12);
    } else if (obj->table_index == 2) {
        obj->ot_depth = 0xb;
        obj->pos_x = 0xc0;
        obj->pos_y = 0x28;
        func_80130768(obj, 1, table_80017870_slot12);
    }
}

void func_80011e58_slot12(Object *obj) {
    if (game_state.status_flags != 0 && obj->table_index == 0) {
        obj->state_0++;
    }
    func_80120028(obj);
}
