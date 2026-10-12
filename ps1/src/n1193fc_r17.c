/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_801252f0(void) {
    u8 r = 0;
    if (game_state.mode != 0) {
        return func_80125268();
    }
    if (game_state.side_bits & 1) {
        if (is_pad_button_pressed()) {
            r = 1;
        }
    }
    if (game_state.side_bits & 2) {
        if (is_pad_button_pressed_2()) {
            r |= 1;
        }
    }
    return r;
}
