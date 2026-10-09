/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80128464(void) {
    if (game_state.field_ee == 0) {
        do {
            game_state.field_f0 = 0x1f;
            *(u16 *)data_80190464 = 0x2c01;
            *(u16 *)data_8019046c = 0x1f00;
            func_80119144(2, 4);
        } while (game_state.field_ee == 0);
    }
}
