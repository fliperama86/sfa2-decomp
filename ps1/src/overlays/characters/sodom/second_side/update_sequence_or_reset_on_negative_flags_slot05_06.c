/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_801dd560_slot05_06[];
extern ObjectFn data_801dd56c_slot05_06[];

void update_sequence_or_reset_on_negative_flags_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_direction_index_6_slot05_06(Object *obj) {
    data_801dd560_slot05_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_21_slot05_06(Object *obj) {
    data_801dd56c_slot05_06[obj->state_3](obj);
}

void restore_facing_enter_state_1_2_on_hit_range_2_slot05_06(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
