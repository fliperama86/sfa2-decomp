/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e8404_slot0f[])(GameState *, Menu *);
void reset_hud_state_and_sound_by_arg_slot0f(Object *obj, int a);
void call_handler_by_step_index_slot0f(void);
void loop_menu_object_until_loop_flag_clear_slot0f(void);

void call_menu_handler_by_mode_index_slot0f(GameState *state, Menu *menu) {
    data_800e8404_slot0f[(s8)menu->mode_index](state, menu);
}

void start_mode_with_menu_side_bits_slot0f(GameState *state, Menu *menu) {
    state->side_bits = menu->active_flag;
    state->versus_flag = 0;
    state->mode3_active_flag = 0;
    reset_hud_state_and_sound_by_arg_slot0f((Object *)state, 0);
}

void start_mode3_with_both_sides_slot0f(GameState *state, Menu *menu) {
    state->side_bits = 3;
    state->mode3_active_flag = 1;
    state->versus_flag = 0;
    reset_hud_state_and_sound_by_arg_slot0f((Object *)state, 2);
}

void start_versus_with_menu_side_bits_slot0f(GameState *state, Menu *menu) {
    state->side_bits = menu->active_flag;
    state->mode3_active_flag = 0;
    state->versus_flag = 1;
    reset_hud_state_and_sound_by_arg_slot0f((Object *)state, 2);
}

void call_step_handler_by_step_index_slot0f(GameState *state, Menu *menu) {
    call_handler_by_step_index_slot0f();
}

void decrement_phase_and_call_loop_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->phase_index--;
    loop_menu_object_until_loop_flag_clear_slot0f();
}
