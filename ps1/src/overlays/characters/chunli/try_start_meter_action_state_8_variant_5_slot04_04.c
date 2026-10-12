/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_slot_byte_runs_unless_kind_4_slot04_04(Object *obj);

u8 try_start_meter_action_state_8_variant_5_slot04_04(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        clear_slot_byte_runs_unless_kind_4_slot04_04(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

u8 try_start_meter_action_state_8_variant_6_slot04_04(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    }
    return r;
}
