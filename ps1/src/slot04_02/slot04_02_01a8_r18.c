/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_by_direction_index_field49_c_slot04_02(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}
