/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801b6e9c_slot04_0f[];
extern u8 data_801b9aa8_slot04_0f[];
extern u32 data_801c58c0_slot04_0f[];
extern ObjectFn data_801c5960_slot04_0f[];
extern ObjectFn data_801c5968_slot04_0f[];
extern ObjectFn data_801c5974_slot04_0f[];
extern ObjectFn data_801c5980_slot04_0f[];
extern u8 data_801c5994_slot04_0f;
extern u8 data_801c5998_slot04_0f;
extern u8 data_801c599c_slot04_0f;
extern u8 data_801c59a0_slot04_0f;
extern ObjectFn data_801c59a4_slot04_0f[];
extern u8 data_801c59b0_slot04_0f[];
extern u8 data_801c59b4_slot04_0f;
extern ObjectFn data_801c59b8_slot04_0f[];
extern u8 data_801c59c4_slot04_0f[];
extern u8 data_801c59c8_slot04_0f[];
extern ObjectFn data_801c59cc_slot04_0f[];
extern u8 data_801c59d8_slot04_0f[];
extern u8 data_801c59dc_slot04_0f[];
extern ObjectFn data_801c59e0_slot04_0f[];

Block172 *pop_block_from_pool(void);
void set_sequence_by_side(Object *object, int index);
void set_sequence_and_tables_from_direction(Object *object);
void update_sequence_with_input_check(Object *object);
void spawn_object_for_kind_0f_player_slot06_0f(void);

void call_handler_by_direction_index_slot04_0f(Object *obj);
void call_table_function_by_state_3_3_slot04_0f(Object *obj);
void call_table_function_by_state_3_4_slot04_0f(Object *obj);
void call_side_routine_and_set_side_flag_slot04_0f(Object *obj);
void update_mode_change_select_row_slot04_0f(Object *obj);
void update_sequence_check_input_or_reset_slot04_0f(Object *obj);

void init_object_data_ptrs_and_copy_scratch_slot04_0f(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6e9c_slot04_0f;
    obj->data_ptr_b = data_801b9aa8_slot04_0f;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c58c0_slot04_0f[i];
    }
    if (game_state.config->compare_value == 0xf) {
        if (obj->other->kind != 0xf || obj->side == 0) {
            spawn_object_for_kind_0f_player_slot06_0f();
        }
    }
}

void call_handler_by_state_2_slot04_0f(Object *obj) {
    data_801c5960_slot04_0f[obj->state_2](obj);
}

void spawn_child_object_copying_position_slot04_0f(Object *obj) {
    Object *c;

    obj->state_2 = obj->state_2 + 1;
    c = (Object *)pop_block_from_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x21;
        c->table_index = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        c->pos_x = obj->pos_x;
        c->pos_y = obj->pos_y;
        c->facing_side = obj->facing_side;
        c->ot_depth = 6;
        c->field_1c = obj->field_1c;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->image_row = obj->image_row;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->mode_id = 0x20;
        c->field_02 = 2;
        c->sound_side = obj->sound_side;
    }
}

void reset_states_when_sequence_ends_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_input_dir_0_slot04_0f(Object *obj) {
    data_801c5968_slot04_0f[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_input_dir_second_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_3_slot04_0f(obj);
    } else {
        call_handler_by_direction_index_slot04_0f(obj);
    }
}

void call_handler_by_direction_index_slot04_0f(Object *obj) {
    data_801c5974_slot04_0f[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_slot04_0f(Object *obj) {
    data_801c5980_slot04_0f[obj->state_3](obj);
}

void select_sequence_by_input_bits_a_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    u16 t;

    o->active_flag_3 = 1;
    t = o->input_bits_2;
    obj->field_337 = 0;
    o->state_3 = o->state_3 + 1;
    p = &obj->saved_pos_x_frac;
    if ((t & 0x8000) != 0) {
        set_sequence_and_tables_from_direction(o);
    } else if ((t & 0x2000) != 0 && (t & 0x80) != 0) {
        o->state_3 = 3;
        obj->countdown_timer = 0x21;
        set_sequence_by_side_and_mode(o, 0x60);
    } else {
        o->state_3 = o->state_3 + 1;
        p[7] = 0xff;
        reset_direction_and_action_fields(o);
        o->kind_table_value = data_801c5994_slot04_0f;
        o->countdown_5 = data_801c5998_slot04_0f;
        set_sequence_by_side_and_mode(o, 0x1a);
    }
}

void call_update_sequence_with_input_check_slot04_0f(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_update_sequence_check_input_or_reset_slot04_0f(Object *obj) {
    update_sequence_check_input_or_reset_slot04_0f(obj);
}

void update_countdown_then_advance_state_3_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown_timer--;
    if (obj->countdown_timer == 0) {
        o->state_3 = o->state_3 + 1;
        set_sequence_by_side_and_mode(o, 0x61);
        obj->field_339 = 0;
        o->sequence_hold_flag = 1;
        o->step_flags = o->step_flags & 0xff00;
        update_mode_change_select_row_slot04_0f(o);
    } else if ((o->input_bits_2 & 0x80) == 0) {
        o->state_3 = o->state_3 + 1;
        o->kind_table_value = data_801c599c_slot04_0f;
        o->countdown_5 = data_801c59a0_slot04_0f;
        set_sequence_by_side_and_mode(o, 0x1a);
    } else {
        update_sequence(o);
    }
}

void set_state_3_two_unless_step_flag_one_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags != 1) {
        obj->state_3 = 2;
        call_side_routine_and_set_side_flag_slot04_0f(obj);
    } else {
        update_sequence_check_input_or_reset_slot04_0f(obj);
    }
}

void call_table_function_by_state_3_2_slot04_0f(Object *obj) {
    data_801c59a4_slot04_0f[obj->state_3](obj);
}

void start_move_or_switch_state_by_input_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    int k;

    o->state_3 = o->state_3 + 1;
    p = &obj->saved_pos_x_frac;
    obj->field_337 = 0;
    if (o->direction_index != 0 && (o->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(o, -0x14, 0x14) != 0) {
        o->state_0 = 1;
        o->state_1 = 2;
        o->state_2 = 0;
        o->state_3 = 0;
    } else {
        o->active_flag_3 = 1;
        if ((o->input_bits_2 & 0x2000) == 0) {
            set_sequence_and_tables_from_direction(o);
        } else {
            o->state_3 = o->state_3 + 1;
            reset_direction_and_action_fields(o);
            p[7] = 0xff;
            k = o->direction_index >> 1;
            add_to_meter_clamped(o, k);
            o->kind_table_value = data_801c59b0_slot04_0f[k];
            o->countdown_5 = data_801c59b4_slot04_0f;
            set_sequence_by_side_and_mode(o, k + 0x1a);
        }
    }
}

void call_update_sequence_with_input_check_2_slot04_0f(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_update_sequence_check_input_or_reset_2_slot04_0f(Object *obj) {
    update_sequence_check_input_or_reset_slot04_0f(obj);
}

void call_table_function_by_state_3_3_slot04_0f(Object *obj) {
    data_801c59b8_slot04_0f[obj->state_3](obj);
}

void start_sequence_from_direction_or_meter_input_a_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->active_flag_3 = 1;
    o->state_3 = o->state_3 + 1;
    obj->field_337 = 0;
    if ((o->input_bits_2 & 0x2000) == 0) {
        set_sequence_and_tables_from_direction(o);
    } else {
        o->state_3 = o->state_3 + 1;
        reset_direction_and_action_fields(o);
        k = o->direction_index >> 1;
        obj->field_337 = 0xff;
        add_to_meter_clamped(o, k);
        o->kind_table_value = data_801c59c4_slot04_0f[k];
        o->countdown_5 = data_801c59c8_slot04_0f[k];
        set_sequence_by_side_and_mode(o, k + 0x1d);
    }
}

void call_update_sequence_with_input_check_3_slot04_0f(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_update_sequence_check_input_or_reset_3_slot04_0f(Object *obj) {
    update_sequence_check_input_or_reset_slot04_0f(obj);
}

void call_state_3_handler_or_alt_by_input_dir_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        data_801c59cc_slot04_0f[obj->state_3](obj);
    } else {
        call_table_function_by_state_3_4_slot04_0f(obj);
    }
}

void start_sequence_from_direction_or_meter_input_b_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->active_flag_3 = 1;
    o->state_3 = o->state_3 + 1;
    obj->field_337 = 0;
    if ((o->input_bits_2 & 0x2000) == 0) {
        set_sequence_and_tables_from_direction(o);
    } else {
        obj->field_337 = 0xff;
        o->state_3 = o->state_3 + 1;
        reset_direction_and_action_fields(o);
        k = o->direction_index >> 1;
        add_to_meter_clamped(o, k);
        o->kind_table_value = data_801c59d8_slot04_0f[k];
        o->countdown_5 = data_801c59dc_slot04_0f[k];
        set_sequence_by_side_and_mode(o, k + 0x20);
    }
}

void call_update_sequence_with_input_check_4_slot04_0f(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_update_sequence_check_input_or_reset_4_slot04_0f(Object *obj) {
    update_sequence_check_input_or_reset_slot04_0f(obj);
}

void call_table_function_by_state_3_4_slot04_0f(Object *obj) {
    data_801c59e0_slot04_0f[obj->state_3](obj);
}
