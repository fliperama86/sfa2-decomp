/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void spawn_four_pool_objects_per_player_slot27(void);
void fill_rows_white_and_queue_jobs_slot27(void);
void restore_rows_from_backup_and_queue_jobs_slot27(void);
void clear_rows_and_queue_jobs_slot27(void);

void alternate_row_effect_until_timer_expires_slot27(void) {
    u16 *p;
    int t;
    if (game_state.timer_or_bits & 1) {
        restore_rows_from_backup_and_queue_jobs_slot27();
        clear_rows_and_queue_jobs_slot27();
    } else {
        restore_rows_from_backup_and_queue_jobs_slot27();
        fill_rows_white_and_queue_jobs_slot27();
    }
    p = &game_state.timer_or_bits;
    t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        data_8018f5a0->step_index_2++;
        spawn_four_pool_objects_per_player_slot27();
        game_state.busy_flag = 0xff;
    }
}

void advance_step_2_and_restore_rows_from_backup_slot27(void) {
    data_8018f5a0->step_index_2++;
    restore_rows_from_backup_and_queue_jobs_slot27();
}

void advance_hud_substep_when_not_busy_slot27(void) {
    if (game_state.busy_flag == 0) {
        HudState *h = data_8018f5a0;
        h->substep_index++;
        h->step_index_2 = 0;
        h->pass_index = 0;
        game_state.field_09 = 0;
    }
}
