/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void send_fade_command_1c01_and_wait(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    game_state.wait_flag = 0x1f;
    *(u16 *)data_80190464 = 0x1c01;
    *(s16 *)data_8019046c = 0;
    set_voice_thread_entry_and_open(2, 4);
    while (game_state.fade_busy != 0) {
        request_sound_command_and_switch_thread(1);
    }
}

void start_fade_command_2a01_0800_if_idle(void) {
    if (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        *(u16 *)data_80190464 = 0x2a01;
        *(s16 *)data_8019046c = 0x800;
        set_voice_thread_entry_and_open(2, 4);
    }
}
