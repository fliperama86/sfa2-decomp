#include "game.h"
#include "externs.h"
#include "protos.h"


void update_players_left_first(void) {
    Object *left = &player_left;
    update_object_input_and_timers(left);
    if (player_left.status_flag != 0) {
        update_input_and_timers_for_both_players();
    } else {
        update_object_input_and_timers(left + 1);
        if (player_right.status_flag != 0) {
            update_both_players();
        } else {
            set_box_distance_x_for_both_players();
            clear_flags_when_player_frames_inactive();
        }
    }
}
