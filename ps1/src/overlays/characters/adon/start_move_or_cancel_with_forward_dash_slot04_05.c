/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_or_cancel_with_forward_dash_slot04_05(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else if (obj->direction_index == 2) {
        if (obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            add_to_meter_clamped(obj, 1);
            *(s32 *)&obj->vel_x = 0x68000;
            obj->accel_x = -0x8000;
            set_sequence_by_side_and_mode(obj, 0x34);
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
