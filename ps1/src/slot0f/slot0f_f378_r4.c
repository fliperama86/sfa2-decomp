/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e83c8_slot0f[])(GameState *state);
void func_800dfa04_slot0f(GameState *state);

void func_800df7ec_slot0f(GameState *state) {
    state->wait_flag = 0;
    data_8018f5a0->phase_index = 0;
    data_8018f5a0->state_index++;
}

void func_800df810_slot0f(GameState *state) {
    func_801192bc(10);
    scratch_word_00 = -1;
    scratch_word_04 = 1;
    scratch_word_10 = 1;
    func_80150cd0(1);
}

void func_800df858_slot0f(GameState *state) {
    data_800e83c8_slot0f[data_8018f5a0->state_index](state);
    func_800dfa04_slot0f(state);
}
