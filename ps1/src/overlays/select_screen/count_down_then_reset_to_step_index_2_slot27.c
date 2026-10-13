/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void restore_rows_from_backup_and_queue_jobs_slot27(void);

void count_down_then_reset_to_step_index_2_slot27(void) {
    u16 *c = &game_state.countdown_2;
    int t = *c - 1;
    *c = t;
    if ((s16)t == 0) {
        clear_arrays_and_init_pools_and_sprite_tables();
        restore_rows_from_backup_and_queue_jobs_slot27();
        screen_state->step_index = 2;
        screen_state->substep_index = 0;
        screen_state->step_index_2 = 0;
        screen_state->pass_index = 0;
        screen_state->field_54 = 0;
    }
}

void save_player_select_values_slot27(void) {
    Object *p;
    u8 k;
    p = &player_left;
    k = player_left.kind;
    if (game_state.field_1c != 0 || player_left.saved_side != k) {
        player_left.field_ef = 1;
    }
    if (p->mode_flag_2 == 0) {
        p->saved_side = k;
        p->select_value_2 = p->field_d8;
        p->select_value_1 = p->field_d4;
    }
    p = &player_right;
    k = player_right.kind;
    if (game_state.field_1c != 0 || player_right.saved_side != k) {
        player_right.field_ef = 1;
    }
    if (p->mode_flag_2 == 0) {
        p->saved_side = k;
        p->select_value_2 = p->field_d8;
        p->select_value_1 = p->field_d4;
    }
}
