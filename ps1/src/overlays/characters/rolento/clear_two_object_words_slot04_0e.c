/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c60fc_slot04_0e[];
extern ObjectFn data_801c6108_slot04_0e[];
extern ObjectFn data_801c6110_slot04_0e[];
extern ObjectFn data_801c611c_slot04_0e[];

void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
Block172 *pop_block_from_pool(void);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_sequence_by_side(Object *object, int arg);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void push_other_by_vel_x(Object *object);
void init_object_after_hit_and_spawn_three_effects(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);

void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
void apply_step_flags_byte_to_pos_x_slot04_0e(Object *obj);
void clear_two_object_words_slot04_0e(Object *obj);
void save_counter_or_count_down_then_push_other_slot04_0e(Object *obj);
void clear_80_bytes_at_tail_slot04_0e(Object *obj);

void clear_two_object_words_slot04_0e(Object *obj) {
    int z = 0;
    u32 *p = (u32 *)((u8 *)obj + 0x2b0);
    u8 i = 0;

    do {
        *p++ = z;
        i++;
    } while (i < 2);
}

void call_table_function_by_state_3_14_slot04_0e(Object *obj) {
    data_801c60fc_slot04_0e[obj->state_3](obj);
}

void decrement_remaining_start_seq_sixty_six_slot04_0e(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    set_sequence_by_side_and_mode(obj, 0x42);
}

void spawn_object_on_step_flag_then_update_sequence_slot04_0e(Object *obj) {
    Object *p;
    int t;
    u8 d;
    if (*(u8 *)&obj->step_flags == 1) {
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x3b;
            p->table_index = 6;
            p->ot_depth = 0;
            p->field_02 = 0x14;
            p->team_id = obj->team_id;
            p->parent = obj;
            p->facing_side = obj->facing_side;
            t = 0x33;
            if (obj->facing_side == 0) {
                t = -0x33;
            }
            p->pos_x = t + obj->pos_x;
            p->pos_y = ((Slot04bObj *)obj)->ground_y - 0x3b;
            *(u8 *)&obj->step_flags = 0;
            obj->state_3++;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            d = obj->image_row;
            p->mode_id = 0x20;
            p->image_row = d;
            p->field_90 = obj->field_90;
            p->sound_side = obj->sound_side;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
    update_sequence(obj);
}

void update_sequence_or_clear_and_reset_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags > 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_15_slot04_0e(Object *obj) {
    data_801c6108_slot04_0e[obj->state_3](obj);
}

void start_seq_sixty_four_with_offset_effect_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->table_value = 0x19;
    obj->field_157 = 0;
    obj->state_3++;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x40);
}

void update_sequence_or_finish_state_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
        apply_step_flags_byte_to_pos_x_slot04_0e(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void call_table_function_by_state_3_16_slot04_0e(Object *obj) {
    data_801c6110_slot04_0e[obj->state_3](obj);
}

void start_seq_sixty_five_with_offset_effect_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->table_value = 0x19;
    obj->field_157 = 0;
    obj->state_3++;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x41);
}

void launch_on_negative_step_flags_slot04_0e(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags < 0) {
        obj->active_state = 1;
        obj->vel_x = 0xc0000;
        obj->accel_x = -0x2000;
        obj->vel_y = 0x40000;
        obj->accel_y = -0x8000;
        obj->state_3++;
        *(u8 *)&obj->step_flags = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
    }
}

void move_and_land_restore_facing_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if (obj->ground_y < obj->pos_y) {
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->state_3++;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        play_object_sound_by_kind(obj);
        obj->facing_side = obj->saved_facing;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void call_table_function_by_state_3_17_slot04_0e(Object *obj) {
    data_801c611c_slot04_0e[obj->state_3](obj);
}

void start_sequence_by_active_state_slot04_0e(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void start_effects_on_mode_bits_then_update_slot04_0e(Object *obj) {
    s16 a;
    int b;

    if (((Slot04bObj *)obj)->mode_bits != 0) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->countdown = 1;
        obj->state_3++;
        if (obj->active_state == 0) {
            a = -0x3b;
            b = 0x51;
        } else {
            a = -0xf;
            b = 0x54;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_80_bytes_at_tail_slot04_0e(obj);
    }
    update_sequence(obj);
}

void count_down_then_set_cursor_timer_slot04_0e(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        obj->state_3++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void count_down_update_sequence_or_finish_slot04_0e(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown != 0) {
        save_counter_or_count_down_then_push_other_slot04_0e(obj);
    } else {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        init_object_after_hit_and_spawn_three_effects(obj);
    }
}
