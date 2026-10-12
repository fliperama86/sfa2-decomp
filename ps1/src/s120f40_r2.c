/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801211dc(GameState *state) {
    if (data_8018f5a0->pass_index == 0) {
        Object *object;
        data_8018f5a0->pass_index++;
        object = func_80125060((Object *)state);
        state->object = object;
        func_8014eef8(object->kind, object->side);
    } else {
        func_80010058(state);
    }
}

void func_8012124c(GameState *unused) {
    func_80010b2c();
}

void func_8012126c(GameState *state) {
    if (data_8018f5a0->step_index == 0) {
        data_8018f5a0->step_index++;
        func_8014efa8(state->audio_arg & 0x7f);
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

void func_80121308(GameState *state) {
    if (data_8018f5a0->pass_index == 0) {
        data_8018f5a0->pass_index++;
        game_state.object = state->focus_object;
        func_8014eef8(state->audio_arg, state->sound_side);
    } else {
        data_8018f5a0->phase_index = 4;
    }
}

void func_80121378(GameState *state) {
    table_8016e760[data_8018f5a0->step_index](state);
}
