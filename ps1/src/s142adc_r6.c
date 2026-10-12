/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_state_1_handler_2(Object *object) {
    table_8017b5c4[object->state_1](object);
}

void call_handler_by_state_2_b5d4_and_advance(Object *object) {
    table_8017b5d4[object->state_2](object);
    advance_sequence_checked(object);
    set_flag_and_push_if_mode_8(object);
}

void init_position_c0_70_and_set_sequence_f(Object *object) {
    object->pos_x = 0xc0;
    object->pos_y = 0x70;
    object->state_2 = object->state_2 + 1;
    set_sequence_from_effect_table(object, 0xf);
    on_step_flag_advance_state_2_play_sound(object);
}

void on_step_flag_advance_state_2_play_sound(Object *object) {
    if ((u8)object->step_flags) {
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
        play_sound_at(0, 0, 0x207);
    }
}
