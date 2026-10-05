/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801a27e4[];

void func_80137640(Rect r, u8 b, u8 a) {
    if (game_state.field_65 == 0 && game_state.field_74 == 0 && data_8018f598 == 0) {
        func_80158028(&r, data_801a27e4 + (a << 10) + (b << 5));
    }
}
