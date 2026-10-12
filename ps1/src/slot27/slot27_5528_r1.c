/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_800284e0_slot27[];
extern GameState *data_800284f8_slot27;
void func_80015838_slot27(Object *obj);
void func_800158c0_slot27(Object *obj);
void func_80015950_slot27(Object *obj);

void func_80015528_slot27(Object *obj) {
    obj->ot_depth = 0x10;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_x = 0x20;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0x18;
    obj->field_90 = (void *)0x80038000;
    obj->field_01 = 0;
    obj->state_0++;
    if (obj->table_index == 0) {
        func_800158c0_slot27(obj);
        func_80015950_slot27(obj);
        func_80130768(obj, 0, data_800284e0_slot27);
    } else if (obj->table_index == 1) {
        func_800158c0_slot27(obj);
        obj->state_0 = 2;
        obj->pos_x = data_800284f8_slot27->field_d2;
        obj->pos_y = data_800284f8_slot27->field_d4;
        func_80015838_slot27(obj);
        func_80015950_slot27(obj);
        func_80130768(obj, 1, data_800284e0_slot27);
    } else if (obj->table_index >= 2) {
        obj->state_0 = 4;
        func_80130768(obj, obj->table_index, data_800284e0_slot27);
    }
}
