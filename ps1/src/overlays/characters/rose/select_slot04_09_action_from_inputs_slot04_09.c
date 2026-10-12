/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_meter_move_type_9_slot04_09(Object *obj);
int try_start_move_7_with_remaining_count_slot04_09(Object *obj);
int try_start_meter_move_with_buttons_group_a_slot04_09(Object *obj);
int try_start_meter_move_with_buttons_group_b_slot04_09(Object *obj);
int try_start_move_0_unless_linked_slot04_09(Object *obj);
int try_start_move_1_slot04_09(Object *obj);
int try_start_move_2_slot04_09(Object *obj);
int try_enter_action_move_without_meter_cost_slot04_09(Object *obj);
int try_start_meter_move_4_slot04_09(Object *obj);
int try_start_meter_move_5_unless_linked_slot04_09(Object *obj);
int try_start_meter_move_6_if_counter_zero_slot04_09(Object *obj);

void select_slot04_09_action_from_inputs_slot04_09(Object *obj) {
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_type_9_slot04_09(obj)) return;
    if (call_second_table_stage_unless_0c_pair(obj, 0, 0x15) && try_start_meter_move_6_if_counter_zero_slot04_09(obj)) return;
    if ((u8)call_second_table_stage_unless_30_pair(obj, 1, 0x14) && try_start_meter_move_4_slot04_09(obj)) return;
    if ((u8)call_second_table_stage_unless_c0_pair(obj, 2, 0x19) && try_start_meter_move_5_unless_linked_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 3, 0x10) && try_start_move_0_unless_linked_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 4, 2) && try_start_move_1_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 5, 4) && try_start_move_2_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 6, 0xd) && try_start_meter_move_with_buttons_group_a_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 7, 1) && try_enter_action_move_without_meter_cost_slot04_09(obj)) return;
    if (call_slot_stage_handler_a(obj, 8, 0xe) && try_start_meter_move_with_buttons_group_b_slot04_09(obj)) return;
    if (get_pressed_bit_8(obj)) try_start_move_7_with_remaining_count_slot04_09(obj);
}
