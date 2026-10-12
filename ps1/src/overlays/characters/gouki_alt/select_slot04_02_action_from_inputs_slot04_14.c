/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
u8 get_slot_result_via_first_stage_table_2(Object *object, u8 index, u8 arg);
int call_second_table_stage_unless_0c_pair(Object *object, u8 index, u8 arg);
int call_second_table_stage_unless_c0_pair(Object *object, u8 index, u8 arg);
int call_second_table_stage_unless_30_pair(Object *object, u8 index, u8 arg);
int call_slot_stage_handler_a(Object *object, u8 index, u8 arg);

/* functions of other units of this module */
u8 try_enter_state_7_with_action_code_c_if_counted_slot04_14(Object *obj);
u8 try_enter_meter_action_state_8_slot04_14(Object *obj);
u8 try_start_state_8_when_meter_high_slot04_14(Object *obj);
u8 try_start_meter_action_state_8_slot04_14(Object *obj);
u8 try_start_meter_move_5_unlinked_slot04_14(Object *obj);
u8 try_start_meter_move_7_slot04_14(Object *obj);
u8 try_enter_move_state_with_same_variant_slot04_14(Object *obj);
u8 try_start_action_08_opposite_variant_slot04_14(Object *obj);
u8 try_enter_state_7_with_action_code_zero_slot04_14(Object *obj);
u8 try_enter_state_7_with_action_code_e_slot04_14(Object *obj);
u8 try_enter_state_7_with_action_code_ten_slot04_14(Object *obj);
u8 try_enter_move_state_grounded_or_airborne_slot04_14(Object *obj);
u8 try_enter_move_state_with_input_direction_slot04_14(Object *obj);
u8 try_enter_state_7_with_action_code_d_slot04_14(Object *obj);
u8 try_start_meter_spend_action_with_sequence_slot04_14(Object *obj);
u8 try_start_meter_spend_action_state_5_slot04_14(Object *obj);

void select_slot04_02_action_from_inputs_slot04_14(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_7_with_action_code_c_if_counted_slot04_14(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_enter_meter_action_state_8_slot04_14(obj)) return;
    if (get_slot_result_via_first_stage_table_2(obj, 0, 0x1e) && try_start_state_8_when_meter_high_slot04_14(obj)) return;
    if ((u8)call_second_table_stage_unless_0c_pair(obj, 1, 0x18) && try_start_meter_action_state_8_slot04_14(obj)) return;
    if ((u8)call_second_table_stage_unless_c0_pair(obj, 2, 0x26) && try_start_meter_move_5_unlinked_slot04_14(obj)) return;
    if ((u8)call_second_table_stage_unless_30_pair(obj, 3, 0x14) && try_start_meter_move_7_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 4, 0x1b) && try_enter_move_state_with_same_variant_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 5, 0x1c) && try_start_action_08_opposite_variant_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 6, 4) && try_enter_state_7_with_action_code_zero_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 7, 0xf) && try_enter_state_7_with_action_code_e_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 8, 0x12) && try_enter_state_7_with_action_code_ten_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 9, 0) && try_enter_move_state_grounded_or_airborne_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 10, 3) && try_enter_move_state_with_input_direction_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 11, 2) && try_enter_state_7_with_action_code_d_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 12, 0xe) && try_start_meter_spend_action_with_sequence_slot04_14(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 13, 0xd)) try_start_meter_spend_action_state_5_slot04_14(obj);
}
