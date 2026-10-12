/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: advance_state_2_flip_facing_side (original stores countdown through a0 in the delay slot). */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_state_2_handler_2684_unless_paused(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        handlers_2684[object->state_2](object);
    }
    if ((object->countdown >> 8) & 0x92) {
        clear_or_set_flag_by_pos_x_limit(object);
    }
}

void advance_state_2_set_countdown_bits_and_sequence(Object *object) {
    object->state_2++;
    object->countdown = (u8)object->countdown | 0x1800;
    advance_sequence_checked(object);
}

void tick_countdown_then_set_state_0_to_3(Object *object) {
    *(s16 *)&object->countdown -= 0x100;
    if ((s16)object->countdown < 0) {
        object->state_0 = 3;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
    advance_sequence_checked(object);
}

void call_state_2_handler_268c_unless_paused(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        handlers_268c[object->state_2](object);
    }
    clear_or_set_flag_by_pos_x_limit(object);
}
