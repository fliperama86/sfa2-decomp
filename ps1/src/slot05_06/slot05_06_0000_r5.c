/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_3_slot05_06(Object *obj);
void call_handler_by_direction_index_4_slot05_06(Object *obj);
extern ObjectFn data_801dd258_slot05_06[];

void func_801c8510_slot05_06(Object *obj) {
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

void shift_pos_x_and_advance_state_3_on_flag_slot05_06(Object *obj) {
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

void update_sequence_or_finish_sequence_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_direction_handler_by_input_dir_second_slot05_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot05_06(obj);
    } else {
        call_handler_by_direction_index_3_slot05_06(obj);
    }
}

void call_handler_by_direction_index_3_slot05_06(Object *obj) {
    data_801dd258_slot05_06[obj->direction_index >> 1](obj);
}
