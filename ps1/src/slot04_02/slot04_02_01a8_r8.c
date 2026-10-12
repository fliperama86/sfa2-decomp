/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 func_801b0d74_slot04_02(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xd;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

u8 func_801b0de0_slot04_02(Object *obj) {
    u8 r;

    if (obj->field_7e == 0 && obj->remaining_count == 0) return;
    r = try_action_clearing_both_flags(obj);
    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xc;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}
