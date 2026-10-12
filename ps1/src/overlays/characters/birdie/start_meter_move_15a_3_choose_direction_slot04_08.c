/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int start_meter_move_15a_3_choose_direction_slot04_08(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->facing_side = obj->saved_facing;
        if (obj->field_25c != 0) {
            if (((obj->pressed_bits | obj->released_bits) & 0x30) != 0) {
                obj->input_dir_second = 0;
                set_direction_index_from_buttons_mask_a(obj);
            } else {
                obj->input_dir_second = 2;
                set_direction_index_from_buttons_mask_b(obj);
            }
        } else {
            if (((obj->pressed_bits | obj->released_bits) & 0x6a) != 0) {
                obj->input_dir_second = 2;
                set_direction_index_from_buttons_mask_b(obj);
            } else {
                obj->input_dir_second = 0;
                set_direction_index_from_buttons_mask_a(obj);
            }
        }
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int start_meter_move_15a_4_clear_substate_slot04_08(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    }
}
