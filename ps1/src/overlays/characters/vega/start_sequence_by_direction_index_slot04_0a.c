/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_by_direction_index_slot04_0a(Object *obj) {
    int a;

    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x1c;
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 != 0) {
        a = 0x4d;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
