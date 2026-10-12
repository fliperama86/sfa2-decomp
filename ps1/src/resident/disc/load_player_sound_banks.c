/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void load_player_sound_banks(int a, int b) {
    close_vab_and_sep(0);
    close_vab_and_sep(1);
    if (a == b) {
        if (b == 0xd) {
            load_cd_entry_and_reset_address_words_a();
        } else if (b == 0x10) {
            load_cd_entry_and_reset_address_words_b();
        } else if (b == 0x11 || b == 0x13) {
            load_cd_entry_and_reset_address_words_c();
        } else goto other;
    } else {
other:
        if ((a == 0x11 || a == 0x13) && (b == 0x11 || b == 0x13)) {
            load_cd_entry_and_reset_address_words_c();
        } else {
            start_cd_track_pair_set_load_addresses(a, b);
        }
    }
    player_left.field_90 = (void *)data_801a27d8;
    player_right.field_90 = (void *)data_801a27dc;
}
