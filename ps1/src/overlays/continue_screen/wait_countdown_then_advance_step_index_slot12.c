/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void reset_to_phase_7_when_no_side_bits_slot12(void);

void wait_countdown_then_advance_step_index_slot12(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    if (is_any_enabled_pad_button_pressed() == 0) {
        u16 *t = &game_state.countdown_2;
        int v = *t - 1;

        *t = v;
        if ((s16)v >= 0) {
            return;
        }
    }
    screen_state->step_index++;
    set_menu_command_by_code(6, 2);
    reset_to_phase_7_when_no_side_bits_slot12();
}
