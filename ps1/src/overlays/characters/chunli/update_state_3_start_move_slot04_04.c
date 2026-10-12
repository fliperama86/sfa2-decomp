/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_state_3_start_move_slot04_04(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
