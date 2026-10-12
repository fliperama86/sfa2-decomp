/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern u8 data_801b6e40_slot04_16[];
extern u8 data_801bb5bc_slot04_16[];
extern u32 data_801ca050_slot04_16[];
extern ObjectFn data_801ca0f0_slot04_16[];
extern ObjectFn data_801ca0f8_slot04_16[];
extern ObjectFn data_801ca100_slot04_16[];
extern ObjectFn data_801ca108_slot04_16[];
extern ObjectFn data_801ca110_slot04_16[];
Object *pop_object_from_second_pool(void);
int match_pressed_direction_input(Object *object);
int try_set_direction_from_pressed_bits(Object *object);
void set_states_1_2_0_0_slot04_16(Object *obj);
void call_table_function_by_state_3_2_slot04_16(Object *obj);
void call_table_function_by_state_3_slot04_16(Object *obj);
void call_state_3_dispatch_by_input_dir_second_a_slot04_16(Object *obj);
void call_handler_by_input_dir_second_slot04_16(Object *obj);
void call_table_function_by_state_3_4_slot04_16(Object *obj);
void call_table_function_by_state_3_3_slot04_16(Object *obj);
void call_handler_by_step_limit_slot04_16(Object *obj);
void load_velocities_from_table_slot04_16(Object *obj);
extern ObjectFn data_801ca11c_slot04_16[];
void set_sequence_by_side(Object *object, int index);
void set_sequence_and_tables_from_direction(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
int apply_velocity_and_check_ground(Object *object);
void set_sequence_by_variant_and_input_dir_slot04_16(Object *obj);
void set_states_step_limit_zero_add_meter_sequence_slot04_16(Object *obj);
void set_states_step_limit_one_add_meter_sequence_slot04_16(Object *obj);
void set_states_step_limit_two_add_meter_sequence_slot04_16(Object *obj);
void load_velocities_from_table_3_slot04_16(Object *obj);

void init_data_pointers_copy_scratch_spawn_child_slot04_16(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6e40_slot04_16;
    obj->data_ptr_b = data_801bb5bc_slot04_16;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801ca050_slot04_16[i];
    }
    ref_other.p = pop_object_from_second_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 0x25;
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

void dispatch_by_input_dir_0_slot04_16(Object *obj) {
    if (obj->input_dir_0 == 4) {
        call_handler_by_step_limit_slot04_16(obj);
    } else if (obj->input_dir_0 != 0) {
        call_state_3_dispatch_by_input_dir_second_a_slot04_16(obj);
    } else {
        call_handler_by_input_dir_second_slot04_16(obj);
    }
}

void call_handler_by_input_dir_second_slot04_16(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_2_slot04_16(obj);
    } else {
        call_table_function_by_state_3_slot04_16(obj);
    }
}

void call_table_function_by_state_3_slot04_16(Object *obj) {
    data_801ca0f0_slot04_16[obj->state_3](obj);
}

void check_range_or_set_sequence_a_slot04_16(Object *obj) {
    int one;

    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
        set_states_1_2_0_0_slot04_16(obj);
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        if (obj->direction_index == 4) {
            obj->field_278 = one;
        }
    }
}

void set_states_1_2_0_0_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void update_sequence_with_x_nudge_and_input_check_slot04_16(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        load_velocities_from_table_slot04_16(obj);
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

void call_table_function_by_state_3_2_slot04_16(Object *obj) {
    data_801ca0f8_slot04_16[obj->state_3](obj);
}

void check_range_or_set_sequence_b_slot04_16(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) == 0) {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
        return;
    }
    if (obj->direction_index != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
        set_states_1_2_0_0_slot04_16(obj);
        return;
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_with_input_check_or_end_slot04_16(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        load_velocities_from_table_slot04_16(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_state_3_dispatch_by_input_dir_second_a_slot04_16(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_4_slot04_16(obj);
    } else {
        call_table_function_by_state_3_3_slot04_16(obj);
    }
}

void call_table_function_by_state_3_3_slot04_16(Object *obj) {
    data_801ca100_slot04_16[obj->state_3](obj);
}

void clear_active_flag_hit_when_bit_8000_set_slot04_16(Object *obj) {
    obj->active_flag = 0;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 0) {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
        return;
    }
    if ((obj->input_bits_2 & 0x8000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x25, 0x14) != 0) {
            set_states_1_2_0_0_slot04_16(obj);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_with_input_check_or_end_2_slot04_16(Object *obj) {
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

void call_table_function_by_state_3_4_slot04_16(Object *obj) {
    data_801ca108_slot04_16[obj->state_3](obj);
}

void advance_state_3_and_set_sequence_if_input_bit_slot04_16(Object *obj) {
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

void update_sequence_with_input_check_or_end_3_slot04_16(Object *obj) {
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

void call_handler_by_step_limit_slot04_16(Object *obj) {
    data_801ca110_slot04_16[obj->step_limit](obj);
}

void update_airborne_land_or_sequence_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        play_object_sound_by_kind(obj);
        load_velocities_from_table_3_slot04_16(obj);
    } else {
        update_sequence(obj);
    }
}

void update_airborne_land_load_velocities_sequence_a_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        play_object_sound_by_kind(obj);
        load_velocities_from_table_3_slot04_16(obj);
        if (*(u32 *)&obj->state_0 == 0x2030001) {
            set_sequence_by_side_and_mode(obj, 0x43);
        }
    } else {
        update_sequence(obj);
    }
}

void update_airborne_land_load_velocities_sequence_b_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->pos_y = (u16)obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        play_object_sound_by_kind(obj);
        load_velocities_from_table_3_slot04_16(obj);
        if (*(u32 *)&obj->state_0 == 0x2030001) {
            set_sequence_by_side_and_mode(obj, 0x44);
        }
    } else {
        update_sequence(obj);
    }
}

void select_sequence_setter_by_input_bits_slot04_16(Object *obj) {
    u16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    t = obj->input_bits_2;
    if (obj->variant_index == 0) {
        if ((t & 0x1000) && obj->input_dir_second == 0 && obj->direction_index != 0) {
            set_states_step_limit_two_add_meter_sequence_slot04_16(obj);
        } else {
            set_sequence_by_variant_and_input_dir_slot04_16(obj);
        }
    } else if ((t & 0x4000) == 0) {
        set_sequence_by_variant_and_input_dir_slot04_16(obj);
    } else if (obj->input_dir_second == 0) {
        if (obj->direction_index == 4) {
            set_states_step_limit_zero_add_meter_sequence_slot04_16(obj);
        } else {
            set_sequence_by_variant_and_input_dir_slot04_16(obj);
        }
    } else if (obj->direction_index == 4) {
        set_sequence_by_variant_and_input_dir_slot04_16(obj);
    } else {
        set_states_step_limit_one_add_meter_sequence_slot04_16(obj);
    }
}

void set_sequence_by_variant_and_input_dir_slot04_16(Object *obj) {
    s16 a = 0xc;

    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}

void set_states_step_limit_zero_add_meter_sequence_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 0;
    add_to_meter_clamped(obj, 1);
    set_sequence_by_side_and_mode(obj, 0x4c);
}

void set_states_step_limit_one_add_meter_sequence_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 1;
    add_to_meter_clamped(obj, 1);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x4d);
}

void set_states_step_limit_two_add_meter_sequence_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    obj->step_limit = 2;
    add_to_meter_clamped(obj, 1);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x4e);
}

void call_handler_by_state_2_slot04_16(Object *obj) {
    data_801ca11c_slot04_16[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_16(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_unless_config_hold_slot04_16(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void start_random_choice_sequence_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 tbl[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    int v;

    o->state_2 = o->state_2 + 1;
    o->countdown = 0x50;
    if (game_state.config->countdown_2 == 0) {
        game_state.config->countdown_2 = 0x1e;
    }
    v = (u8)get_input_mapped_value_or_default(o, tbl[get_random_byte() & 0xf]);
    obj->saved_pos_x_frac = v;
    set_sequence_by_side(o, v + 0x23);
}
