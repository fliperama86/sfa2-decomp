/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b9d64_slot04_sel;

void start_selection_and_advance_step_slot04_sel(void) {
    HudState *h = screen_state;
    game_state.b0_flag = 1;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
    data_801b9d64_slot04_sel = 1;
    h->step_index_2++;
}

void count_down_then_advance_step_slot04_sel(void) {
    int t = data_801b9d64_slot04_sel - 1;
    data_801b9d64_slot04_sel = t;
    if ((u16)t == 0) {
        screen_state->step_index_2++;
    }
}

void advance_substep_index_clear_step_index_2_slot04_sel(void) {
    HudState *h = screen_state;
    game_state.field_09 = 0;
    h->step_index_2 = 0;
    h->substep_index++;
}
