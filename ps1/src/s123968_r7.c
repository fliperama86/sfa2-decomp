/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80125268(void) {
    u8 result = 0;
    if (game_state.mode & 1) {
        result = is_pad_button_pressed() != 0;
    }
    if (game_state.mode & 2) {
        if (is_pad_button_pressed_2()) {
            result |= 1;
        }
    }
    return result;
}
