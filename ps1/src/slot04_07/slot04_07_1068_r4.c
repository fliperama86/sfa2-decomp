/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_7_set_other_lock_sequence_by_mode_b_slot04_07(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 4;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->other->field_6b = 0x11;
    obj->field_6b = 0;
    obj->other_lock = 0x15;
    set_sequence_by_side_and_mode(obj, 0x1d);
}

int try_start_meter_move_a_slot04_07(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

int try_start_meter_move_b_slot04_07(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        ((Slot04aObj *)obj)->field_1cd = obj->field_4b;
        set_direction_index_from_buttons_mask_a(obj);
    }
    return r;
}
