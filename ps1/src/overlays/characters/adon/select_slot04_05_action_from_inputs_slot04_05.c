/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int update_input_slot_sequence_result_slot04_05(Object *obj, int a1, int a2);
void decrement_counter_byte_if_nonzero_slot04_05(Object *obj);
int try_start_meter_move_15a_9_slot04_05(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_05(Object *obj);
int try_start_meter_move_15a_4_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_1_slot5_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_0_slot5_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_2_slot5_slot04_05(Object *obj);
int try_spend_meter_then_start_move_2_slot04_05(Object *obj);
int try_spend_meter_then_start_move_slot04_05(Object *obj);
int clear_slot_stages_7_set_flag_1_slot04_05(Object *obj);
int clear_slot_stages_8_set_flag_2_slot04_05(Object *obj);
int try_enter_state_07_move_7_with_count_slot04_05(Object *obj);

void select_slot04_05_action_from_inputs_slot04_05(Object *obj) {
    decrement_counter_byte_if_nonzero_slot04_05(obj);
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_15a_9_slot04_05(obj)) return;
    if ((u8)call_slot_stage_if_both_dir_pairs_ok(obj, 0, 0x14) && try_start_meter_move_state_8_type_5_slot04_05(obj)) return;
    if ((u8)call_second_table_stage_unless_c0_pair(obj, 1, 0x15) && try_start_meter_move_15a_4_slot04_05(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 2, 5) && enter_state_1_0_7_0_type_1_slot5_slot04_05(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 3, 1) && enter_state_1_0_7_0_type_0_slot5_slot04_05(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 4, 0x13) && enter_state_1_0_7_0_type_2_slot5_slot04_05(obj)) return;
    if (call_slot_stage_handler_in_second_table(obj, 5, 0xd) && try_spend_meter_then_start_move_2_slot04_05(obj)) return;
    if (call_slot_stage_handler_in_second_table(obj, 6, 0xe) && try_spend_meter_then_start_move_slot04_05(obj)) return;
    if (update_input_slot_sequence_result_slot04_05(obj, 7, 0) && clear_slot_stages_7_set_flag_1_slot04_05(obj)) return;
    if (update_input_slot_sequence_result_slot04_05(obj, 8, 1) && clear_slot_stages_8_set_flag_2_slot04_05(obj)) return;
    if (get_pressed_bit_8(obj)) try_enter_state_07_move_7_with_count_slot04_05(obj);
}
