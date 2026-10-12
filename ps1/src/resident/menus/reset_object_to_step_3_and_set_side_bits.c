/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void reset_object_to_step_3_and_set_side_bits(GameState *state, Object *object) {
    object->step_index = 3;
    object->phase_index = 0;
    object->phase_state = 0;
    object->vel_table_index = 0;
    *(u16 *)&object->table_index_3 = 0;
    object->countdown_timer = 0;
    object->field_b2 = 0;
    object->field_a5 = 2;
    object->mode_flag_2 = 0;
    object->win_count = 0;
    object->win_streak = 0;
    object->count = 0;
    object->score_sort_key = 0;
    object->effect_code_bc = 0;
    object->effect_request = 0;
    object->sound_index = 0;
    object->field_bf = 0;
    object->max_score = 0;
    object->kind = object->saved_side;
    reset_object_fields(object);
    if (object->side == 0) {
        state->side_bits |= 1;
        state->side_mask |= 1;
    } else if (object->side == 1) {
        state->side_bits |= 2;
        state->side_mask |= 2;
    }
    state->field_1c = 1;
}
