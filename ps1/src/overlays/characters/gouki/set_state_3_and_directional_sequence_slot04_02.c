/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_state_3_and_directional_sequence_slot04_02(Object *obj) {
    obj->state_3 = 0x11;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x5a);
}
