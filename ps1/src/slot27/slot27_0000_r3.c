/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80029324_slot27;
void spawn_three_pool_objects_slot27(void);
void spawn_five_pool_objects_10_indexed_slot27(void);
void spawn_four_pool_objects_per_player_slot27(void);
void fill_rows_white_and_queue_jobs_slot27(void);
void restore_rows_from_backup_and_queue_jobs_slot27(void);

void set_flag_ff_and_advance_hud_step_2_slot27(void) {
    HudState *h = data_8018f5a0;
    data_80029324_slot27 = 0xff;
    h->step_index_2++;
}

void func_800104c4_slot27(void) {
    data_8018f5a0->step_index_2++;
    spawn_three_pool_objects_slot27();
    game_state.busy_flag = 0;
    game_state.field_09 = 0;
    game_state.countdown_2 = 0x2b;
}

void func_80010514_slot27(void) {
    int t = game_state.countdown_2 - 1;
    game_state.countdown_2 = t;
    if ((s16)t == 0) {
        data_8018f5a0->step_index_2++;
        spawn_five_pool_objects_10_indexed_slot27();
        fill_rows_white_and_queue_jobs_slot27();
        game_state.timer_or_bits = 8;
    }
}

void wait_timer_then_advance_substep_by_2_slot27(void) {
    int t = game_state.timer_or_bits - 1;
    game_state.timer_or_bits = t;
    if ((s16)t == 0) {
        HudState *h = data_8018f5a0;
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->substep_index += 2;
        spawn_four_pool_objects_per_player_slot27();
        restore_rows_from_backup_and_queue_jobs_slot27();
    }
}
