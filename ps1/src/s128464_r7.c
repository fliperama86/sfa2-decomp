/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_state_3_and_set_sequence_by_side(Object *object) {
    int arg = 0;
    object->state_3 = object->state_3 + 1;
    if (object->mode_flag_2 != 0) {
        if (object->field_157 != 0) {
            object->state_3 = object->state_3 + 1;
            arg = 1;
        }
    } else {
        if (object->kind == 10) arg = 0x30;
        if (object->field_7e != 0) {
            object->field_278 = 0;
            arg = 2;
        }
    }
    set_sequence_by_side(object, arg);
    choose_transition_or_update_sequence(object);
}
