/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residuals: close_sounds_and_load_command_4 (original keeps game_state base in a saved
 * register from before the third close_vab_and_sep call, and tests/clears through
 * it; tried a local g set at top, set mid-function, ternary and if/else for
 * the argument). load_command_8_for_player_kind_and_stage (original derives both player addresses from
 * the field_cd address in one register; tried if/else, ternary, p++,
 * default-then-override). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void load_command_5_for_kind_and_wait(int a) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    set_voice_thread_entry_and_open(2, 7);
    request_sound_command_and_switch_thread(1);
    start_cd_track_wait_with_thread_switch(5, a);
    start_cd_track_wait_with_thread_switch(5, game_state.kind_index + 0x15);
    g->loop_mode = 0;
    while (g->wait_flag) {
        request_sound_command_and_switch_thread(1);
    }
}

void load_command_6_for_side_and_wait(int a, int b) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    set_voice_thread_entry_and_open(2, 7);
    request_sound_command_and_switch_thread(1);
    b ^= 1;
    close_vab_and_sep(b);
    start_cd_track_wait_with_thread_switch(6, b * 21 + a);
    g->loop_mode = 0;
    while (g->wait_flag) {
        request_sound_command_and_switch_thread(1);
    }
}

void load_command_7_and_wait(int a) {
    GameState *g = &game_state;
    g->loop_mode = 1;
    set_voice_thread_entry_and_open(2, 7);
    request_sound_command_and_switch_thread(1);
    close_vab_and_sep(3);
    start_cd_track_wait_with_thread_switch(7, a);
    g->loop_mode = 0;
    while (g->wait_flag) {
        request_sound_command_and_switch_thread(1);
    }
}
