/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Aliases of player_left.field_b2 (read as u8) and player_right.field_b2 (read as s16); the member width differs. */

void log_both_players_state(void) {
    Object *object = &player_left;
    log_object_history(object);
    log_object_history(object + 1);
}
