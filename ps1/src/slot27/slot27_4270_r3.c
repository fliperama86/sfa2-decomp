/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027cbc_slot27[];

void add_angle_step_until_limit_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    obj->angle += obj->field_4c;
    if ((s16)obj->angle == 0x1a0) {
        obj->field_46 = 0x18;
        obj->field_05 = obj->field_05 + 1;
    }
}

void count_down_then_clear_flag_next_state_1_slot27(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->field_01 = 0;
        obj->state_1 = obj->state_1 + 1;
    }
}

void release_object_to_pool_table_3_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_state_1_handler_from_slot27_table_b_2_slot27(Object *obj) {
    data_80027cbc_slot27[obj->state_1](obj);
}

void start_move_down_set_vel_y_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->vel_y = 8;
    obj->state_1 = obj->state_1 + 1;
}
