/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121ef0(Object *unused) {
    if (game_state.field_06 == 0) {
        game_state.field_ca = (s16)game_state.field_ca - 1;
        if ((s16)game_state.field_ca < 0) {
            data_8018f5a0->field_50 = 0xd;
            func_80124ecc();
        }
    }
}

void func_80121f48(Object *unused) {
    if (game_state.field_06 == 0) {
        data_8018f5a0->field_50 = 0xb;
    }
}
