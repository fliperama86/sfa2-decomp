/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void reset_hud_indices_and_state_counters_slot0f(GameState *state, int flag);

void check_buttons_and_call_reset_slot0f(GameState *state) {
    if (((data_801a696a | data_801a6976) & 0x820) != 0) {
        reset_hud_indices_and_state_counters_slot0f(state, 1);
    }
}

void reset_hud_indices_and_state_counters_slot0f(GameState *state, int flag) {
    HudState *hud = data_8018f5a0;
    scratch_word_10 = 0;
    hud->state_index = 0;
    hud->phase_index = 0;
    hud->step_index = 0;
    hud->substep_index = 0;
    hud->step_index_2 = 0;
    hud->pass_index = 0;
    state->step_counter = 0;
    state->step_delay_timer = 0;
    state->countdown_2 = 0;
    state->timer_or_bits = 0;
    state->countdown_timer_2 = 0;
    state->field_cc = 0;
    func_8011eb14();
    stop_all_keys_clear_table();
    stop_sep_by_word(0x200);
    set_menu_command_by_code(4, 0);
    if (flag != 0) {
        play_sound_at(0, 0, 0x205);
    }
    func_80119198(6);
}
