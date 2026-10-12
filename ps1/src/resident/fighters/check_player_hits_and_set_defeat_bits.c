/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void check_player_hits_and_set_defeat_bits(void) {
    Object *pair = &player_left;

    reset_hit_results();
    check_hit_of_a_wide_box_against_b_boxes(pair, pair + 1);
    reset_hit_results();
    check_hit_of_a_wide_box_against_b_boxes(pair + 1, pair);
    if ((short)player_left.health < 0) {
        game_state.config->defeat_side_bits |= 2;
    }
    if ((short)player_right.health < 0) {
        game_state.config->defeat_side_bits |= 1;
    }
}
