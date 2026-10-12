/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_sequence_unless_paused_2(Object *object) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(object);
    }
    set_flag_and_push_if_mode_8(object);
}
