/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012a8dc(Object *object) {
    handler_table_19b0[object->state_3](object);
}

void func_8012a91c(Object *object) {
    int index;
    if ((s16)object->step_flags >= 0) {
        update_sequence(object);
        return;
    }
    object->state_3 = object->state_3 + 1;
    object->field_157 = 0;
    object->active_state = 1;
    if (object->variant_index == 1) {
        index = 0x13;
    } else if (object->variant_index & 0x80) {
        index = 0x14;
    } else {
        index = 0x12;
    }
    set_sequence_by_side(object, index);
}
