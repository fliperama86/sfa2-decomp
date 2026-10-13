/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051a90_slot28;

void start_countdown_180_when_step_flags_negative_7_slot28(Object *obj) {
    if ((s16)data_80051a90_slot28.p->step_flags < 0) {
        screen_state->countdown_timer = 0xb4;
        screen_state->pass_index = screen_state->pass_index + 1;
    }
}

void count_down_then_fade_and_advance_pass_7_slot28(Object *obj) {
    HudState *h = screen_state;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        screen_state->pass_index += 1;
    }
}

void advance_substep_when_wait_flag_clear_7_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        screen_state->substep_index += 1;
    }
}
