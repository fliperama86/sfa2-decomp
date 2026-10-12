/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_state3_first_frame_range_check_narrow_slot04_04(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
            if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0xe) != 0) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
            } else {
                obj->active_flag_3 = 1;
                set_sequence_and_tables_from_direction(obj);
            }
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    }
}

void update_state_3_start_move_with_sound_slot04_04(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index == 4) {
            play_side_sound_at(obj, obj->side, 0xb);
        }
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
