/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f3b8(int a, int b) {
    start_cd_track_from_table(&ctl_80190948, a, b);
    while (!is_cd_ctl_mode_3()) {
        request_sound_command_and_switch_thread(1);
    }
}

void func_8014f408(int a, int b) {
    GameState *g = &game_state;
    start_cd_track_from_table(&ctl_80190948, a, b);
    while (!is_cd_ctl_mode_3()) {
        if (g->loop_mode == 2) {
            data_801ac620 |= 0x8000;
        }
        update_cd_control();
        func_8015fb30(0);
    }
    data_801ac620 &= 0x1f;
}

int is_cd_ctl_mode_3(void) {
    return ctl_80190948.mode == 3;
}
