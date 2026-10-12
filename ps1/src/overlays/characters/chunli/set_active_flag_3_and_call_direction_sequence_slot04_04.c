/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_active_flag_3_and_call_direction_sequence_slot04_04(Object *object) {
    object->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(object);
}
