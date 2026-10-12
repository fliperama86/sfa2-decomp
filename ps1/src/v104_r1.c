/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Same form as advance_state_3_set_sequence_7_if_flag: the early return has its own call of
   select_transition_or_next_sequence_plus_7, and the compiler merges only the call instruction. */
void advance_state_3_set_sequence_4_if_flag(Object *object) {
    object->state_3++;
    if (object->mode_flag_2 == 0) {
        select_transition_or_next_sequence_plus_7(object);
        return;
    }
    set_sequence_by_side(object, 4);
    select_transition_or_next_sequence_plus_7(object);
}
