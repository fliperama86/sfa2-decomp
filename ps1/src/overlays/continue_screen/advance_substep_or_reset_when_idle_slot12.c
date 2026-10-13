/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern int data_8002d57c_slot12;
extern Object *data_8002d574_slot12;
void advance_substep_and_play_side_sound_8_slot12(void);
void reset_hud_and_game_state_to_phase_4_slot12(void);

void advance_substep_or_reset_when_idle_slot12(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    if (game_state.wait_flag == 0) {
        data_8002d57c_slot12 = 1;
        if (game_state.busy_flag == 0) {
            if (game_state.mode_copy_2 != 0) {
                screen_state->substep_index++;
                game_state.field_09 = 0;
                game_state.state_2c_flag = 0;
            } else {
                reset_hud_and_game_state_to_phase_4_slot12();
            }
        }
    }
}
