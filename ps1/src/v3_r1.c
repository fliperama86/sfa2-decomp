/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_state_3_set_sequence_7_if_flag(Object *object) {
    object->state_3++;
    if (object->mode_flag_2 == 0) {
        func_8012a1e4(object);
        return;
    }
    set_sequence_by_side(object, 7);
    func_8012a1e4(object);
}
