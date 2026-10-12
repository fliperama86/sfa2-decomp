/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int is_pad_pressed_for_side_bits_or_mode(void) {
    u8 r = 0;
    if (game_state.mode != 0) {
        return is_any_enabled_pad_button_pressed();
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
