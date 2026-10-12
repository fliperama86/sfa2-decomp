/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4824_slot04_0c[];
extern u8 data_801b5bf4_slot04_0c[];
extern u32 data_801bd828_slot04_0c[];
extern u16 data_801bd8c8_slot04_0c[];
extern ObjectFn data_801bd8cc_slot04_0c[];
extern s16 data_801bd8d8_slot04_0c[];
extern ObjectFn data_801bd8dc_slot04_0c[];
extern ObjectFn data_801bd8e4_slot04_0c[];
extern ObjectFn data_801bd8f0_slot04_0c[];

Object *pop_object_from_second_pool(void);
void set_sequence_by_side(Object *object, int arg);
void set_sequence_and_tables_from_direction(Object *object);
void update_sequence_with_input_check(Object *object);

void move_x_to_side_target_then_set_sequence_slot04_0c(Object *obj);
void call_handler_by_input_dir_0_2_slot04_0c(Object *obj);
void call_table_function_by_state_3_slot04_0c(Object *obj);
void call_handler_by_state_3_play_flag_sound_slot04_0c(Object *obj);
void call_handler_by_state_3_set_flag_slot04_0c(Object *obj);

void init_object_copy_words_and_spawn_child_slot04_0c(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    if (game_state.count_index == 0) {
        obj->pos_x = data_801bd8c8_slot04_0c[obj->side];
    }
    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b4824_slot04_0c;
    obj->data_ptr_b = data_801b5bf4_slot04_0c;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801bd828_slot04_0c[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0xc;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        obj->linked_object = (u32)c;
        c->sound_side = obj->side;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void call_handler_by_state_2_slot04_0c(Object *obj) {
    data_801bd8cc_slot04_0c[obj->state_2](obj);
}

void start_move_x_and_advance_state_2_slot04_0c(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->vel_x = 0x80000;
    move_x_to_side_target_then_set_sequence_slot04_0c(obj);
}

void move_x_to_side_target_then_set_sequence_slot04_0c(Object *obj) {
    int a = 0x32;

    if (obj->facing_side == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    }
    if (obj->pos_x != data_801bd8d8_slot04_0c[obj->side]) {
        update_sequence(obj);
    } else {
        obj->countdown = 0x48;
        obj->state_2 = obj->state_2 + 1;
        if (obj->other->kind == 0xb) {
            a = 0x33;
        }
        set_sequence_by_side(obj, a);
    }
}

void wait_countdown_then_reset_state_slot04_0c(Object *obj) {
    s16 t;

    update_sequence(obj);
    t = obj->countdown;
    t -= 1;
    obj->countdown = t;
    if (t == 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    }
}

void call_handler_by_input_dir_0_slot04_0c(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_0c(obj);
    } else {
        call_handler_by_state_3_set_flag_slot04_0c(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_0c(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_slot04_0c(obj);
    } else {
        call_handler_by_state_3_play_flag_sound_slot04_0c(obj);
    }
}

void call_table_function_by_state_3_slot04_0c(Object *obj) {
    data_801bd8dc_slot04_0c[obj->state_3](obj);
}

void check_input_range_switch_state_or_advance_slot04_0c(Object *obj) {
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void load_velocities_from_table_slot04_0c(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_state_3_play_flag_sound_slot04_0c(Object *obj) {
    u16 t;

    data_801bd8e4_slot04_0c[obj->state_3](obj);
    t = obj->step_flags;
    if ((t & 0x7f00) != 0) {
        obj->step_flags = t & 0x80ff;
        play_sound_at(obj, obj->side, 0x31f);
    }
}

void start_state_check_direction_4_slot04_0c(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index == 4) {
        obj->state_3 = 2;
        obj->field_278 = one;
    }
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_2_slot04_0c(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b0510_slot04_0c(Object *obj) {
    s16 t = obj->step_flags;
    int d = 0x80000;

    if ((t & 0x8000) != 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff00;
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - d;
            } else {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + d;
            }
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_slot04_0c(Object *obj) {
    obj->field_157 = 1;
    data_801bd8f0_slot04_0c[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_slot04_0c(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_3_slot04_0c(Object *obj) {
    update_sequence_with_input_check(obj);
}

void set_state_3_and_sequence_from_direction_slot04_0c(Object *obj) {
    int x = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + x));
}
