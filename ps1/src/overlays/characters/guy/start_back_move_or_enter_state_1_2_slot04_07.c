/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_back_move_or_enter_state_1_2_slot04_07(Object *obj) {
    u8 one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0) {
        if (obj->hit_flag_a != 0 && (try_force_other_state_if_box_in_range(obj, -0x12, 0x14) & 0xff) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 4) {
            obj->state_3 = 2;
            if (obj->attack_param_d != 0) {
                if (obj->facing_side == 0) {
                    obj->pos_x = obj->pos_x - 0x20;
                } else {
                    obj->pos_x = obj->pos_x + 0x20;
                }
            }
            set_sequence_by_side_and_mode(obj, 0x2f);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}
