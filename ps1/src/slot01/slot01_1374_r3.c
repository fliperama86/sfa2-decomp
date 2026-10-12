/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void reset_hud_state_and_restore_kind_index_slot01(GameState *g);

void increment_hud_substep_if_byte_set_slot01(void) {
    if (*(u8 *)&data_801ae066 != 0) {
        data_8018f5a0->substep_index++;
    }
}

void fade_out_and_reset_hud_when_not_busy_slot01(void) {
    if (game_state.busy_flag == 0) {
        func_8011eae4();
        while (game_state.wait_flag != 0) {
            request_sound_command_and_switch_thread(1);
        }
        start_fade_mode_1_and_wait();
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            request_sound_command_and_switch_thread(1);
        }
        reset_hud_state_and_restore_kind_index_slot01(&game_state);
    }
}
