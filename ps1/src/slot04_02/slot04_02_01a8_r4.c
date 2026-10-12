/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void advance_state_3_and_start_sequence_slot04_02(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void func_801b0714_slot04_02(Object *obj) {
    u16 x;

    obj->input_dir_0 = 4;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    x = 0xc;
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    x += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, x);
    if ((u16)obj->pos_y < (u16)(obj->ground_y - 0x50) && obj->variant_index != 0 && (obj->variant_index & 0x80) == 0 && obj->input_dir_second != 0 && obj->direction_index == 2 && (obj->input_bits_2 & 0x4000) != 0) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 5, 0);
        set_sequence_by_side_and_mode(obj, 0x29);
    }
}
