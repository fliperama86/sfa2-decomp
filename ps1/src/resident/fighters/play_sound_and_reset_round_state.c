/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void play_sound_and_reset_round_state(Object *object) {
    add_round_score_to_other_side(object);
    play_sound_at(object, object->side, 0x323);
    object->field_69 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if ((object->field_1a1 & 0x80) != 0) {
            object->field_19f = 0;
        }
    }
    object->facing_side = object->saved_facing;
    object->field_170 = 0;
    object->field_292 = 0x20;
    if (object->mode_flag_2 == 0) {
        if (find_nearby_enemy_and_get_flag(object) != 0) {
            enter_state_2_6_by_button_input(object);
        } else if ((object->input_bits_2 & 0x4000) != 0) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            reset_object_to_state_1_0_sequence_0(object);
        }
    } else {
        reset_object_to_state_1_0_sequence_0(object);
    }
}
