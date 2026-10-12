/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6afc_slot04_0d[];
extern u8 data_801b9910_slot04_0d[];
extern u32 data_801c2894_slot04_0d[];
extern ObjectFn data_801c2934_slot04_0d[];
extern ObjectFn data_801c2940_slot04_0d[];
extern ObjectFn data_801c294c_slot04_0d[];
extern ObjectFn data_801c2954_slot04_0d[];
extern ObjectFn data_801c2960_slot04_0d[];
extern ObjectFn data_801c296c_slot04_0d[];

Object *pop_object_from_second_pool(void);
void set_sequence_by_side(Object *object, int arg);
void set_sequence_and_tables_from_direction(Object *object);
void update_sequence_with_input_check(Object *object);

void call_helper_up_to_twice_on_frame_phase_slot04_0d(Object *obj);
void call_handler_by_input_dir_second_slot04_0d(Object *obj);
void call_handler_by_direction_index_slot04_0d(Object *obj);
void call_handler_by_direction_index_2_slot04_0d(Object *obj);
void call_handler_by_state_3_set_flag_slot04_0d(Object *obj);

void init_object_data_and_spawn_child_slot04_0d(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b6afc_slot04_0d;
    obj->data_ptr_b = data_801b9910_slot04_0d;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c2894_slot04_0d[i];
    }
    if (game_state.count_index == 0) {
        if (obj->side == 0) {
            obj->pos_x = 0x390;
        } else {
            obj->pos_x = 0x170;
        }
        obj->vel_x = 0x80000;
        obj->accel_x = -0x6000;
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0xd;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->linked_object = (u32)c;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->sound_side = s;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void call_handler_by_state_2_slot04_0d(Object *obj) {
    data_801c2934_slot04_0d[obj->state_2](obj);
}

void move_x_until_edge_then_set_sequence_slot04_0d(Object *obj) {
    int d = obj->vel_x;

    if (obj->facing_side != 0) {
        d = -d;
    }
    *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
    if (obj->pos_x == 0x280) {
        obj->state_2 = obj->state_2 + 1;
        play_sound_at(obj, obj->side, 0x324);
        set_sequence_by_side(obj, 0x30);
    } else {
        update_sequence(obj);
    }
}

void update_decelerating_slide_to_fixed_x_slot04_0d(Object *obj) {
    int a = obj->vel_x;

    if (obj->facing_side != 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->state_2 = obj->state_2 + 1;
        obj->pos_x_frac = 0;
        obj->vel_x = 0;
        obj->accel_x = 0;
        if (obj->side == 0) {
            obj->pos_x = 0x220;
        } else {
            obj->pos_x = 0x2e0;
        }
        set_sequence_by_side(obj, 0x31);
    } else {
        update_sequence(obj);
        call_helper_up_to_twice_on_frame_phase_slot04_0d(obj);
    }
}

void reset_states_when_sequence_ends_slot04_0d(Object *obj) {
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

void call_helper_up_to_twice_on_frame_phase_slot04_0d(Object *obj) {
    u32 i;
    s8 r;

    if ((u8)obj->step_flags == 0) {
        i = game_state.frame_counter;
        if (((i + obj->side) & 3) == 0) {
            i = 0;
            do {
                r = func_80148e84(obj);
                if (r == 0) {
                    break;
                }
                i++;
            } while ((u8)i < 2);
        }
    }
}

void call_handler_by_input_dir_0_nonzero_slot04_0d(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_handler_by_state_3_set_flag_slot04_0d(obj);
    } else {
        call_handler_by_input_dir_second_slot04_0d(obj);
    }
}

void call_handler_by_input_dir_second_slot04_0d(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_2_slot04_0d(obj);
    } else {
        call_handler_by_direction_index_slot04_0d(obj);
    }
}

void call_handler_by_direction_index_slot04_0d(Object *obj) {
    data_801c2940_slot04_0d[obj->direction_index >> 1](obj);
}

void func_801b044c_slot04_0d(Object *obj) {
    data_801c294c_slot04_0d[obj->state_3](obj);
}

void func_801b048c_slot04_0d(Object *obj) {
    data_801c2954_slot04_0d[obj->state_3](obj);
}

void func_801b04cc_slot04_0d(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void load_velocities_from_table_slot04_0d(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b0574_slot04_0d(Object *obj) {
    int one;

    obj->state_3 = obj->state_3 + 1;
    if ((obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
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

void shift_pos_x_on_step_flag_advance_state_3_slot04_0d(Object *obj) {
    int d = 0x18;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}

void update_sequence_or_finish_sequence_slot04_0d(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_direction_index_2_slot04_0d(Object *obj) {
    data_801c2960_slot04_0d[obj->direction_index >> 1](obj);
}

void func_801b06d8_slot04_0d(Object *obj) {
    data_801c296c_slot04_0d[obj->state_3](obj);
}

void func_801b0718_slot04_0d(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            reset_direction_and_action_fields(obj);
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
            set_sequence_by_side_and_mode(obj, 0x1f);
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
