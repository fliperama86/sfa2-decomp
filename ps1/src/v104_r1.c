/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Same form as advance_state_3_set_sequence_7_if_flag: the early return has its own call of
   func_8012a3f4, and the compiler merges only the call instruction. */
void advance_state_3_set_sequence_4_if_flag(Object *object) {
    object->state_3++;
    if (object->mode_flag_2 == 0) {
        func_8012a3f4(object);
        return;
    }
    set_sequence_by_side(object, 4);
    func_8012a3f4(object);
}
