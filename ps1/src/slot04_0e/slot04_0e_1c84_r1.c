/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
u8 func_8013d130(Object *object, u8 a, u8 b);
u8 try_action_clearing_both_flags(Object *object);
void reset_substate_and_set_direction_index_slot04_0e(Object *obj);
int try_start_action_8_clearing_counters_slot04_0e(Object *obj);
int func_801b253c_slot04_0e(Object *obj);
int try_start_meter_action_6_slot04_0e(Object *obj);
int try_start_meter_action_5_slot04_0e(Object *obj);
int try_start_action_0_if_unlinked_slot04_0e(Object *obj);
int try_start_action_1_slot04_0e(Object *obj);
int try_start_action_7_slot04_0e(Object *obj);
int func_801b2348_slot04_0e(Object *obj);
int func_801b1fa0_slot04_0e(Object *obj);
int try_start_airborne_action_with_variant_slot04_0e(Object *obj);
int try_start_action_3_with_variant_slot04_0e(Object *obj);
int try_start_action_9_setting_other_field_slot04_0e(Object *obj);
int func_801b2428_slot04_0e(Object *obj);

void select_slot04_0e_action_from_inputs_slot04_0e(Object *obj) {
    if (get_pressed_bit_8(obj) && (u8)try_start_action_8_clearing_counters_slot04_0e(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && (u8)func_801b253c_slot04_0e(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x15) && (u8)try_start_meter_action_6_slot04_0e(obj)) return;
    if ((u8)func_8013caf0(obj, 2, 0x19) && (u8)try_start_meter_action_5_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 9) && (u8)try_start_action_0_if_unlinked_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 3) && (u8)try_start_action_1_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0) && (u8)try_start_action_7_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 2) && (u8)func_801b2348_slot04_0e(obj)) return;
    if ((u8)func_8013d0c8(obj) && (u8)func_801b1fa0_slot04_0e(obj)) return;
    if ((u8)func_8013d0fc(obj) && (u8)try_start_airborne_action_with_variant_slot04_0e(obj)) return;
    if (func_8013d130(obj, 7, 0x1d) && (u8)try_start_action_3_with_variant_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0xd) && (u8)try_start_action_9_setting_other_field_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 9, 0xe)) func_801b2428_slot04_0e(obj);
}

int try_start_action_0_if_unlinked_slot04_0e(Object *obj) {
    if (obj->link_flags != 0) return 0;
    if (!(u8)try_action_clearing_both_flags(obj)) return 0;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->field_15a = 0;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int try_start_action_1_slot04_0e(Object *obj) {
    if (!(u8)try_action_clearing_both_flags(obj)) return 0;
    obj->field_15a = 1;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int func_801b1fa0_slot04_0e(Object *obj) {
    if (!(u8)try_action_clearing_both_flags(obj)) return 0;
    obj->field_15a = 0xd;
    obj->active_flag_3 = 1;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}

int try_start_airborne_action_with_variant_slot04_0e(Object *obj) {
    if (!(u8)func_801418bc(obj)) return 0;
    if (obj->active_state == 0) return 0;
    if (obj->vel_y >= 0) return 0;
    if (obj->pos_y < obj->ground_y - 0x20) return 0;
    obj->variant_index = 1;
    if (!(obj->input_bits_2 & 0x8000)) {
        obj->variant_index = 0xff;
    }
    obj->field_15a = 2;
    obj->active_flag_3 = 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    reset_substate_and_set_direction_index_slot04_0e(obj);
    return 1;
}
