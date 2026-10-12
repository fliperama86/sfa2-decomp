/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_801dd4f8_slot05_06[];

void hit_other_by_input_dir_when_mode_set_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->mode != 0) {
        obj->state_3++;
        if (obj->input_dir_second == 0) {
            set_other_hit_reaction_state_0(obj, 0x26, 5, 0x11, 0, 0, 1);
        } else {
            set_other_hit_reaction_state_0(obj, 0x29, 5, 1, 0, 0, 1);
        }
    }
    update_sequence(obj);
}

void update_sequence_or_finish_sequence_3_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj) {
    int t = obj->vel_x;

    if (obj->facing_side == 0) {
        t = -t;
    }
    *(s32 *)&obj->pos_x_frac += t;
    obj->vel_x += obj->accel_x;
}

void apply_vertical_velocity_slot05_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void negate_value_if_facing_side_zero_slot05_06(Object *obj, u16 *p) {
    if (obj->facing_side == 0) {
        *p = -*p;
    }
}

void call_handler_by_state_2_slot05_06(Object *obj) {
    data_801dd4f8_slot05_06[obj->state_2](obj);
}
