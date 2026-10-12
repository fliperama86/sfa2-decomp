/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_7_kind_8_slot04_0a(Object *obj);
int try_start_meter_move_type_a_slot04_0a(Object *obj);
int try_start_meter_state_8_move_5_slot04_0a(Object *obj);
int try_start_meter_state_8_move_4_slot04_0a(Object *obj);
int try_enter_state_7_kind_3_offset_0_slot04_0a(Object *obj);
int try_enter_state_7_kind_3_offset_4_slot04_0a(Object *obj);
int try_enter_state_7_kind_6_slot04_0a(Object *obj);
int try_enter_state_7_kind_0_slot04_0a(Object *obj);
int try_enter_state_7_kind_1_slot04_0a(Object *obj);
int try_enter_state_7_kind_2_slot04_0a(Object *obj);
int func_801b10c4_slot04_0a(Object *obj);
int func_801b116c_slot04_0a(Object *obj);

void select_slot04_0a_action_from_inputs_slot04_0a(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_7_kind_8_slot04_0a(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_type_a_slot04_0a(obj)) return;
    if (func_8013cf34(obj, 0, 5) && try_start_meter_state_8_move_5_slot04_0a(obj)) return;
    if ((u8)func_8013cdf0(obj, 1, 4) && try_start_meter_state_8_move_4_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 2, 0x1b) && try_enter_state_7_kind_3_offset_0_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0x1c) && try_enter_state_7_kind_3_offset_4_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 4, 0) && try_enter_state_7_kind_6_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 5, 3) && try_enter_state_7_kind_0_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 6, 1) && try_enter_state_7_kind_1_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 7, 2) && try_enter_state_7_kind_2_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 8, 0xd) && func_801b10c4_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 9, 0xe)) func_801b116c_slot04_0a(obj);
}
