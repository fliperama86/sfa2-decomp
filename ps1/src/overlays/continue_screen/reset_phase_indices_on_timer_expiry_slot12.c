/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void play_sounds_at_countdown_timer_values_slot12(void);

void reset_phase_indices_on_timer_expiry_slot12(void) {
    u16 *p = &game_state.timer_or_bits;
    int t = *p - 1;
    *p = t;
    if ((s16)t < 0) {
        game_state.status_flags = 0;
        game_state.active_flag = 0;
        game_state.state_2c_flag = 0;
        game_state.field_4f = 0;
        game_state.field_09 = 1;
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            play_sounds_at_countdown_timer_values_slot12();
            request_sound_command_and_switch_thread(1);
        }
        start_fade_mode_1_and_wait();
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            play_sounds_at_countdown_timer_values_slot12();
            request_sound_command_and_switch_thread(1);
        }
        data_8018f5a0->phase_index = 0;
        game_state.step_delay_timer = 0;
        data_8018f5a0->step_index = 0;
        game_state.countdown_2 = 0;
        data_8018f5a0->substep_index = 0;
        game_state.timer_or_bits = 0;
        data_8018f5a0->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        data_8018f5a0->pass_index = 0;
        game_state.field_cc = 0;
        clear_arrays_and_init_pools_and_sprite_tables();
    }
}
