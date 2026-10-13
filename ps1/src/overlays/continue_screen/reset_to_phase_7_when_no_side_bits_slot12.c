/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void reset_to_phase_7_when_no_side_bits_slot12(void) {
    HudState *h;

    if (game_state.side_bits == 0) {
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            queue_player_status_text_items();
            request_sound_command_and_switch_thread(1);
        }
        start_fade_mode_1_and_wait();
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            queue_player_status_text_items();
            request_sound_command_and_switch_thread(1);
        }
        h = screen_state;
        h->phase_index = 7;
        h->step_index = 0;
        game_state.countdown_2 = 0;
        h->substep_index = 0;
        game_state.timer_or_bits = 0;
        h->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        h->pass_index = 0;
        game_state.field_cc = 0;
        clear_arrays_and_init_pools_and_sprite_tables();
        close_voice_thread(1);
    }
}
