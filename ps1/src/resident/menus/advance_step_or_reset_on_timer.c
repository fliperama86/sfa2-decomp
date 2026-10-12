/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_step_or_reset_on_timer(GameState *state) {
    if (((Entity *)state)->field_6c == 0) {
        if (((Entity *)state)->mode_flag_b == 0) {
            ((HudBig *)data_8018f5a0)->step_count++;
            ((Entity *)state)->field_c8 = 0;
            ((Entity *)state)->countdown_timer = 0;
            ((HudBig *)data_8018f5a0)->substate_index = 0;
            ((Entity *)state)->field_cc = 0;
            if (((Entity *)state)->nonzero_flag != 0 || ((Entity *)state)->field_78->mode_flag_2 != 0) {
                ((HudBig *)data_8018f5a0)->state_index = 5;
            } else {
                ((HudBig *)data_8018f5a0)->state_index = 0;
                ((Entity *)state)->field_ac = 0xff;
            }
        } else {
            int v = ((Entity *)state)->countdown_timer - 1;
            ((Entity *)state)->countdown_timer = v;
            if ((s16)v == 0) {
                reset_game_state_for_new_match(state);
            }
        }
    }
}
