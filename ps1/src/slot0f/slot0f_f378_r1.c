/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_800df520_slot0f(GameState *state);
void func_800df858_slot0f(GameState *state);
void func_800dfa40_slot0f(GameState *state, int a);

void func_800df378_slot0f(void) {
    GameState *s = &game_state;
    game_state.field_01 = 1;
    game_state.field_dc = 0x1b0e;
    data_8018f5a0->field_48 = 0;
    data_8018f5a0->field_4a = 0;
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    game_state.field_c2 = 0;
    game_state.field_c4 = 0;
    game_state.field_c6 = 0;
    game_state.field_c8 = 0;
    game_state.field_ca = 0;
    game_state.field_cc = 0;
    game_state.field_40 = 0;
    game_state.field_08 = 0;
    game_state.field_17 = 0;
    game_state.field_07 = 0;
    game_state.field_19 = 0;
    game_state.field_ee = 0;
    game_state.field_f0 = 0;
    game_state.field_f1 = 0;
    game_state.field_09 = 0;
    game_state.field_2c = 0;
    game_state.field_158 = 0;
    game_state.field_1a = 0;
    game_state.field_ce = 0;
    game_state.field_d0 = 0;
    game_state.field_d2 = 0;
    game_state.field_d4 = 0;
    game_state.field_d6 = 0;
    game_state.field_d8 = 0;
    game_state.field_2d = 7;
    func_80138358(s);
    func_801192bc(1);
    data_801a89f0 = 0;
    data_801ac61c = 0;
    {
        u32 *p = (u32 *)0x1f80000c;
        if (*p != 0) {
            *p = 0;
            func_800dfa40_slot0f(s, 0);
        }
    }
    while (1) {
        if (scratch_word_10 == 0) {
            func_800df520_slot0f(s);
        } else {
            func_800df858_slot0f(s);
        }
        func_801192bc(1);
    }
}
