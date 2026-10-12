/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_state_1_1_2_0_or_enter_state_1_6(Object *object) {
    if ((s16)object->health < 0) {
        enter_state_1_6_and_set_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
    }
}

void set_state_3_two_if_step_done(Object *object) {
    if (apply_table_step_to_pos_x(object) != 0) {
        object->state_3 = 2;
    }
}

void play_side_sound_0(Object *object) {
    play_side_sound_at(object, object->side, 0);
}

void play_side_sound_1(Object *object) {
    play_side_sound_at(object, object->side, 1);
}

void call_state_3_table_function_3(Object *object) {
    table_80171a30[object->state_3](object);
}

void advance_state_3_and_call_step(Object *object) {
    object->state_3 = object->state_3 + 1;
    update_table_step_then_choose_sequence_c_or_e(object);
}
