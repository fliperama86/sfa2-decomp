/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e83a8_slot0f[])(GameState *state);
extern void (*data_800e83b4_slot0f[])(GameState *state);
void fade_palette_in_step_by_step_slot0f(GameState *state);
void check_buttons_and_call_reset_slot0f(GameState *state);
void queue_item_from_table_by_index_slot0f(int a);

void call_slot0f_state_handler_a_slot0f(GameState *state) {
    data_800e83a8_slot0f[screen_state->state_index](state);
    if (screen_state->state_index == 1) {
        check_buttons_and_call_reset_slot0f(state);
    }
}

void call_phase_handler_and_queue_entries_slot0f(GameState *state) {
    data_800e83b4_slot0f[screen_state->phase_index](state);
    queue_item_from_table_by_index_slot0f(2);
    queue_item_from_table_by_index_slot0f(1);
    if ((s16)state->field_cc != 0) {
        queue_item_from_table_by_index_slot0f(3);
    }
}

void advance_phase_and_start_palette_fade_slot0f(GameState *state) {
    screen_state->phase_index++;
    state->step_delay_timer = 1;
    state->step_counter = 0;
    state->wait_flag = 1;
    fade_palette_in_step_by_step_slot0f(state);
}
