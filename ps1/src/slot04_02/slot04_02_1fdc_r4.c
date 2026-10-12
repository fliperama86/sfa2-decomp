/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2604_slot04_02(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->field_29c = 2;
    set_sequence_by_side_and_mode(obj, 0x26);
}
