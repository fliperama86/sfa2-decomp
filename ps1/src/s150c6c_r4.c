/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150f7c(GameState *state) {
    int n = state->field_c4 - 1;
    state->field_c4 = n;
    if ((s16)n == 0) {
        int m;
        state->field_c4 = 1;
        func_80125dc0(4, 0x20, state->field_c2, 0);
        func_80137220(4, 7);
        m = state->field_c2 - 1;
        state->field_c2 = m;
        if ((s16)m < 0) {
            ((Hud *)data_8018f5a0)->field_48++;
        }
    }
}
