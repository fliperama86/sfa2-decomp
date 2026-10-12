/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd414_slot05_06[];
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);

void call_table_function_by_state_3_11_slot05_06(Object *obj) {
    data_801dd414_slot05_06[obj->state_3](obj);
}

void call_effect_on_step_flag_byte_slot05_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        spawn_offset_effect_and_play_sounds(obj);
        obj->state_3++;
    }
    update_sequence(obj);
}

void wait_step_flag_then_start_slide_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->vel_x = 0xc0000;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->accel_x = 0xffff0000;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x3a);
    }
}

void slide_until_flag_2_or_vel_x_negative_slot05_06(Object *obj) {
    u8 t = obj->step_flags;

    if (t != 0) {
        if (t != 2 && obj->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
            update_sequence(obj);
        } else {
            obj->accel_x = -0x6000;
            obj->state_3++;
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}
