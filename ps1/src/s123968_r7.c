/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80125268(void) {
    u8 result = 0;
    if (game_state.mode & 1) {
        result = func_8012543c() != 0;
    }
    if (game_state.mode & 2) {
        if (func_80125454()) {
            result |= 1;
        }
    }
    return result;
}
