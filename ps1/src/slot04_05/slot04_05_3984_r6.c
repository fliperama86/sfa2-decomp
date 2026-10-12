/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void restore_facing_and_start_sequence_by_direction_slot04_05(Object *obj) {
    obj->facing_side = obj->saved_facing;
    obj->state_3++;
    if (obj->direction_index == 2) {
        if (obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            add_to_meter_clamped(obj, 1);
            set_sequence_by_side_and_mode(obj, 0x28);
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_or_end_state_slot04_05(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
