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


void start_cd_track_pair_set_load_addresses(int a, int b) {
    start_cd_track_wait_with_thread_switch(1, a * 2);
    start_cd_track_wait_with_thread_switch(1, b * 2 | 1);
    data_801a27d8 = 0x80010000;
    if (a == b) {
        data_801a27dc = 0x80010000;
    } else {
        data_801a27dc = 0x800fb100 - data_8017d918[b];
    }
}

void load_cd_entry_and_reset_address_words_a(void) {
    start_cd_track_wait_with_thread_switch(1, 0x2a);
    data_801a27d8 = data_801a27dc = 0x80010000;
}

void load_cd_entry_and_reset_address_words_b(void) {
    start_cd_track_wait_with_thread_switch(1, 0x2b);
    data_801a27d8 = data_801a27dc = 0x80010000;
}

void load_cd_entry_and_reset_address_words_c(void) {
    start_cd_track_wait_with_thread_switch(1, 0x2c);
    data_801a27d8 = data_801a27dc = 0x80010000;
}
