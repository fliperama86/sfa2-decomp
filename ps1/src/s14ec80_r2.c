/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void start_cd_track_wait_with_thread_switch(int a, int b) {
    start_cd_track_from_table(&ctl_80190948, a, b);
    while (!is_cd_ctl_mode_3()) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_cd_track_and_wait_updating_control(int a, int b) {
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
