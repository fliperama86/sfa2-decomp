/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void choose_jump_or_action_from_direction_input_slot04_03(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0) {
        if (obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 0;
        } else if (obj->direction_index != 2) {
            if (obj->attack_param_d != 0 && obj->direction_flag == 0) {
                obj->state_3 = 2;
                *(s16 *)&obj->countdown = -1;
                add_to_meter_clamped(obj, 2);
                set_sequence_by_side_and_mode(obj, 0x1c);
            } else {
                set_sequence_and_tables_from_direction(obj);
            }
        } else if (obj->attack_param_d != 0) {
            obj->state_3 = 3;
            obj->vel_y = 0xfffc0000;
            obj->accel_y = 0x6000;
            if (obj->facing_side != (obj->direction_flag ^ 1)) {
                obj->vel_x = 0xfffca000;
            } else {
                obj->vel_x = 0x36000;
            }
            add_to_meter_clamped(obj, 1);
            set_sequence_by_side_and_mode(obj, 0x1d);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}
