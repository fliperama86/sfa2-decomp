/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void next_substep_if_halfword_set_slot01(void) {
    if (*(s16 *)&data_801ae066 != 0) {
        data_8018f5a0->substep_index++;
    }
}

void next_step_if_not_busy_clear_indexes_slot01(void) {
    if (game_state.busy_flag == 0) {
        HudState *h = data_8018f5a0;
        h->step_index++;
        h->substep_index = 0;
        game_state.timer_or_bits = 0;
        h->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        h->pass_index = 0;
        game_state.field_cc = 0;
        h->pass_index = 0;
        game_state.field_65 = 0;
    }
}
