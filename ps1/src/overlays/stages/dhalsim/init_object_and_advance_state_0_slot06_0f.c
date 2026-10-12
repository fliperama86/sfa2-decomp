/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_table_and_start_entry_slot06_0f(Object *obj);

void init_object_and_advance_state_0_slot06_0f(Object *obj) {
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->pos_y += 0x58;
    if (obj->facing_side == 0) {
        obj->field_0a = 1;
    }
    set_sequence_table_and_start_entry_slot06_0f(obj);
}
