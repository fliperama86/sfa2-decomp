/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_table_function_by_mode_byte(Object *object) {
    table_80171a08[object->field_60](object);
}

void call_state_3_table_function_2(Object *object) {
    table_80171a20[object->state_3](object);
}

void advance_state_3_play_sound_and_step(Object *object) {
    object->state_3 = object->state_3 + 1;
    play_side_sound_0(object);
    update_table_step_with_input_branches(object);
}
