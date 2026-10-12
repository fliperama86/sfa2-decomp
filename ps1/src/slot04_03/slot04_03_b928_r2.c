/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_07_move_6_with_count_slot04_03(Object *obj);
int try_start_meter_move_state_8_type_8_slot04_03(Object *obj);
int start_meter_move_15a_4_if_unlinked_slot04_03(Object *obj);
int start_meter_move_15a_3_if_meter_ready_slot04_03(Object *obj);
int start_meter_move_15a_2_if_meter_ready_slot04_03(Object *obj);
int try_spend_meter_then_start_move_slot04_03(Object *obj);
int try_spend_meter_then_start_move_2_slot04_03(Object *obj);
int try_enter_state_07_move_1_if_linked_slot04_03(Object *obj);
int enter_state_1_0_7_0_if_ready_a_slot04_03(Object *obj);

void select_slot04_03_action_from_inputs_slot04_03(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_6_with_count_slot04_03(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_03(obj)) return;
    if (func_8013cdf0(obj, 0, 4) && start_meter_move_15a_4_if_unlinked_slot04_03(obj)) return;
    if (func_8013cec8(obj, 1, 6) && start_meter_move_15a_3_if_meter_ready_slot04_03(obj)) return;
    if (func_8013ce5c(obj, 2, 5) && start_meter_move_15a_2_if_meter_ready_slot04_03(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 0xd) && try_spend_meter_then_start_move_slot04_03(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 4, 0xe) && try_spend_meter_then_start_move_2_slot04_03(obj)) return;
    if (call_slot_stage_handler_a_2(obj, 5, 2) && try_enter_state_07_move_1_if_linked_slot04_03(obj)) return;
    if (call_slot_stage_handler_a_2(obj, 6, 1)) enter_state_1_0_7_0_if_ready_a_slot04_03(obj);
}

