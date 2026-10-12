/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_2_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void start_state_choose_hit_or_jump_sequence_slot04_02(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            set_object_states_0_to_3_2_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
    }
    if (obj->direction_index == 2 && obj->attack_param_d != 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = 2;
        add_to_meter_clamped(obj, 1);
        set_sequence_by_side_and_mode(obj, 0x27);
        return;
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_reset_by_input_dir_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        if (obj->input_dir_0 == 2) {
            reset_object_to_state_1_0_0_2_sequence_1(obj);
        } else {
            reset_object_to_state_1_0_sequence_0(obj);
        }
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
