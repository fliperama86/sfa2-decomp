/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80011358_slot12(void) {
    while (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        *(u16 *)data_80190464 = 0x2404;
        *(u16 *)data_8019046c = 0x1f00;
        set_voice_thread_entry_and_open(2, 4);
    }
}
