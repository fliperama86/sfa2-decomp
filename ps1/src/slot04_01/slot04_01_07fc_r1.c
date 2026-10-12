/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0e04_slot04_01(Object *obj);
int try_enter_state_07_move_7_with_count_slot04_01(Object *obj);
int try_start_meter_move_15a_9_slot04_01(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_01(Object *obj);
int try_start_meter_move_15a_4_slot04_01(Object *obj);
int try_enter_state_07_move_0_slot04_01(Object *obj);
int try_enter_state_07_move_1_if_linked_slot04_01(Object *obj);
int try_enter_state_07_move_6_with_direction_slot04_01(Object *obj);
int try_enter_state_07_move_2_or_3_slot04_01(Object *obj);
int func_801b0ef8_slot04_01(Object *obj);
int func_801b0fb8_slot04_01(Object *obj);

void select_slot04_01_action_from_inputs_slot04_01(Object *obj) {
    if (func_8013d180(obj, 6, 0x1f) && func_801b0e04_slot04_01(obj)) return;
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_7_with_count_slot04_01(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_15a_9_slot04_01(obj)) return;
    if (func_8013cb70(obj, 0, 0x14) && try_start_meter_move_state_8_type_5_slot04_01(obj)) return;
    if ((u8)func_8013caf0(obj, 1, 0x15) && try_start_meter_move_15a_4_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 2, 4) && try_enter_state_07_move_0_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0) && try_enter_state_07_move_1_if_linked_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 4, 2) && try_enter_state_07_move_6_with_direction_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 5, 3) && try_enter_state_07_move_2_or_3_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 8, 0xd) && func_801b0ef8_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 9, 0xe)) func_801b0fb8_slot04_01(obj);
}
