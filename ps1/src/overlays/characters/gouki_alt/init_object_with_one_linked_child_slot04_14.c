/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801b8580_slot04_14[];
extern u8 data_801ba928_slot04_14[];
extern u32 data_801c60c8_slot04_14[];
extern ObjectFn data_801c6168_slot04_14[];
extern ObjectFn data_801c6170_slot04_14[];
extern ObjectFn data_801c617c_slot04_14[];
extern ObjectFn data_801c6184_slot04_14[];
extern ObjectFn data_801c6190_slot04_14[];

Object *pop_object_from_second_pool(void);
Block172 *pop_block_from_pool(void);
void set_sequence_by_side(Object *object, int index);
void set_sequence_and_tables_from_direction(Object *object);
void update_sequence_with_input_check(Object *object);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);

void init_object_with_one_linked_child_slot04_14(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b8580_slot04_14;
    obj->data_ptr_b = data_801ba928_slot04_14;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c60c8_slot04_14[i];
    }
    game_state.check_flag = 0;
    game_state.field_13c = 0;
    obj->field_252 = 0;
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x1a;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->linked_object = (u32)c;
        c->sound_side = s;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void call_handler_by_state_2_slot04_14(Object *obj) {
    data_801c6168_slot04_14[obj->state_2](obj);
}

void advance_state_2_set_side_sequence_slot04_14(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    set_sequence_by_side(obj, 0x30);
}

void update_sequence_spawn_on_flag_or_reset_states_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    Object *c;
    u8 e;

    if (t < 0) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 0, 0);
        set_sequence_by_side(obj, 0);
    } else {
        if ((t & 0xff00) == 0x100) {
            obj->step_flags = t & 0x80ff;
            game_state.state_param = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
            c = (Object *)pop_block_from_pool();
            if (c != 0) {
                c->active_count = 1;
                c->field_02 = 0x7d;
                c->parent = obj;
                c->field_0c = obj->field_0c;
                c->image_row = obj->image_row;
                e = obj->field_0e;
                obj->parent = c;
                c->field_02 = 0x12;
                c->mode_id = 0x20;
                c->field_0e = e;
                c->sound_side = obj->sound_side;
            }
        }
        update_sequence(obj);
    }
}

void call_handler_by_input_dir_0_slot04_14(Object *obj) {
    data_801c6170_slot04_14[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_input_dir_second_slot04_14(Object *obj) {
    data_801c617c_slot04_14[obj->input_dir_second >> 1](obj);
}

void call_table_function_by_state_3_slot04_14(Object *obj) {
    data_801c6184_slot04_14[obj->state_3](obj);
}

void update_range_check_or_seq_27_with_hold_slot04_14(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            set_object_states_0_to_3_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = one;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            reset_direction_and_action_fields(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            set_sequence_by_side_and_mode(obj, 0x27);
            return;
        }
    }
    obj->active_flag_3 = one;
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_slot04_14(Object *obj) {
    update_sequence_with_input_check(obj);
}

void update_sequence_with_step_flag_x_shift_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    int d;

    if (t < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) != 0) {
            d = -0x20000;
            if (obj->facing_side != 0) {
                d = 0x20000;
            }
            *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_2_slot04_14(Object *obj) {
    data_801c6190_slot04_14[obj->state_3](obj);
}

void update_state_or_start_jump_with_velocity_slot04_14(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            set_object_states_0_to_3_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0) {
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            obj->vel_x = -0x36000;
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            reset_direction_and_action_fields(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            obj->active_flag_3 = one;
            set_sequence_by_side_and_mode(obj, 0x40);
            return;
        }
    }
    obj->active_flag_3 = one;
    set_sequence_and_tables_from_direction(obj);
}
