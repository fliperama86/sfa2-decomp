/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80029324_slot27;
void restore_rows_from_backup_and_queue_jobs_slot27(void);
void clear_rows_and_queue_jobs_slot27(void);
void fill_rows_white_and_queue_jobs_slot27(void);

void advance_hud_step_2_set_countdown_8_slot27(void) {
    if (game_state.busy_flag == 0) {
        data_8018f5a0->step_index_2++;
        game_state.countdown_2 = 8;
    }
}

void alternate_row_effect_until_countdown_2_expires_slot27(void) {
    u16 *c;
    int t;
    if (game_state.countdown_2 & 1) {
        restore_rows_from_backup_and_queue_jobs_slot27();
        clear_rows_and_queue_jobs_slot27();
    } else {
        restore_rows_from_backup_and_queue_jobs_slot27();
        fill_rows_white_and_queue_jobs_slot27();
    }
    c = &game_state.countdown_2;
    t = *c - 1;
    *c = t;
    if ((s16)t == 0) {
        data_8018f5a0->step_index_2++;
        data_80029324_slot27 = 0;
    }
}

void advance_hud_step_2_set_countdown_2_slot27(void) {
    data_8018f5a0->step_index_2++;
    game_state.countdown_2 = 2;
}
