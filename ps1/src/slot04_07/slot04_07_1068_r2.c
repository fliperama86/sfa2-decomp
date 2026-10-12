/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_07_move_7_with_count_slot04_07(Object *obj);
int try_start_meter_move_state_8_type_8_slot04_07(Object *obj);
int try_enter_state_7_kind_3_slot04_07(Object *obj);
int try_start_meter_move_b_slot04_07(Object *obj);
int try_start_meter_move_a_slot04_07(Object *obj);
int try_enter_state_07_move_0_slot04_07(Object *obj);
int enter_state_1_0_7_0_type_1_slot5_slot04_07(Object *obj);
int try_enter_state_7_kind_9_slot04_07(Object *obj);
int try_spend_meter_then_start_move_2_slot04_07(Object *obj);
int try_spend_meter_then_start_move_slot04_07(Object *obj);

void select_slot04_07_action_from_inputs_slot04_07(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_7_with_count_slot04_07(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 0, 3) && try_enter_state_7_kind_3_slot04_07(obj)) return;
    if ((u8)call_slot_stage_if_both_dir_pairs_ok(obj, 1, 0x14) && try_start_meter_move_b_slot04_07(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x15) && try_start_meter_move_a_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 0) && try_enter_state_07_move_0_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 4, 1) && enter_state_1_0_7_0_type_1_slot5_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 5, 2) && try_enter_state_7_kind_9_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 6, 0xe) && try_spend_meter_then_start_move_2_slot04_07(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 7, 0xd)) try_spend_meter_then_start_move_slot04_07(obj);
}
