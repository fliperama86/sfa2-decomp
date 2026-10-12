/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void decrement_counter_byte_if_nonzero_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_1a5 != 0) obj->field_1a5--;
}
