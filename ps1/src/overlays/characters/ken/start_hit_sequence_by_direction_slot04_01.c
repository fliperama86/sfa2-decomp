/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_hit_sequence_by_direction_slot04_01(Object *obj) {
    int a = 0x2c;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x45;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
