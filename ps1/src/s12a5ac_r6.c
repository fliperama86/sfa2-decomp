/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void update_sequence_or_return_to_idle(Object *object) {
    if ((s16)object->step_flags >= 0) {
        update_sequence(object);
    } else if (object->field_157 != 0) {
        reset_object_to_state_1_0_0_2_sequence_1(object);
    } else {
        reset_object_to_state_1_0_sequence_0(object);
    }
}
