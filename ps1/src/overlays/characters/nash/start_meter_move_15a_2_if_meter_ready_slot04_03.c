/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_three_slot_byte_runs_slot04_03(Object *obj);


/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int start_meter_move_15a_2_if_meter_ready_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->substate_index = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int start_meter_move_15a_3_if_meter_ready_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        clear_three_slot_byte_runs_slot04_03(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int start_meter_move_15a_4_if_unlinked_slot04_03(Object *obj) {
    if ((s16)obj->meter >= 0x30 && obj->link_flags == 0 && (u8)func_80141788(obj)) {
        clear_three_slot_byte_runs_slot04_03(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    }
}

