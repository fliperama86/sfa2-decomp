/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_07_move_6_with_count_slot04_08(Object *obj);
int try_start_meter_move_state_8_type_8_slot04_08(Object *obj);
int start_meter_move_15a_4_clear_substate_slot04_08(Object *obj);
int start_meter_move_15a_3_choose_direction_slot04_08(Object *obj);
int try_start_state_7_by_pressed_bits_slot04_08(Object *obj);
int try_enter_state_07_move_0_slot04_08(Object *obj);
int try_enter_state_7_dir_second_6_variant_a_slot04_08(Object *obj);
int try_enter_state_7_dir_second_0_variant_a_slot04_08(Object *obj);
int try_enter_state_7_dir_second_2_variant_a_slot04_08(Object *obj);
int try_enter_state_7_dir_second_4_variant_a_slot04_08(Object *obj);
int try_enter_state_7_dir_second_6_variant_b_slot04_08(Object *obj);
int try_enter_state_7_dir_second_0_variant_b_slot04_08(Object *obj);
int try_enter_state_7_dir_second_2_variant_b_slot04_08(Object *obj);
int try_enter_state_7_dir_second_4_variant_b_slot04_08(Object *obj);
int try_spend_meter_and_start_move_5_slot04_08(Object *obj);
int try_spend_meter_and_start_move_7_slot04_08(Object *obj);

void select_slot04_08_action_from_inputs_slot04_08(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_6_with_count_slot04_08(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_08(obj)) return;
    if ((u8)call_third_table_stage_unless_c0_pair(obj, 0xb, 4) && start_meter_move_15a_4_clear_substate_slot04_08(obj)) return;
    if ((u8)call_slot_stage_if_both_dir_pairs_ok(obj, 0xa, 0x1a) && start_meter_move_15a_3_choose_direction_slot04_08(obj)) return;
    if (call_slot_stage_handler_b(obj, 9, 0x24) && try_start_state_7_by_pressed_bits_slot04_08(obj)) return;
    if ((u8)call_slot_stage_handler_a_2(obj, 0, 2) && try_enter_state_07_move_0_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 4, 3) && try_enter_state_7_dir_second_6_variant_a_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 1, 0) && try_enter_state_7_dir_second_0_variant_a_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 2, 1) && try_enter_state_7_dir_second_2_variant_a_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 3, 2) && try_enter_state_7_dir_second_4_variant_a_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 8, 7) && try_enter_state_7_dir_second_6_variant_b_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 5, 4) && try_enter_state_7_dir_second_0_variant_b_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 6, 5) && try_enter_state_7_dir_second_2_variant_b_slot04_08(obj)) return;
    if (call_slot_stage_handler_b_2(obj, 7, 6) && try_enter_state_7_dir_second_4_variant_b_slot04_08(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 0xc, 0xd) && try_spend_meter_and_start_move_5_slot04_08(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 0xd, 0xe)) try_spend_meter_and_start_move_7_slot04_08(obj);
}
