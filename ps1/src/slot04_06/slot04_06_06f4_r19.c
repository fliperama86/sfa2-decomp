/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_sequence_checked_then_pos_x_limit_flag_slot04_06(Object *obj) {
    advance_sequence_checked(obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}
