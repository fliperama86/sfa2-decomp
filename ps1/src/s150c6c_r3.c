/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150e4c(GameState *state) {
    ((Hud *)data_8018f5a0)->field_48++;
    state->field_c4 = 1;
    state->field_c2 = 0;
    state->field_f0 = 1;
    func_80150e94(state);
}

void func_80150e94(GameState *state) {
    int n = state->field_c4 - 1;
    state->field_c4 = n;
    if ((s16)n == 0) {
        state->field_c4 = 1;
        func_80125dc0(4, 0x20, state->field_c2, 0);
        func_80137220(4, 7);
        state->field_c2++;
        if ((s16)state->field_c2 >= 0x20) {
            ((Hud *)data_8018f5a0)->field_48++;
            state->field_f0 = 0x80;
        }
    }
}
