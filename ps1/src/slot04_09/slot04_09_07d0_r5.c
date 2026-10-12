/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_start_meter_move_type_9_slot04_09(Object *obj);
int try_start_move_7_with_remaining_count_slot04_09(Object *obj);
int func_801b110c_slot04_09(Object *obj);
int func_801b11a4_slot04_09(Object *obj);
int try_start_move_0_unless_linked_slot04_09(Object *obj);
int try_start_move_1_slot04_09(Object *obj);
int try_start_move_2_slot04_09(Object *obj);
int func_801b1474_slot04_09(Object *obj);
int try_start_meter_move_4_slot04_09(Object *obj);
int try_start_meter_move_5_unless_linked_slot04_09(Object *obj);
int try_start_meter_move_6_if_counter_zero_slot04_09(Object *obj);

void select_slot04_09_action_from_inputs_slot04_09(Object *obj) {
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_type_9_slot04_09(obj)) return;
    if (func_8013cc48(obj, 0, 0x15) && try_start_meter_move_6_if_counter_zero_slot04_09(obj)) return;
    if ((u8)func_8013cbdc(obj, 1, 0x14) && try_start_meter_move_4_slot04_09(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x19) && try_start_meter_move_5_unless_linked_slot04_09(obj)) return;
    if (func_8013cac8(obj, 3, 0x10) && try_start_move_0_unless_linked_slot04_09(obj)) return;
    if (func_8013cac8(obj, 4, 2) && try_start_move_1_slot04_09(obj)) return;
    if (func_8013cac8(obj, 5, 4) && try_start_move_2_slot04_09(obj)) return;
    if (func_8013cac8(obj, 6, 0xd) && func_801b110c_slot04_09(obj)) return;
    if (func_8013cac8(obj, 7, 1) && func_801b1474_slot04_09(obj)) return;
    if (func_8013cac8(obj, 8, 0xe) && func_801b11a4_slot04_09(obj)) return;
    if (get_pressed_bit_8(obj)) try_start_move_7_with_remaining_count_slot04_09(obj);
}
