/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
extern ObjectFn data_801dd428_slot05_06[];

void slide_until_vel_x_negative_slot05_06(Object *obj) {
    if (obj->vel_x >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
        update_sequence(obj);
    } else {
        obj->state_3++;
        update_sequence(obj);
    }
}

void update_sequence_or_finish_sequence_2_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_12_slot05_06(Object *obj) {
    data_801dd428_slot05_06[obj->state_3](obj);
}

void start_jump_with_fixed_velocity_slot05_06(Object *obj) {
    int v;

    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 8);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->active_state = 1;
    obj->active_flag_3 = 1;
    v = 0x48;
    if (obj->field_49 == 0) {
        v = 0x2a;
    }
    set_sequence_by_side_and_mode(obj, v);
}
