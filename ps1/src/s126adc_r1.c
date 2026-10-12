/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_side_bit_and_dispatch_by_phase(GameState *state, Object *object) {
    if (object->side == 0)
        state->side_bits_b |= 1;
    else if (object->side == 1)
        state->side_bits_b |= 2;
    if (state->state_46_flag != 0 || state->mode_copy_2 == 0)
        reset_phase_fields_and_next_step(state, object);
    else if (object->phase_index == 0)
        init_countdown_and_advance_phase(state, object);
    else if (object->phase_index == 1)
        func_80126bcc(state, object);
}

void init_countdown_and_advance_phase(GameState *state, Object *object) {
    object->countdown_timer = 0x7f;
    object->field_b2 = 9;
    object->phase_index++;
    set_wait_flag(object, 0);
}
