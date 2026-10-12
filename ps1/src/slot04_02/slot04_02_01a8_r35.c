/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_hop_from_other_box_position_slot04_02(Object *obj);
void set_state_3_and_directional_sequence_slot04_02(Object *obj);
void start_high_hop_from_other_box_position_slot04_02(Object *obj);
void set_state_3_9_direction_sequence_slot04_02(Object *obj);

void choose_air_attack_branch_by_button_bits_slot04_02(Object *obj) {
    if (obj->vel_y < 0) {
        if ((obj->pressed_bits | obj->released_bits) & 0x95) {
            if (obj->other->bound_push_flag == 0 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x54, 0x18)) {
                start_hop_from_other_box_position_slot04_02(obj);
                return;
            }
            set_state_3_and_directional_sequence_slot04_02(obj);
        }
        if (obj->vel_y < 0) {
            if ((obj->pressed_bits | obj->released_bits) & 0x6a) {
                if (obj->other->bound_push_flag == 0 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x30, 0x18)) {
                    start_high_hop_from_other_box_position_slot04_02(obj);
                } else {
                    set_state_3_9_direction_sequence_slot04_02(obj);
                }
            }
        }
    }
}
