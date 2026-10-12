/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0510_slot04_06(Object *obj) {
    int one;

    obj->state_3 = obj->state_3 + 1;
    if ((obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        obj->field_278 = one;
    }
}

void shift_pos_x_and_advance_state_3_on_flag_slot04_06(Object *obj) {
    int d = -0x10;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side != 0) {
            d = 0x10;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}
