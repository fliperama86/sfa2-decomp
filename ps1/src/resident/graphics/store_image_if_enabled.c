/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void store_image_if_enabled(Rect r, u8 b, u8 a) {
    if (game_state.field_65 == 0 && game_state.field_74 == 0 && white_fill_active == 0) {
        func_80158028(&r, data_801a27e4 + (a << 10) + (b << 5));
    }
}
