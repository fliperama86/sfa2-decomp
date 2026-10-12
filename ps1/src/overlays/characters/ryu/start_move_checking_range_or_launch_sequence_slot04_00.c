/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_checking_range_or_launch_sequence_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (try_force_other_state_if_box_in_range(obj, -0x14, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->state_3 = 2;
            add_to_meter_clamped(obj, 1);
            if (obj->facing_side == 0) {
                obj->vel_x = -0x36000;
            } else {
                obj->vel_x = 0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            set_sequence_by_side_and_mode(obj, 0x2f);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
