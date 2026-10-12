/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void begin_state_3_sequence_by_half_direction_slot04_04(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x2d;
    if (obj->field_49 != 0) {
        a = 0x49;
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
}
