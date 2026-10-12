/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: set_table_columns_by_win_counts (original keeps only the left cf address in a
   register), adjust_counter_c_by_table_and_random (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_counters_from_index_table(GameState *state) {
    int value = table_6cf0[state->table_index_2];

    counter_a = 0x780;
    counter_b = 0;
    state->counter_shifted = value;
    counter_c = value << 3;
}

void update_table_columns_unless_mode3_inactive(GameState *state) {
    if (state->mode == 3 && state->mode3_active_flag == 0) {
        set_table_columns_by_win_counts(state);
    } else {
        update_counter_limit_from_table(state);
        clamp_meter_value_to_byte_range(state);
        set_counter_shifted_from_counter_c(state);
        set_player_table_column_from_object(state);
    }
}

void reset_counter_a_and_table_columns(GameState *state) {
    counter_a = 0x780;
    if (state->mode == 3 && state->mode3_active_flag == 0) {
        set_table_columns_by_win_counts(state);
    } else {
        if (player_left.mode_flag_2 == 0 && game_state.mode3_active_flag == 0) {
            player_left.table_column = 0x10;
        }
        if (player_right.mode_flag_2 == 0 && game_state.mode3_active_flag == 0) {
            player_right.table_column = 0x10;
        }
    }
}
