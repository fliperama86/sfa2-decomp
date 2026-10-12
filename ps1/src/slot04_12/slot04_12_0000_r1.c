/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4de8_slot04_12[];
extern u8 data_801b7604_slot04_12[];
extern u32 data_801c4948_slot04_12[];
extern ObjectFn data_801c49ec_slot04_12[];
extern u8 data_801c49fc_slot04_12[];
extern ObjectFn data_801c4a0c_slot04_12[];

Object *pop_object_from_second_pool(void);
int check_hit_other_active_state_nonzero(Object *object, s16 a, s16 b, s16 c, u16 d);
void update_sequence_with_input_check(Object *object);
void set_sequence_by_side(Object *object, int index);
void set_sequence_and_tables_from_direction(Object *object);

void call_handler_by_input_dir_second_slot04_12(Object *obj);
void func_801b05a8_slot04_12(Object *obj);
void update_state_3_start_move_with_sound_slot04_12(Object *obj);
void func_801b046c_slot04_12(Object *obj);
void func_801b0634_slot04_12(Object *obj);
void update_state_3_start_move_slot04_12(Object *obj);
void func_801b0a40_slot04_12(Object *obj);
void func_801b0904_slot04_12(Object *obj);
void set_sequence_by_direction_and_variant_slot04_12(Object *obj);
int func_801b3a4c_slot04_12(Object *obj);

void init_object_with_two_linked_children_slot04_12(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b4de8_slot04_12;
    obj->data_ptr_b = data_801b7604_slot04_12;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x29; i++) {
        dst[i] = data_801c4948_slot04_12[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x12;
        c->table_index = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->sound_side = obj->sound_side;
        c->state_3 = obj->kind;
        *(u32 *)&obj->linked_object_2 = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x12;
        c->table_index = 1;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->state_3 = obj->kind;
        c->sound_side = obj->sound_side;
        obj->linked_object = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
}

void reset_states_when_step_flag_negative_slot04_12(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_state_2_slot04_12(Object *obj) {
    data_801c49ec_slot04_12[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_slot04_12(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void update_sequence_advance_state_2_on_timer_slot04_12(Object *obj) {
    if (game_state.field_64 == 0 || game_state.countdown_timer == 0) {
        obj->state_2++;
    }
    update_sequence(obj);
}

void start_random_sequence_set_countdown_slot04_12(Object *obj) {
    s16 k;

    obj->countdown = 0x3c;
    obj->state_2++;
    game_state.countdown = 0x1e;
    k = 3;
    if ((s16)obj->health != 0x90) {
        k = data_801c49fc_slot04_12[get_random_byte() & 0xf];
    }
    k = (s8)get_input_mapped_value_or_default(obj, (s8)k);
    k += 0x23;
    set_sequence_by_side(obj, k);
}

void count_down_and_set_side_bit_slot04_12(Object *obj) {
    s16 t = obj->countdown;
    if (t != 0) {
        t = t - 1;
        obj->countdown = t;
        if (t == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}

void dispatch_by_input_dir_0_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b0a40_slot04_12(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b05a8_slot04_12(obj);
    } else {
        call_handler_by_input_dir_second_slot04_12(obj);
    }
}

void call_handler_by_input_dir_second_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        update_state_3_start_move_with_sound_slot04_12(obj);
    } else {
        func_801b046c_slot04_12(obj);
    }
}

void func_801b046c_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
            if ((u8)func_8013f8c4(obj, -0x14, 0xe) != 0) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
            } else {
                obj->active_flag_3 = 1;
                set_sequence_and_tables_from_direction(obj);
            }
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    }
}

void update_state_3_start_move_with_sound_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index == 4) {
            play_side_sound_at(obj, obj->side, 0xb);
        }
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void func_801b05a8_slot04_12(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b0634_slot04_12(obj);
    } else {
        update_state_3_start_move_slot04_12(obj);
    }
}

void update_state_3_start_move_slot04_12(Object *obj) {
    if (obj->state_3 != 0) {
        update_sequence_with_input_check(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void func_801b0634_slot04_12(Object *obj) {
    data_801c4a0c_slot04_12[obj->state_3](obj);
}

void start_jump_on_input_bit_or_default_slot04_12(Object *obj) {
    u8 t = obj->state_3;
    obj->state_3 = t + 1;
    if (obj->direction_index == 4 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->state_3++;
        obj->active_flag_3 = 1;
        add_to_meter_clamped(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = -0x18000;
        }
        obj->pos_y = obj->pos_y - 0x10;
        obj->active_state = 1;
        obj->field_157 = 0;
        reset_direction_and_action_fields(obj);
        obj->field_278 = 1;
        set_sequence_by_side_and_mode(obj, 0x1b);
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void load_velocities_from_table_slot04_12(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b0790_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        if (obj->field_7e == 0) {
            obj->facing_side = obj->facing_side ^ 1;
        }
        play_object_sound_by_kind(obj);
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void set_air_direction_or_range_check_switch_slot04_12(Object *obj) {
    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    if (obj->input_dir_second != 0) {
        func_801b0904_slot04_12(obj);
    } else if (obj->direction_index != 0 && (obj->input_bits_2 & 0xe000) != 0 && obj->pos_y < obj->ground_y - 0x30 && (u8)check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        set_sequence_by_direction_and_variant_slot04_12(obj);
    }
}
