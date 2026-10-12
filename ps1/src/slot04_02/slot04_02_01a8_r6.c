/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b0de0_slot04_02(Object *obj);
u8 func_801b0c0c_slot04_02(Object *obj);
u8 try_start_meter_full_state_slot04_02(Object *obj);
u8 func_801b15e8_slot04_02(Object *obj);
u8 try_start_meter_move_5_unlinked_slot04_02(Object *obj);
u8 try_start_meter_move_7_slot04_02(Object *obj);
u8 try_enter_move_state_with_same_variant_slot04_02(Object *obj);
u8 try_start_action_08_opposite_variant_slot04_02(Object *obj);
u8 func_801b1314_slot04_02(Object *obj);
u8 func_801b0d08_slot04_02(Object *obj);
u8 func_801b1698_slot04_02(Object *obj);
u8 func_801b1384_slot04_02(Object *obj);
u8 func_801b148c_slot04_02(Object *obj);
u8 func_801b0d74_slot04_02(Object *obj);
u8 func_801b10ac_slot04_02(Object *obj);
u8 func_801b11e0_slot04_02(Object *obj);

void select_slot04_02_action_from_inputs_slot04_02(Object *obj) {
    if (get_pressed_bit_8(obj) && func_801b0de0_slot04_02(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && func_801b0c0c_slot04_02(obj)) return;
    if (func_8013d158(obj, 0, 0x1e) && try_start_meter_full_state_slot04_02(obj)) return;
    if ((u8)func_8013cc48(obj, 1, 0x18) && func_801b15e8_slot04_02(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x26) && try_start_meter_move_5_unlinked_slot04_02(obj)) return;
    if ((u8)func_8013cbdc(obj, 3, 0x14) && try_start_meter_move_7_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0x1b) && try_enter_move_state_with_same_variant_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0x1c) && try_start_action_08_opposite_variant_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 4) && func_801b1314_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xf) && func_801b0d08_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0x12) && func_801b1698_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 9, 0) && func_801b1384_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 10, 3) && func_801b148c_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 11, 2) && func_801b0d74_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 12, 0xe) && func_801b10ac_slot04_02(obj)) return;
    if ((u8)func_8013cac8(obj, 13, 0xd)) func_801b11e0_slot04_02(obj);
}
