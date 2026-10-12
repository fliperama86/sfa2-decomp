/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slot_byte_runs_unless_kind_4_slot04_04(Object *obj);

u8 try_start_action_state_7_variant_3_slot04_04(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        clear_slot_byte_runs_unless_kind_4_slot04_04(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_meter_action_state_8_variant_4_slot04_04(Object *obj) {
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
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
    return r;
}
