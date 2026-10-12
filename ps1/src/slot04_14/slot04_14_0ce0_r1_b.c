/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
/* function of another unit of this module */
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_80142718(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);

u8 func_801b1558_slot04_14(Object *obj) {
    u8 r;

    if (obj->active_state != 0) {
        r = func_801418bc(obj);
        if (!r) return;
        obj->field_15a = 3;
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        set_direction_index_from_buttons_group_b(obj);
    } else {
        r = try_action_clearing_both_flags(obj);
        if (!r) return;
        obj->field_15a = 2;
        obj->facing_side = obj->saved_facing;
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_meter_move_5_unlinked_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) return 0;
        r = func_80141788(obj);
        if (r) {
            set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 5;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            func_80142718(obj);
        }
    }
    return r;
}
