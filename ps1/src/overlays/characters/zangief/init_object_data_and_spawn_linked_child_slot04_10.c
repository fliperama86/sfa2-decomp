/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801b6e08_slot04_10[];
extern u8 data_801ba218_slot04_10[];
extern u32 data_801c68bc_slot04_10[];
extern ObjectFn data_801c695c_slot04_10[];
extern ObjectFn data_801c6964_slot04_10[];
extern ObjectFn data_801c696c_slot04_10[];
extern ObjectFn data_801c6974_slot04_10[];
extern ObjectFn data_801c697c_slot04_10[];

Object *pop_object_from_second_pool(void);
int match_pressed_direction_input(Object *object);
int try_set_direction_from_pressed_bits(Object *object);
void set_sequence_and_tables_from_direction(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);

void set_states_1_2_0_0_slot04_10(Object *obj);
void call_table_function_by_state_3_2_slot04_10(Object *obj);
void call_table_function_by_state_3_slot04_10(Object *obj);
void call_state_3_dispatch_by_input_dir_second_a_slot04_10(Object *obj);
void call_handler_by_input_dir_second_slot04_10(Object *obj);
void call_table_function_by_state_3_4_slot04_10(Object *obj);
void call_table_function_by_state_3_3_slot04_10(Object *obj);
void call_handler_by_step_limit_slot04_10(Object *obj);
void load_velocities_from_table_slot04_10(Object *obj);

void init_object_data_and_spawn_linked_child_slot04_10(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6e08_slot04_10;
    obj->data_ptr_b = data_801ba218_slot04_10;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c68bc_slot04_10[i];
    }
    ref_other.p = pop_object_from_second_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 0x10;
        ref_other.p->parent = obj;
        ref_other.p->field_0c = obj->field_0c;
        ref_other.p->field_0e = obj->field_0e;
        obj->linked_object = (u32)ref_other.p;
        ref_other.p->sound_side = obj->sound_side;
        ref_other.p->image_row = obj->image_row;
        ref_other.p->spawn_x = obj->spawn_x;
        ref_other.p->spawn_y = obj->spawn_y;
        ref_other.p->field_90 = obj->field_90;
        ref_other.p->data_ptr_a = obj->data_ptr_a;
        ref_other.p->data_ptr_b = obj->data_ptr_b;
    }
}

void dispatch_by_input_dir_0_slot04_10(Object *obj) {
    if (obj->input_dir_0 == 4) {
        call_handler_by_step_limit_slot04_10(obj);
    } else if (obj->input_dir_0 != 0) {
        call_state_3_dispatch_by_input_dir_second_a_slot04_10(obj);
    } else {
        call_handler_by_input_dir_second_slot04_10(obj);
    }
}

void call_handler_by_input_dir_second_slot04_10(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_2_slot04_10(obj);
    } else {
        call_table_function_by_state_3_slot04_10(obj);
    }
}

void call_table_function_by_state_3_slot04_10(Object *obj) {
    data_801c695c_slot04_10[obj->state_3](obj);
}

void check_range_or_set_sequence_a_slot04_10(Object *obj) {
    int one;

    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
        set_states_1_2_0_0_slot04_10(obj);
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        if (obj->direction_index == 4) {
            obj->field_278 = one;
        }
    }
}

void set_states_1_2_0_0_slot04_10(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void update_sequence_with_x_nudge_and_input_check_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        load_velocities_from_table_slot04_10(obj);
    } else {
        if ((t & 1) != 0) {
            s16 d = 3;
            if (obj->facing_side == 0) {
                d = -3;
            }
            obj->pos_x = d + obj->pos_x;
        }
        if ((obj->step_flags & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_2_slot04_10(Object *obj) {
    data_801c6964_slot04_10[obj->state_3](obj);
}

void check_range_or_set_sequence_b_slot04_10(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) == 0) {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
        return;
    }
    if (obj->direction_index != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
        set_states_1_2_0_0_slot04_10(obj);
        return;
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_with_input_check_or_end_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        load_velocities_from_table_slot04_10(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_state_3_dispatch_by_input_dir_second_a_slot04_10(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_4_slot04_10(obj);
    } else {
        call_table_function_by_state_3_3_slot04_10(obj);
    }
}

void call_table_function_by_state_3_3_slot04_10(Object *obj) {
    data_801c696c_slot04_10[obj->state_3](obj);
}

void clear_active_flag_hit_when_bit_8000_set_slot04_10(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 0) {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
        return;
    }
    if ((obj->input_bits_2 & 0x8000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
            set_states_1_2_0_0_slot04_10(obj);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_with_input_check_or_end_2_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_4_slot04_10(Object *obj) {
    data_801c6974_slot04_10[obj->state_3](obj);
}

void advance_state_3_and_set_sequence_if_input_bit_slot04_10(Object *obj) {
    unsigned a;

    obj->active_flag_3 = 1;
    obj->state_3 = obj->state_3 + 1;
    a = obj->direction_index;
    obj->active_flag = 0;
    if (a == 0 || (obj->input_bits_2 & 0x2000) == 0) {
        set_sequence_and_tables_from_direction(obj);
    } else {
        set_sequence_by_side_and_mode(obj, (a >> 1) + 0x55);
    }
}

void update_sequence_with_input_check_or_end_3_slot04_10(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_step_limit_slot04_10(Object *obj) {
    data_801c697c_slot04_10[obj->step_limit](obj);
}
