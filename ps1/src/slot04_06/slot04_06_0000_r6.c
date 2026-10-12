/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_7_kind_7_slot04_06(Object *obj);
int try_start_meter_move_15a_9_slot04_06(Object *obj);
int try_start_meter_move_3_slot04_06(Object *obj);
int try_start_meter_move_15a_1_direction_4_slot04_06(Object *obj);
int try_enter_state_7_kind_0_slot04_06(Object *obj);
int try_enter_state_7_kind_5_slot04_06(Object *obj);
int try_enter_state_7_kind_2_slot04_06(Object *obj);
int try_enter_state_7_kind_a_slot04_06(Object *obj);
int try_enter_state_7_when_ready_direction_2_slot04_06(Object *obj);
int func_801b0de4_slot04_06(Object *obj);
int func_801b0ee4_slot04_06(Object *obj);

void select_slot04_06_action_from_inputs_slot04_06(Object *obj) {
    if (get_pressed_bit_8(obj) && (u8)try_enter_state_7_kind_7_slot04_06(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && (u8)try_start_meter_move_15a_9_slot04_06(obj)) return;
    if (call_slot_stage_d4cc_if_both_dir_pairs_ok(obj, 0, 0x22) && (u8)try_start_meter_move_3_slot04_06(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x14) && (u8)try_start_meter_move_15a_1_direction_4_slot04_06(obj)) return;
    if (call_slot_stage_handler_b(obj, 2, 0x21) && (u8)try_enter_state_7_kind_0_slot04_06(obj)) return;
    if (call_slot_stage_handler_b(obj, 3, 0x23) && (u8)try_enter_state_7_kind_5_slot04_06(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 4, 0) && (u8)try_enter_state_7_kind_2_slot04_06(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 5, 5) && (u8)try_enter_state_7_kind_a_slot04_06(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 6, 0xe) && (u8)try_enter_state_7_when_ready_direction_2_slot04_06(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 7, 0xd) && (u8)func_801b0de4_slot04_06(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 8, 0xe)) func_801b0ee4_slot04_06(obj);
}
