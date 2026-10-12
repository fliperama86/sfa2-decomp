/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027ccc_slot27[];
extern ObjectFn data_80027cdc_slot27[];
extern ObjectFn data_80027cec_slot27[];

void func_80014744_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void func_80014764_slot27(Object *obj) {
    data_80027ccc_slot27[obj->state_1](obj);
}

void start_move_down_set_vel_y_2_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->vel_y = 8;
    obj->state_1 = obj->state_1 + 1;
}

void func_800147c4_slot27(Object *obj) {
    data_80027cdc_slot27[obj->state_1](obj);
}

void start_spawn_x20_move_down_slot27(Object *obj) {
    obj->spawn_x = 0x20;
    obj->spawn_y = 0x1e0;
    obj->field_01 = 1;
    obj->vel_y = 8;
    obj->state_1 = obj->state_1 + 1;
}

void func_80014834_slot27(Object *obj) {
    data_80027cec_slot27[obj->state_1](obj);
}

void start_spawn_x10_move_left_slot27(Object *obj) {
    obj->spawn_x = 0x10;
    obj->spawn_y = 0x1e0;
    obj->field_01 = 1;
    obj->vel_x = -8;
    obj->state_1 = obj->state_1 + 1;
}
