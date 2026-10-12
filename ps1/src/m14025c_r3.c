/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int match_pressed_direction_input(Object *object) {
    BytePair buf;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_a) != 0 || (is_other_object_state_0_and_1_six_one(object) & 0xff) != 0 ||
        (object->pressed_bits & 0xfc) == 0) {
        object->input_matched_flag = 0;
    } else {
        object->facing_side = object->saved_facing;
        buf = get_direction_pair_from_pressed_bits(object);
        if (object->direction_index != (u8)buf.first) {
            object->input_matched_flag = 0;
        } else {
            object->input_dir_second = buf.second;
            object->input_dir_0 = 0;
            if (object->input_bits_2 & 0x4000) object->input_dir_0 = 2;
            object->field_16a = 0;
            reset_hold_flags_and_other_hit_count(object);
            object->input_matched_flag = 1;
            return 1;
        }
    }
    return 0;
}
