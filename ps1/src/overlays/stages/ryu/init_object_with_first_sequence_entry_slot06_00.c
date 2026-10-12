/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801e9f44_slot06_00[];

void init_object_with_first_sequence_entry_slot06_00(Object *object) {
    object->field_0a = 1;
    object->field_0f = 1;
    object->spawn_mode = 4;
    object->state_0 = object->state_0 + 1;
    object->field_0c = 0;
    object->pos_x_frac = object->pos_x;
    object->pos_y_frac = object->pos_y;
    set_sequence_entry(object, data_801e9f44_slot06_00[0]);
}
