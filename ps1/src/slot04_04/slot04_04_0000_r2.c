/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_start_meter_action_state_8_variant_a_slot04_04(Object *obj);
u8 enter_state_1_0_7_0_type_0_slot04_04(Object *obj);
u8 enter_state_1_0_7_0_type_1_slot04_04(Object *obj);
u8 enter_state_1_0_7_0_type_2_if_unlinked_slot04_04(Object *obj);
u8 try_start_action_state_7_variant_3_slot04_04(Object *obj);
u8 try_start_meter_action_state_8_variant_4_slot04_04(Object *obj);
u8 try_start_meter_action_state_8_variant_5_slot04_04(Object *obj);
u8 try_start_meter_action_state_8_variant_6_slot04_04(Object *obj);
u8 enter_state_1_0_7_0_type_7_if_remaining_slot04_04(Object *obj);
int spend_meter_and_start_variant_eight_lock_slot04_04(Object *obj);
int spend_meter_and_start_variant_nine_lock_slot04_04(Object *obj);

void select_slot04_04_action_from_inputs_slot04_04(Object *obj) {
    if ((u8)is_pressed_bits_in_table(obj) && (u8)try_start_meter_action_state_8_variant_a_slot04_04(obj)) return;
    if ((u8)call_second_table_stage_unless_0c_pair(obj, 0, 0x18) && try_start_meter_action_state_8_variant_6_slot04_04(obj)) return;
    if ((u8)call_third_table_stage_unless_30_pair(obj, 1, 6) && try_start_meter_action_state_8_variant_5_slot04_04(obj)) return;
    if ((u8)call_third_table_stage_unless_c0_pair(obj, 2, 5) && try_start_meter_action_state_8_variant_4_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 3, 0x25) && enter_state_1_0_7_0_type_2_if_unlinked_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a_2(obj, 4, 1) && try_start_action_state_7_variant_3_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 5, 0x13) && enter_state_1_0_7_0_type_1_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a_3(obj, 6, 1) && enter_state_1_0_7_0_type_0_slot04_04(obj)) return;
    if (get_pressed_bit_8(obj) && enter_state_1_0_7_0_type_7_if_remaining_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 7, 0xd) && (u8)spend_meter_and_start_variant_eight_lock_slot04_04(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 8, 0xe)) spend_meter_and_start_variant_nine_lock_slot04_04(obj);
}
