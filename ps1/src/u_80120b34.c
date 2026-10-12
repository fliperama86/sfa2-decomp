/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void init_hud_indices_and_loop_game_state_updates(void) {
    GameState *g = &game_state;
    data_801ac61c = 0;
    data_801a89f0 = 0;
    data_80190568 = 0;
    data_8018f5a0->state_index = 0;
    data_8018f5a0->phase_index = 0;
    data_8018f5a0->step_index = 0;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
    game_state.fade_busy = 0;
    game_state.wait_flag = 0;
    request_sound_command_and_switch_thread(1);
    for (;;) {
        g->loop_counter++;
        g->pass_flag = 0xff;
        update_pending_flag_from_table_bit(g);
        update_player_inputs_and_handlers();
        g->field_04 = 0;
        call_handler_by_stage_0_1(g);
        if (g->pass_flag == 0 || g->pending_flag == 0) {
            request_sound_command_and_switch_thread(1);
        }
    }
}
