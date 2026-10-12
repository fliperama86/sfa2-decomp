/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_state_3_check_range_or_special_move_slot04_01(Object *obj) {
    int d = 0x18;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            add_to_meter_clamped(obj, 1);
            set_sequence_by_side_and_mode(obj, 0x1c);
            return;
        }
        if (obj->facing_side == 0) {
            obj->pos_x = obj->pos_x - d;
        } else {
            obj->pos_x = d + obj->pos_x;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
