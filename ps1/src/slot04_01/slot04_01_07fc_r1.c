/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_action_and_enter_state_7_move_a_slot04_01(Object *obj);
int try_enter_state_07_move_7_with_count_slot04_01(Object *obj);
int try_start_meter_move_15a_9_slot04_01(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_01(Object *obj);
int try_start_meter_move_15a_4_slot04_01(Object *obj);
int try_enter_state_07_move_0_slot04_01(Object *obj);
int try_enter_state_07_move_1_if_linked_slot04_01(Object *obj);
int try_enter_state_07_move_6_with_direction_slot04_01(Object *obj);
int try_enter_state_07_move_2_or_3_slot04_01(Object *obj);
int try_spend_meter_then_start_move_slot04_01(Object *obj);
int try_spend_meter_then_start_move_2_slot04_01(Object *obj);

void select_slot04_01_action_from_inputs_slot04_01(Object *obj) {
    if (get_slot_result_via_first_stage_table_3(obj, 6, 0x1f) && try_action_and_enter_state_7_move_a_slot04_01(obj)) return;
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_7_with_count_slot04_01(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_15a_9_slot04_01(obj)) return;
    if (func_8013cb70(obj, 0, 0x14) && try_start_meter_move_state_8_type_5_slot04_01(obj)) return;
    if ((u8)call_slot_stage_if_both_dir_pairs_ok(obj, 1, 0x15) && try_start_meter_move_15a_4_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 2, 4) && try_enter_state_07_move_0_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 0) && try_enter_state_07_move_1_if_linked_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 4, 2) && try_enter_state_07_move_6_with_direction_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 5, 3) && try_enter_state_07_move_2_or_3_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 8, 0xd) && try_spend_meter_then_start_move_slot04_01(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 9, 0xe)) try_spend_meter_then_start_move_2_slot04_01(obj);
}
