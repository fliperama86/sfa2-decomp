/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void loop_update_players_and_switch_thread(void) {
    GameState *g = &game_state;
    Object *left;

    ((HudBig *)screen_state)->field_48 = 0;
    ((HudBig *)screen_state)->field_4a = 0;
    ((HudBig *)screen_state)->field_4c = 0;
    ((HudBig *)screen_state)->step_count = 0;
loop:
    if (data_8019032d == 0) {
        left = &player_left;
        call_step_handler_by_step_index(g, left);
        call_step_handler_by_step_index(g, left + 1);
    }
    request_sound_command_and_switch_thread(1);
    goto loop;
}
