/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void restart_substep_when_field_six_c_is_four(GameState *state) {
    Entity *e = (Entity *)state;
    if (e->field_6c == 4) {
        screen_state->step_index_2 = 0;
        screen_state->substep_index++;
        e->countdown_timer = 0;
        e->field_4e = 0;
        background_layers[0].active_flag = 1;
        background_layers[1].active_flag = 1;
        background_layers[2].active_flag = 1;
        e->field_6d = 1;
    }
}
