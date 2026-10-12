/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void repeat_fade_command_2c01_until_busy(void) {
    if (game_state.fade_busy == 0) {
        do {
            game_state.wait_flag = 0x1f;
            *(u16 *)data_80190464 = 0x2c01;
            *(u16 *)data_8019046c = 0x1f00;
            set_voice_thread_entry_and_open(2, 4);
        } while (game_state.fade_busy == 0);
    }
}
