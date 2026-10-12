/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e83a8_slot0f[])(GameState *state);
extern void (*data_800e83b4_slot0f[])(GameState *state);
void func_800df654_slot0f(GameState *state);
void func_800dfa04_slot0f(GameState *state);
void func_800e0784_slot0f(int a);

void func_800df520_slot0f(GameState *state) {
    data_800e83a8_slot0f[data_8018f5a0->state_index](state);
    if (data_8018f5a0->state_index == 1) {
        func_800dfa04_slot0f(state);
    }
}

void func_800df594_slot0f(GameState *state) {
    data_800e83b4_slot0f[data_8018f5a0->phase_index](state);
    func_800e0784_slot0f(2);
    func_800e0784_slot0f(1);
    if ((s16)state->field_cc != 0) {
        func_800e0784_slot0f(3);
    }
}

void func_800df60c_slot0f(GameState *state) {
    data_8018f5a0->phase_index++;
    state->step_delay_timer = 1;
    state->step_counter = 0;
    state->wait_flag = 1;
    func_800df654_slot0f(state);
}
