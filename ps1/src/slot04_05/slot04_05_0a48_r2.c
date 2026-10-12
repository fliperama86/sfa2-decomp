/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int update_input_slot_sequence_result_slot04_05(Object *obj, int a1, int a2);
void func_801b142c_slot04_05(Object *obj);
int try_start_meter_move_15a_9_slot04_05(Object *obj);
int try_start_meter_move_state_8_type_5_slot04_05(Object *obj);
int try_start_meter_move_15a_4_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_1_slot5_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_0_slot5_slot04_05(Object *obj);
int enter_state_1_0_7_0_type_2_slot5_slot04_05(Object *obj);
int func_801b1270_slot04_05(Object *obj);
int func_801b11b0_slot04_05(Object *obj);
int clear_slot_stages_7_set_flag_1_slot04_05(Object *obj);
int clear_slot_stages_8_set_flag_2_slot04_05(Object *obj);
int try_enter_state_07_move_7_with_count_slot04_05(Object *obj);

void select_slot04_05_action_from_inputs_slot04_05(Object *obj) {
    func_801b142c_slot04_05(obj);
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_15a_9_slot04_05(obj)) return;
    if ((u8)func_8013caf0(obj, 0, 0x14) && try_start_meter_move_state_8_type_5_slot04_05(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x15) && try_start_meter_move_15a_4_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 2, 5) && enter_state_1_0_7_0_type_1_slot5_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 1) && enter_state_1_0_7_0_type_0_slot5_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0x13) && enter_state_1_0_7_0_type_2_slot5_slot04_05(obj)) return;
    if (func_8013de2c(obj, 5, 0xd) && func_801b1270_slot04_05(obj)) return;
    if (func_8013de2c(obj, 6, 0xe) && func_801b11b0_slot04_05(obj)) return;
    if (update_input_slot_sequence_result_slot04_05(obj, 7, 0) && clear_slot_stages_7_set_flag_1_slot04_05(obj)) return;
    if (update_input_slot_sequence_result_slot04_05(obj, 8, 1) && clear_slot_stages_8_set_flag_2_slot04_05(obj)) return;
    if (get_pressed_bit_8(obj)) try_enter_state_07_move_7_with_count_slot04_05(obj);
}
