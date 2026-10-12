/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_menu_command_one_and_wait_pending(s16 value) {
    Sprite *s = &data_80190948;
    GameState *g = &game_state;
    g->loop_mode = 0;
    set_menu_command_by_code(1, value);
    while (s->pending == 0 || g->wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
}
