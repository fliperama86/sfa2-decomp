/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_step_handler_by_step_index(GameState *state, Object *object) {
    if (object->field_a5 == 0) data_8016f5a4[object->step_index](state, object);
}

void func_801265ac(GameState *state, Object *object) {
    if (*(u32 *)&state->field_44 & 0xff00ff)
        clear_wait_flag(object);
    else if (object->phase_index == 0)
        increment_object_field(state, object);
}

void increment_object_field(GameState *state, Object *object) {
    object->step_index++;
}
