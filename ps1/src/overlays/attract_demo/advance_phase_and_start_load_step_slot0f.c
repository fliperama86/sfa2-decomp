/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int load_saved_settings_from_card_slot0f(void);

void advance_phase_and_start_load_step_slot0f(GameState *state) {
    screen_state->phase_index++;
    state->step_counter = 0x1f;
    state->step_delay_timer = 0x20;
    if (load_saved_settings_from_card_slot0f() == 0) {
        state->field_cc = 1;
    }
}
