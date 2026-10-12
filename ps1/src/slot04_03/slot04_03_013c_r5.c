/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_and_start_sequence_slot04_03(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
