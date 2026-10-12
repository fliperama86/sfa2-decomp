/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017064_slot27(Object *obj);
extern s16 data_80055eb2;
extern s16 data_80055eb6;
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_80028e10_slot27[];

void func_800168e8_slot27(Object *obj) {
    func_80017064_slot27(obj);
    obj->field_01 = 1;
    obj->ot_depth = 2;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->field_90 = (void *)0x80038000;
    obj->facing_side = 0;
    obj->spawn_x = 0;
    obj->image_row = 0x1c;
    obj->state_0++;
    if (game_state.kind_index == 0x12) {
        func_80130768(obj, 0x14, data_80028e10_slot27);
    } else {
        func_80130768(obj, (s16)game_state.kind_index, data_80028e10_slot27);
    }
    obj->pos_x = 0x70;
    obj->pos_y = 0x2c;
    data_80055eb2 = 0x19d;
    data_80055eb6 = 0x20;
}
