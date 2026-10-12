/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void count_down_updating_sequence_then_next_state_3(Object *object) {
    int v;
    object->field_15b = 0;
    if (object->kind == 0x14) {
        play_sound_and_reset_round_state(object);
        return;
    }
    if (game_state.config->mode_flag_b == 0 && !is_other_object_state_0_and_1_six_one(object)) {
        v = object->countdown - 1;
        object->countdown = v;
        if ((s16)v != -1) {
            if ((u8)is_object_pressed_bits_high_nibble_set(object)) {
                update_sequence(object);
                v = object->countdown - 1;
                object->countdown = v;
                if ((s16)v == -1) goto tail;
                if ((u8)is_prev_input_low_byte_zero(object)) {
                    update_sequence(object);
                    v = object->countdown - 1;
                    object->countdown = v;
                    if ((s16)v == -1) goto tail;
                }
            }
            if ((u8)has_pressed_high_bits_or_call_random_test(object)) {
                update_sequence(object);
                v = object->countdown - 1;
                object->countdown = v;
                if ((s16)v == -1) goto tail;
            }
            update_sequence(object);
            return;
        }
    }
tail:
    object->state_3 = object->state_3 + 1;
}
