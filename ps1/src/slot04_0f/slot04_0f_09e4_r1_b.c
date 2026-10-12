/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 a, u8 b);
u8 try_action_clearing_both_flags(Object *object);
int is_state_01010101_and_ready(Object *object);

/* functions of other units of this module */
int try_start_meter_special_state_8_type_4_slot04_0f(Object *obj);
int try_start_meter_special_state_8_type_8_slot04_0f(Object *obj);
int try_start_meter_special_state_8_type_7_slot04_0f(Object *obj);
int try_start_move_variant_same_facing_slot04_0f(Object *obj);
int try_start_move_variant_flipped_facing_slot04_0f(Object *obj);
int try_start_move_2_with_dir_slot04_0f(Object *obj);
int try_start_move_3_with_dir_slot04_0f(Object *obj);
int try_start_move_1_with_dir_slot04_0f(Object *obj);
int func_801b1530_slot04_0f(Object *obj);
int func_801b1628_slot04_0f(Object *obj);

int try_start_move_by_active_state_slot04_0f(Object *obj);
int func_801b12bc_slot04_0f(Object *obj);

void count_down_then_reset_or_update_sequence_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown--;
    if (obj->countdown != 0) {
        reset_object_to_state_1_0_sequence_0(o);
    } else {
        update_sequence(o);
    }
}

void select_slot04_0f_action_from_inputs_slot04_0f(Object *obj) {
    if (obj->active_state == 0) {
        ((Slot04bObj *)obj)->set_flag = 0;
    }
    if (get_pressed_bit_8(obj) && try_start_move_by_active_state_slot04_0f(obj)) return;
    if (is_pressed_bits_in_table(obj) && try_start_meter_special_state_8_type_4_slot04_0f(obj)) return;
    if (func_8013caf0(obj, 0, 0x15) && try_start_meter_special_state_8_type_8_slot04_0f(obj)) return;
    if (func_8013cb70(obj, 1, 0x18) && try_start_meter_special_state_8_type_7_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 2, 0x1b) && try_start_move_variant_same_facing_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 3, 0x1c) && try_start_move_variant_flipped_facing_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 4, 0x12) && try_start_move_2_with_dir_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 5, 0x13) && try_start_move_3_with_dir_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 6, 0) && try_start_move_1_with_dir_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 7, 0xe) && func_801b12bc_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 8, 0xd) && func_801b1530_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 9, 0xe)) func_801b1628_slot04_0f(obj);
}

int try_start_move_by_active_state_slot04_0f(Object *obj) {
    if (obj->field_7e == 0 && obj->remaining_count == 0) {
        return 0;
    }
    if (obj->active_state == 0) {
        if (try_action_clearing_both_flags(obj)) {
            obj->state_0 = 1;
            obj->state_2 = 7;
            obj->state_1 = 0;
            obj->state_3 = 0;
            obj->field_15a = 0xa;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
        return 0;
    }
    if (obj->vel_y < 0) {
        return 0;
    }
    if ((u8)func_801418bc(obj)) {
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 0xb;
        obj->facing_side = obj->saved_facing;
        return 1;
    }
    return 0;
}

/* The call of is_state_01010101_and_ready passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 4 instruction slots. */
int func_801b12bc_slot04_0f(Object *obj) {
    if (obj->hit_flag != 0) {
        return 0;
    }
    if (((int (*)(void))is_state_01010101_and_ready)()) {
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 0;
        return 1;
    }
    return 0;
}
