/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int is_not_state_2_five(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);

int try_start_meter_special_state_8_type_4_slot04_0f(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) {
        if (!is_not_state_2_five(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->facing_side = obj->saved_facing;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)try_action_unless_state_bytes_match(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 4;
    return 1;
}

int try_start_meter_special_state_8_type_7_slot04_0f(Object *obj) {
    if (obj->link_flags != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_mask_a(obj);
    return 1;
}

int try_start_meter_special_state_8_type_8_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_mask_b(obj);
    return 1;
}
