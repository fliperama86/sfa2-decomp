/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
int try_spend_meter_for_action(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void reset_substate_and_set_direction_index_slot04_0e(Object *obj);
int try_start_action_8_clearing_counters_slot04_0e(Object *obj);
int func_801b25e8_slot04_0e(Object *obj);
int func_801b253c_slot04_0e(Object *obj);
int try_start_meter_action_6_slot04_0e(Object *obj);
int try_start_meter_action_5_slot04_0e(Object *obj);
int try_start_action_7_slot04_0e(Object *obj);
int func_801b2348_slot04_0e(Object *obj);
int try_start_action_9_setting_other_field_slot04_0e(Object *obj);
int func_801b2428_slot04_0e(Object *obj);

int try_start_meter_action_4_slot04_0e(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    if (((Slot04bObj *)obj)->field_1da != 0) return 0;
    obj->field_15a = 4;
    obj->state_0 = 1;
    obj->active_flag_3 = 0;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    func_80142778(obj);
    return 1;
}

int try_start_meter_action_5_slot04_0e(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->field_15a = 5;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    func_80142718(obj);
    return 1;
}

int try_start_meter_action_6_slot04_0e(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    func_80142778(obj);
    return 1;
}

int try_start_action_7_slot04_0e(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int func_801b2348_slot04_0e(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->field_15a = 0xc;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int try_start_action_9_setting_other_field_slot04_0e(Object *obj) {
    if (!(u8)try_spend_meter_for_action(obj)) return 0;
    obj->field_15a = 9;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int func_801b2428_slot04_0e(Object *obj) {
    if (!(u8)try_spend_meter_for_action(obj)) return 0;
    obj->field_15a = 0xa;
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->active_flag_3 = 0;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int try_start_action_8_clearing_counters_slot04_0e(Object *obj) {
    if (obj->field_7e != 0 || obj->remaining_count != 0) {
        if (try_action_clearing_both_flags(obj) != 0) {
            obj->field_15a = 8;
            obj->state_0 = 1;
            obj->active_flag_3 = 0;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->substate_index = 0;
            obj->field_12d = 0;
            obj->step_count_2 = 0;
            obj->step_limit = 0;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
    }
    return 0;
}

int func_801b253c_slot04_0e(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state != 0) {
        return (u8)func_801b25e8_slot04_0e(obj);
    }
    if (!(u8)func_80141788(obj)) return 0;
    obj->field_15a = 0xb;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    return 1;
}

int func_801b25e8_slot04_0e(Object *obj) {
    if (obj->field_7e != 0) return 0;
    if (!(u8)func_801418bc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 0xb;
    return 1;
}

void reset_substate_and_set_direction_index_slot04_0e(Object *object) {
    u8 side = object->saved_facing;
    u16 bits = object->pressed_bits | object->released_bits;
    u8 value;
    object->substate_index = 0;
    object->field_12d = 0;
    object->step_count_2 = 0;
    object->step_limit = 0;
    object->facing_side = side;
    if (bits & 0xc0) {
        value = 0;
    } else if (bits & 0x30) {
        value = 2;
    } else {
        value = 4;
    }
    object->direction_index = value;
}
