/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void count_down_timer_2_then_set_step_index_d(Object *unused) {
    if (game_state.busy_flag == 0) {
        game_state.countdown_timer_2 = (s16)game_state.countdown_timer_2 - 1;
        if ((s16)game_state.countdown_timer_2 < 0) {
            data_8018f5a0->step_index_2 = 0xd;
            blink_item_loop_until_button_with_palette_dim();
        }
    }
}

void set_step_index_2_if_not_busy(Object *unused) {
    if (game_state.busy_flag == 0) {
        data_8018f5a0->step_index_2 = 0xb;
    }
}
