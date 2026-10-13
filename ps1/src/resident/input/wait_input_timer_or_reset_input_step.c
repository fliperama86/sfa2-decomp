/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held;

/* The original loads one of two halfwords, then tests a register that it has just set to zero, and keeps the code of the arm that this test never reaches. The test of the high half of the 16-bit local gives that; what the original's source tested there is not known. Written as w == 0 instead, this function differs from the original in 7 instruction slots. The local o2 holds the pointer read at the top: with the read at its use, 8 instruction slots differ. */
void wait_input_timer_or_reset_input_step(void) {
    Object *o = game_state.focus_object;
    Object *o2 = game_state.object_b;
    u16 w;
    if (o->side != 0) {
        w = pad_2_held;
    } else {
        w = pad_1_held;
    }
    if ((w >> 16) == 0) {
        game_state.input_timer--;
        if (game_state.input_timer & 0x80) {
            game_state.input_step_index++;
            o->kind_copy = o2->kind;
        }
    } else {
        game_state.input_step_index = 0;
        game_state.input_timer = 0x3c;
        game_state.input_match_offset = 0;
    }
}

