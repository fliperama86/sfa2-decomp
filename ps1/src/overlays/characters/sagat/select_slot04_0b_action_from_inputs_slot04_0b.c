/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_meter_move_type_a_slot04_0b(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_0b(Object *obj);
int try_start_meter_move_15a_4_slot04_0b(Object *obj);
int try_enter_state_07_move_0_slot04_0b(Object *obj);
int try_enter_state_7_kind_3_slot04_0b(Object *obj);
int try_enter_state_07_move_1_if_linked_slot04_0b(Object *obj);
int spend_meter_start_move_type_seven_slot04_0b(Object *obj);
int spend_meter_start_move_type_eight_slot04_0b(Object *obj);
int try_enter_state_7_kind_9_slot04_0b(Object *obj);
int try_enter_state_7_kind_2_unlinked_slot04_0b(Object *obj);
int try_start_move_state_8_type_6_slot04_0b(Object *obj);

void select_slot04_0b_action_from_inputs_slot04_0b(Object *obj) {
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_type_a_slot04_0b(obj)) return;
    if (get_pressed_bit_8(obj) && try_enter_state_7_kind_9_slot04_0b(obj)) return;
    if (call_second_table_stage_unless_30_pair(obj, 0, 0x15) && try_start_meter_move_state_8_type_5_slot04_0b(obj)) return;
    if ((u8)call_second_table_stage_unless_c0_pair(obj, 1, 0x18) && try_start_move_state_8_type_6_slot04_0b(obj)) return;
    if (call_second_table_stage_unless_0c_pair(obj, 2, 0x17) && try_start_meter_move_15a_4_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 3, 4) && try_enter_state_07_move_0_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 4, 5) && try_enter_state_7_kind_3_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 5, 0) && try_enter_state_07_move_1_if_linked_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 6, 1) && try_enter_state_7_kind_2_unlinked_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 7, 0xe) && spend_meter_start_move_type_seven_slot04_0b(obj)) return;
    if (call_slot_stage_handler_a(obj, 8, 0xd)) spend_meter_start_move_type_eight_slot04_0b(obj);
}
