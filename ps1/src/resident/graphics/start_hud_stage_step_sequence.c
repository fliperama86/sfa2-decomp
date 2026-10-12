/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void start_hud_stage_step_sequence(GameState *state) {
    ((Hud *)data_8018f5a0)->stage++;
    state->step_delay_timer = 1;
    state->step_counter = 0;
    state->wait_flag = 1;
    update_fade_step_counter_up(state);
}

void update_fade_step_counter_up(GameState *state) {
    int n = state->step_delay_timer - 1;
    state->step_delay_timer = n;
    if ((s16)n == 0) {
        state->step_delay_timer = 1;
        scale_palette_rows_from_backup_bank(4, 0x20, state->step_counter, 0);
        queue_image_upload_job(4, 7);
        state->step_counter++;
        if ((s16)state->step_counter >= 0x20) {
            ((Hud *)data_8018f5a0)->stage++;
            state->wait_flag = 0x80;
        }
    }
}
