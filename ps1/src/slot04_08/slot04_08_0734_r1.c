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
int func_801b0a38_slot04_08(Object *obj);
int func_801b0ab4_slot04_08(Object *obj);
int func_801b0b2c_slot04_08(Object *obj);
int func_801b0ba8_slot04_08(Object *obj);
int func_801b0c24_slot04_08(Object *obj);
int func_801b0ca0_slot04_08(Object *obj);
int func_801b0d18_slot04_08(Object *obj);
int func_801b0d94_slot04_08(Object *obj);
int func_801b108c_slot04_08(Object *obj);
int func_801b1144_slot04_08(Object *obj);

void select_slot04_08_action_from_inputs_slot04_08(Object *obj) {
    if (get_pressed_bit_8(obj) && try_enter_state_07_move_6_with_count_slot04_08(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_08(obj)) return;
    if ((u8)func_8013cdf0(obj, 0xb, 4) && start_meter_move_15a_4_clear_substate_slot04_08(obj)) return;
    if ((u8)func_8013caf0(obj, 0xa, 0x1a) && start_meter_move_15a_3_choose_direction_slot04_08(obj)) return;
    if (func_8013ccb4(obj, 9, 0x24) && try_start_state_7_by_pressed_bits_slot04_08(obj)) return;
    if ((u8)func_8013cdc8(obj, 0, 2) && try_enter_state_07_move_0_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 4, 3) && func_801b0a38_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 1, 0) && func_801b0ab4_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 2, 1) && func_801b0b2c_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 3, 2) && func_801b0ba8_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 8, 7) && func_801b0c24_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 5, 4) && func_801b0ca0_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 6, 5) && func_801b0d18_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 7, 6) && func_801b0d94_slot04_08(obj)) return;
    if ((u8)func_8013de2c(obj, 0xc, 0xd) && func_801b108c_slot04_08(obj)) return;
    if ((u8)func_8013de2c(obj, 0xd, 0xe)) func_801b1144_slot04_08(obj);
}
