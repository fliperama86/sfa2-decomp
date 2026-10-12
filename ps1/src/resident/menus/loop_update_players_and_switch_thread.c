/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void loop_update_players_and_switch_thread(void) {
    GameState *g = &game_state;
    Object *left;

    ((HudBig *)data_8018f5a0)->field_48 = 0;
    ((HudBig *)data_8018f5a0)->field_4a = 0;
    ((HudBig *)data_8018f5a0)->field_4c = 0;
    ((HudBig *)data_8018f5a0)->step_count = 0;
loop:
    if (data_8019032d == 0) {
        left = &player_left;
        call_step_handler_by_step_index(g, left);
        call_step_handler_by_step_index(g, left + 1);
    }
    request_sound_command_and_switch_thread(1);
    goto loop;
}
