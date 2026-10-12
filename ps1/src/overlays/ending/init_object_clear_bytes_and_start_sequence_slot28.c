/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_object_clear_bytes_and_start_sequence_slot28(Object *obj) {
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    set_sequence_from_table_5(obj, obj->table_index, ((Slot28Obj *)obj)->sequence_ptr);
}
