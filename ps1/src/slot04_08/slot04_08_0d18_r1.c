/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0d18_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->input_dir_second = 2;
        obj->active_flag_3 = one;
        obj->field_15a = one;
        obj->direction_index = ((Slot04aObj *)obj)->field_2e5;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int func_801b0d94_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->input_dir_second = 4;
        obj->active_flag_3 = one;
        obj->field_15a = one;
        obj->direction_index = ((Slot04aObj *)obj)->field_2ed;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int try_start_state_7_by_pressed_bits_slot04_08(Object *obj) {
    if (try_action_clearing_both_flags(obj) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->facing_side = obj->saved_facing;
        if (((obj->pressed_bits | obj->released_bits) & 0x6a) != 0) {
            obj->field_15a = 9;
            set_direction_index_from_buttons_group_b(obj);
        } else {
            obj->field_15a = 2;
            set_direction_index_from_buttons_group_a(obj);
        }
    }
}
