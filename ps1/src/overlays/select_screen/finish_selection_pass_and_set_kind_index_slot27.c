/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002f158_slot27;

/* One local holds two values in turn: the mode value that the first test
   compares, and then the byte that goes to kind_index. With a second local for
   the byte, this function differs from the original in 11 instruction slots;
   with the first test written without a local, in 15. */
void finish_selection_pass_and_set_kind_index_slot27(void) {
    int value;
    data_8002f158_slot27 = 0;
    value = game_state.mode | game_state.side_mask;
    if (screen_state->pass_index == value) {
        screen_state->substep_index++;
        game_state.field_09 = 1;
        select_kind_for_second_player((Select *)&game_state);
        if (game_state.b0_flag != 0) {
            value = game_state.selection_value;
            if ((value & 0x80) == 0) {
                if (game_state.b2_count != 0) {
                    if (game_state.b2_count == 10) {
                        value = 0x13;
                    }
                    if (game_state.b2_count == 11) {
                        value = 0x12;
                    }
                }
                game_state.kind_index = value;
            }
        }
        game_state.countdown_2 = 0;
        screen_state->step_index_2 = 0;
        screen_state->pass_index = 0;
        screen_state->field_54 = 0;
        screen_state->substep_index++;
        if ((game_state.mode | game_state.side_mask) == 3) {
            screen_state->substep_index++;
            game_state.countdown_2 = 0x40;
        }
    }
}
