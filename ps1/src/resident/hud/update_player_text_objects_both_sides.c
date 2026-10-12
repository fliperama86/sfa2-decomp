/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_player_text_objects_both_sides(void) {
    Object *p = &player_left;
    copy_kind_text_to_buffer(p);
    if (game_state.mode & 1) {
        format_win_streak_text(p);
        format_object_score_text(p);
        noop_object_handler_3(p);
    }
    p++;
    copy_kind_text_to_buffer(p);
    if (game_state.mode & 2) {
        format_win_streak_text(p);
        format_object_score_text(p);
        noop_object_handler_3(p);
    }
}
