/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800dfa40_slot0f(GameState *state, int flag);

void func_800dfa04_slot0f(GameState *state) {
    if (((data_801a696a | data_801a6976) & 0x820) != 0) {
        func_800dfa40_slot0f(state, 1);
    }
}

void func_800dfa40_slot0f(GameState *state, int flag) {
    HudState *hud = data_8018f5a0;
    scratch_word_10 = 0;
    hud->field_48 = 0;
    hud->field_4a = 0;
    hud->field_4c = 0;
    hud->field_4e = 0;
    hud->field_50 = 0;
    hud->field_52 = 0;
    state->field_c2 = 0;
    state->field_c4 = 0;
    state->field_c6 = 0;
    state->field_c8 = 0;
    state->field_ca = 0;
    state->field_cc = 0;
    func_8011eb14();
    func_80120408();
    func_80120498(0x200);
    func_8014f4d4(4, 0);
    if (flag != 0) {
        func_80120554(0, 0, 0x205);
    }
    func_80119198(6);
}
