/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_state_3_one_restore_facing_slot04_02(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}
