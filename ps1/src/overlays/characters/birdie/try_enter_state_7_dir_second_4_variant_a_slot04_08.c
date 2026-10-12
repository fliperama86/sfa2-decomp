/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_enter_state_7_dir_second_4_variant_a_slot04_08(Object *obj) {
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
        obj->direction_index = ((Slot04aObj *)obj)->field_2cd;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int try_enter_state_7_dir_second_6_variant_b_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->input_dir_second = 6;
        obj->field_15a = one;
        obj->active_flag_3 = one;
        obj->direction_index = ((Slot04aObj *)obj)->field_2f5;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int try_enter_state_7_dir_second_0_variant_b_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->input_dir_second = 0;
        obj->field_15a = one;
        obj->active_flag_3 = one;
        obj->direction_index = ((Slot04aObj *)obj)->field_2dd;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}
