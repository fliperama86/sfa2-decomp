/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80125394(void) {
    u8 result = 0;
    if (game_state.mode != 3) {
        return func_80125268();
    }
    if (game_state.field_4c & 1) {
        if (func_8012543c()) {
            result = 1;
        }
    }
    if (game_state.field_4c & 2) {
        if (func_80125454()) {
            result |= 1;
        }
    }
    return result;
}

u8 func_8012543c(void) {
    return (data_801a696a & 0x9f0) != 0;
}

u8 func_80125454(void) {
    return (data_801a6976 & 0x9f0) != 0;
}

void func_8012546c(void) {
    if (game_state.field_78->field_cd == 0 && game_state.field_0c == 0 && (game_state.field_45 & 1)) {
        table_8016e99c[game_state.field_138]();
    }
}
