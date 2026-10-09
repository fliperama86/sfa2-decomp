/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_801252f0(void) {
    u8 r = 0;
    if (game_state.mode != 0) {
        return func_80125268();
    }
    if (game_state.field_17 & 1) {
        if (func_8012543c()) {
            r = 1;
        }
    }
    if (game_state.field_17 & 2) {
        if (func_80125454()) {
            r |= 1;
        }
    }
    return r;
}
