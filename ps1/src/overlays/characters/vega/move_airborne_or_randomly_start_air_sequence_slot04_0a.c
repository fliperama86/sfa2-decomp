/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);
void land_on_ground_set_state_3_seven_slot04_0a(Object *obj);
void nudge_pos_x_from_input_bits_slot04_0a(Object *obj);

void move_airborne_or_randomly_start_air_sequence_slot04_0a(Object *obj) {
    int k;

    if (obj->vel_y < 0) {
        if ((get_random_byte() & 0xf) != 0) {
            k = get_random_byte() & 3;
            if (k != 3) {
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, k | 0x28);
                add_to_meter_clamped(obj, 2);
                add_one_to_ec4_if_object_unflagged(&game_state, obj);
                nudge_pos_x_from_input_bits_slot04_0a(obj);
                if (apply_velocity_and_check_ground_slot04_0a(obj)) {
                    land_on_ground_set_state_3_seven_slot04_0a(obj);
                }
                return;
            }
        }
    }
    nudge_pos_x_from_input_bits_slot04_0a(obj);
    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        land_on_ground_set_state_3_seven_slot04_0a(obj);
    }
    update_sequence(obj);
}
