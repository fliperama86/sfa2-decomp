/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002d57c_slot12;
extern Object *data_8002d574_slot12;
void advance_substep_and_play_side_sound_8_slot12(void);
void reset_hud_and_game_state_to_phase_4_slot12(void);

void check_active_flag_then_advance_or_reset_slot12(void) {
    if (game_state.active_flag & 0x80) {
        data_8002d574_slot12->active_count = 0;
        advance_substep_and_play_side_sound_8_slot12();
    } else if (game_state.side_bits_b == 0) {
        reset_hud_and_game_state_to_phase_4_slot12();
    }
}
