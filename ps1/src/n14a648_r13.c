/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150f40(GameState *state) {
    if (state->field_225 == 0) {
        data_8018f5a0->field_48++;
        state->field_c2 = 0x1f;
    }
}
