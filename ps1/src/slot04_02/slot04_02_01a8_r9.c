/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void set_input_dir_second_from_buttons_slot04_02(Object *obj);

u8 try_start_action_08_opposite_variant_slot04_02(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);
    u8 t;

    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        t = obj->saved_facing;
        obj->field_15a = 8;
        obj->facing_side = t;
        obj->variant_index = t ^ 1;
        set_input_dir_second_from_buttons_slot04_02(obj);
    }
    return r;
}

u8 try_enter_move_state_with_same_variant_slot04_02(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 8;
        obj->variant_index = obj->facing_side = obj->saved_facing;
        set_input_dir_second_from_buttons_slot04_02(obj);
    }
    return r;
}

void set_input_dir_second_from_buttons_slot04_02(Object *obj) {
    u16 t;

    t = obj->pressed_bits | obj->released_bits;
    obj->input_dir_second = 0;
    obj->direction_index = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->input_dir_second = obj->input_dir_second + 2;
    }
}
