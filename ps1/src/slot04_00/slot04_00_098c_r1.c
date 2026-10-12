/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_07_move_9_slot04_00(Object *obj);
int try_enter_state_07_move_6_with_count_slot04_00(Object *obj);
int try_start_meter_move_state_8_type_8_slot04_00(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_00(Object *obj);
int try_start_meter_move_state_8_type_4_slot04_00(Object *obj);
int try_enter_state_07_move_0_slot04_00(Object *obj);
int try_enter_state_07_move_1_if_linked_slot04_00(Object *obj);
int try_enter_state_07_move_2_or_3_slot04_00(Object *obj);
int try_spend_meter_then_start_move_slot04_00(Object *obj);
int try_spend_meter_then_set_lock_sequence_slot04_00(Object *obj);

void try_special_move_actions_in_order_slot04_00(Object *obj) {
    if (get_slot_result_via_first_stage_table_3(obj, 5, 0x1f) && try_enter_state_07_move_9_slot04_00(obj)) return;
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_6_with_count_slot04_00(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_00(obj)) return;
    if ((u8)call_slot_stage_if_both_dir_pairs_ok(obj, 0, 0x17) && try_start_meter_move_state_8_type_5_slot04_00(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x18) && try_start_meter_move_state_8_type_4_slot04_00(obj)) return;
    if (obj->field_7e == 0) {
        if ((u8)call_slot_stage_handler_in_second_table(obj, 2, 4) && try_enter_state_07_move_0_slot04_00(obj)) return;
        if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 0) && try_enter_state_07_move_1_if_linked_slot04_00(obj)) return;
    } else {
        if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 0) && try_enter_state_07_move_1_if_linked_slot04_00(obj)) return;
        if ((u8)call_slot_stage_handler_in_second_table(obj, 2, 4) && try_enter_state_07_move_0_slot04_00(obj)) return;
    }
    if ((u8)call_slot_stage_handler_in_second_table(obj, 4, 3) && try_enter_state_07_move_2_or_3_slot04_00(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 6, 0xd) && try_spend_meter_then_start_move_slot04_00(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 7, 0xe)) try_spend_meter_then_set_lock_sequence_slot04_00(obj);
}
