/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd574_slot05_06[];

void update_sequence_or_reset_on_negative_flags_2_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_22_slot05_06(Object *obj) {
    data_801dd574_slot05_06[obj->state_3](obj);
}

void update_hit_flag_range_check_or_default_slot05_06(Object *obj) {
    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void shift_pos_x_on_step_flag_advance_state_3_slot05_06(Object *obj) {
    int d = 0x18;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}
