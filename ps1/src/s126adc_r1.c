/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126adc(GameState *state, Object *object) {
    if (object->side == 0)
        state->side_bits_b |= 1;
    else if (object->side == 1)
        state->side_bits_b |= 2;
    if (state->state_46_flag != 0 || state->mode_copy_2 == 0)
        func_80126d68(state, object);
    else if (object->phase_index == 0)
        func_80126b8c(state, object);
    else if (object->phase_index == 1)
        func_80126bcc(state, object);
}

void func_80126b8c(GameState *state, Object *object) {
    object->countdown_timer = 0x7f;
    object->field_b2 = 9;
    object->phase_index++;
    func_80135c80(object, 0);
}
