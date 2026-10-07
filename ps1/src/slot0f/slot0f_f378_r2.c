/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800e83a8_slot0f[])(GameState *state);
extern void (*data_800e83b4_slot0f[])(GameState *state);
void func_800df654_slot0f(GameState *state);
void func_800dfa04_slot0f(GameState *state);
void func_800e0784_slot0f(int a);

void func_800df520_slot0f(GameState *state) {
    data_800e83a8_slot0f[data_8018f5a0->field_48](state);
    if (data_8018f5a0->field_48 == 1) {
        func_800dfa04_slot0f(state);
    }
}

void func_800df594_slot0f(GameState *state) {
    data_800e83b4_slot0f[data_8018f5a0->field_4a](state);
    func_800e0784_slot0f(2);
    func_800e0784_slot0f(1);
    if ((s16)state->field_cc != 0) {
        func_800e0784_slot0f(3);
    }
}

void func_800df60c_slot0f(GameState *state) {
    data_8018f5a0->field_4a++;
    state->field_c4 = 1;
    state->field_c2 = 0;
    state->field_f0 = 1;
    func_800df654_slot0f(state);
}
