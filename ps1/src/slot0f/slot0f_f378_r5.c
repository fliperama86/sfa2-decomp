/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800e83e0_slot0f[])(GameState *state);
void func_800e4d10_slot0f(Object *obj);
void func_800e4d60_slot0f(Object *obj);
void func_800e0150_slot0f(Object *obj);
void func_800e483c_slot0f(void);
void func_800e4988_slot0f(void);

void func_800df8b0_slot0f(GameState *state) {
    func_800e4d10_slot0f((Object *)state);
}

void func_800df8d0_slot0f(GameState *state) {
    func_800e4d60_slot0f((Object *)state);
}

void func_800df8f0_slot0f(GameState *state) {
    func_800e0150_slot0f((Object *)state);
}

void func_800df910_slot0f(GameState *state) {
    data_800e83e0_slot0f[data_8018f5a0->field_4a](state);
}

void func_800df958_slot0f(GameState *state) {
    data_8018f5a0->field_4a++;
    state->field_c2 = 300;
    func_800e483c_slot0f();
}

void func_800df994_slot0f(GameState *state) {
    int t = state->field_c2 - 1;
    state->field_c2 = t;
    if ((s16)t == 0) {
        data_8018f5a0->field_4a = 0;
        data_8018f5a0->field_48++;
    }
    func_800e4988_slot0f();
}

void func_800df9ec_slot0f(GameState *state) {
    scratch_word_10 = 0;
    data_8018f5a0->field_48 = 0;
}
