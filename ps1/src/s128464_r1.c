/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Both addresses sit inside or at the edge of game_state, but the original
   addresses them as separate globals (own lui each), not through the struct
   base, so members do not match. */
extern u16 data_80190464, data_8019046c;

void func_80128464(void) {
    if (game_state.field_ee == 0) {
        do {
            game_state.field_f0 = 0x1f;
            data_80190464 = 0x2c01;
            data_8019046c = 0x1f00;
            func_80119144(2, 4);
        } while (game_state.field_ee == 0);
    }
}
