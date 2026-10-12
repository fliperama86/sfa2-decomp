/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_1_when_variant_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1++;
        set_sequence_from_table_5(obj, 1, ((Slot28Obj *)obj)->sequence_ptr);
    }
}
