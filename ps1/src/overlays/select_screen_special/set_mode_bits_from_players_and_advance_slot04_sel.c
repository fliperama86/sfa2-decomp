/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b9d24_slot04_sel;

void set_mode_bits_from_players_and_advance_slot04_sel(void) {
    Object *r = &player_right;
    game_state.field_09 = 0xff;
    if (player_left.mode_flag_2 == 0) {
        game_state.mode |= 1;
    } else {
        game_state.mode &= 0xfe;
    }
    if (r->mode_flag_2 == 0) {
        game_state.mode |= 2;
    } else {
        game_state.mode &= 0xfd;
    }
    select_kind_for_second_player((Select *)&game_state);
    data_801b9d24_slot04_sel = 0x40;
    screen_state->step_index_2++;
}
