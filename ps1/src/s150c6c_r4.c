/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_fade_step_counter_down(GameState *state) {
    int n = state->step_delay_timer - 1;
    state->step_delay_timer = n;
    if ((s16)n == 0) {
        int m;
        state->step_delay_timer = 1;
        scale_palette_rows_from_backup_bank(4, 0x20, state->step_counter, 0);
        queue_image_upload_job(4, 7);
        m = state->step_counter - 1;
        state->step_counter = m;
        if ((s16)m < 0) {
            ((Hud *)data_8018f5a0)->stage++;
        }
    }
}
