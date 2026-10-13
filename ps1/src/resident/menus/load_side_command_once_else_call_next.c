/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void load_side_command_once_else_call_next(GameState *state) {
    if (screen_state->pass_index == 0) {
        Object *object;
        screen_state->pass_index++;
        object = get_player_by_mode_id((Object *)state);
        state->object = object;
        load_command_6_for_side_and_wait(object->kind, object->side);
    } else {
        func_80010058(state);
    }
}

void call_func_ignoring_argument(GameState *unused) {
    func_80010b2c();
}

void func_8012126c(GameState *state) {
    if (screen_state->step_index == 0) {
        screen_state->step_index++;
        load_command_7_and_wait(state->audio_arg & 0x7f);
    } else {
        func_80013834();
    }
}

void func_801212c8(GameState *unused) {
    func_8001365c();
}

void func_801212e8(GameState *unused) {
    func_800136b0();
}

void load_focus_object_sound_then_set_phase_4(GameState *state) {
    if (screen_state->pass_index == 0) {
        screen_state->pass_index++;
        game_state.object = state->focus_object;
        load_command_6_for_side_and_wait(state->audio_arg, state->sound_side);
    } else {
        screen_state->phase_index = 4;
    }
}

void call_handler_by_step_index(GameState *state) {
    table_8016e760[screen_state->step_index](state);
}
