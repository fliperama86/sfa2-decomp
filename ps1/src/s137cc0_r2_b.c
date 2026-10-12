/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: set_table_columns_by_win_counts (original keeps only the left cf address in a
   register), adjust_counter_c_by_table_and_random (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_player_field_cf(void) {
    player_left.table_column = 0;
    player_right.table_column = 0;
}

void func_801385a0(GameState *state) {
    if (!(state->mode == 3 && state->mode3_active_flag == 0)) {
        update_script_index_for_both_players(state);
        tick_counter_a_add_delta_8(state);
        func_8013886c(state);
        set_counter_shifted_from_counter_c(state);
        set_player_table_column_from_object(state);
    }
}

void set_counter_shifted_from_counter_c(GameState *state) {
    state->counter_shifted = (s16)counter_c >> 3;
}
