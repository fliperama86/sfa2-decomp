/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Menu data_800f8574_slot0f;
extern void (*data_800e83e8_slot0f[])(GameState *, Menu *);
extern void (*data_800e83f0_slot0f[])(GameState *, Menu *);
extern void (*data_800e83fc_slot0f[])(GameState *, Menu *);

void loop_call_menu_state_handler_forever_slot0f(void) {
    GameState *state = &game_state;
    Menu *menu = &data_800f8574_slot0f;
    data_800f8574_slot0f.active_flag = 0;
    menu->mode_index = 0;
    for (;;) {
        data_800e83e8_slot0f[screen_state->state_index](state, menu);
        get_random_byte();
        request_sound_command_and_switch_thread(1);
    }
}

void increment_state_index_slot0f(GameState *state, Menu *menu) {
    screen_state->state_index++;
}

void call_menu_handler_by_phase_index_slot0f(GameState *state, Menu *menu) {
    data_800e83f0_slot0f[screen_state->phase_index](state, menu);
}

void call_menu_handler_by_step_index_slot0f(GameState *state, Menu *menu) {
    data_800e83fc_slot0f[screen_state->step_index](state, menu);
}
