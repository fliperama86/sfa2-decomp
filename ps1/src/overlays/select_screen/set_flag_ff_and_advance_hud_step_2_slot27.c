/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80029324_slot27;
void spawn_three_pool_objects_slot27(void);
void spawn_five_pool_objects_10_indexed_slot27(void);
void spawn_four_pool_objects_per_player_slot27(void);
void fill_rows_white_and_queue_jobs_slot27(void);
void restore_rows_from_backup_and_queue_jobs_slot27(void);

void set_flag_ff_and_advance_hud_step_2_slot27(void) {
    HudState *h = screen_state;
    data_80029324_slot27 = 0xff;
    h->step_index_2++;
}

void advance_step_2_spawn_three_and_set_countdown_slot27(void) {
    screen_state->step_index_2++;
    spawn_three_pool_objects_slot27();
    game_state.busy_flag = 0;
    game_state.field_09 = 0;
    game_state.countdown_2 = 0x2b;
}

void count_down_then_spawn_five_and_fill_rows_white_slot27(void) {
    int t = game_state.countdown_2 - 1;
    game_state.countdown_2 = t;
    if ((s16)t == 0) {
        screen_state->step_index_2++;
        spawn_five_pool_objects_10_indexed_slot27();
        fill_rows_white_and_queue_jobs_slot27();
        game_state.timer_or_bits = 8;
    }
}

void wait_timer_then_advance_substep_by_2_slot27(void) {
    int t = game_state.timer_or_bits - 1;
    game_state.timer_or_bits = t;
    if ((s16)t == 0) {
        HudState *h = screen_state;
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->substep_index += 2;
        spawn_four_pool_objects_per_player_slot27();
        restore_rows_from_backup_and_queue_jobs_slot27();
    }
}
