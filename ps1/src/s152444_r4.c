/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void switch_to_next_table_entry_and_wait(void) {
    GameState *state = &game_state;

    func_80136dc4();
    game_state.loop_mode = 1;
    load_table_entry_or_wait_then_close_vab(state);
    *(u32 *)0x1f800014 = *(u32 *)0x1f800014 + 1;
    if (*(u32 *)0x1f800014 == 6) {
        *(u32 *)0x1f800014 = 0;
    }
    game_state.loop_mode = 0;
    if (game_state.wait_flag != 0) {
        do {
            request_sound_command_and_switch_thread(1);
        } while (state->wait_flag != 0);
    }
    set_snd_ctl_thread_entry_and_close(3);
}
