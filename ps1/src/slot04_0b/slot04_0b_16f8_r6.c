/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c00_slot04_0b(Object *obj) {
    int a = 0x1d;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->field_157++;
    if (obj->field_49 != 0) {
        a = 0x3c;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
