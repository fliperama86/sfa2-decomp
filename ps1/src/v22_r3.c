/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

/* Exact: the earlier candidate matches once the externs it needs are declared. */
void update_player_inputs_and_pick_handler(void) {
    game_state.config = &game_state_second;
    if (game_state.config->pause_flag_b == 0) {
        player_left.prev_input_bits_2 = player_left.input_bits_2;
        player_right.prev_input_bits_2 = player_right.input_bits_2;
        player_left.prev_input_word = player_left.input_word;
        player_right.prev_input_word = player_right.input_word;
        player_left.input_bits_2 = data_801a6966;
        player_right.input_bits_2 = data_801a6972;
        player_left.input_word = data_801a6966;
        player_right.input_word = data_801a6972;
        func_80128c48();
        if (player_left.status_flag == 0xff) {
            update_both_players();
        } else if (player_right.status_flag == 0xff) {
            func_80128e7c();
        } else if ((player_left.field_165 & player_right.field_165) == 0) {
            if (player_left.field_165 != 0) {
                update_player_left_object();
            } else if (player_right.field_165 != 0) {
                update_player_right_object();
            } else if ((get_random_byte() & 1) != 0) {
                update_players_right_first();
            } else {
                update_players_left_first();
            }
        } else if ((get_random_byte() & 1) != 0) {
            update_players_right_first();
        } else {
            update_players_left_first();
        }
    }
}
