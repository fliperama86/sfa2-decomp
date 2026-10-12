/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_with_meter_gain_one_slot04_02(Object *obj) {
    int one = 1;
    int a;

    obj->active_flag_4 = one;
    obj->state_3 = one;
    obj->field_157 = 0;
    add_to_meter_clamped(obj, 1);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x6c;
        obj->field_225 = one;
    } else {
        a = 0x49;
    }
    set_sequence_by_side_and_mode(obj, a);
}
