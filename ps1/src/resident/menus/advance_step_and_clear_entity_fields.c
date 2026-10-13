/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_step_and_clear_entity_fields(GameState *state) {
    Entity *e = (Entity *)state;
    e->field_83 = 0;
    screen_state->step_index_2++;
    e->active_flag_2 = 0;
    if (e->field_84 == 0) {
        e->active_flag = 0;
        e->field_2c = 0;
    }
}
