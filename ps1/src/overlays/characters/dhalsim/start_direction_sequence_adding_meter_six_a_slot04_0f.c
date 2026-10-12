/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c5b08_slot04_0f[];
extern ObjectFn data_801c5b18_slot04_0f[];

void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void start_action_if_buttons_pressed(Object *object);
void push_other_by_vel_x(Object *object);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void init_object_after_hit_and_spawn_three_effects(Object *object);
void set_sequence_by_side(Object *object, int arg);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void clear_two_byte_blocks_of_object_slot04_0f(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_slot04_0f(Object *obj);

void start_direction_sequence_adding_meter_six_a_slot04_0f(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, obj);
    a = 0x3e;
    if (obj->field_49 != 0) {
        a = 0x5a;
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
}

void spawn_linked_object_type_19_on_step_flag_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x19;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&obj->pos_x_frac;
            y = *(s32 *)&obj->pos_y_frac;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = y;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0x11);
        }
    }
}

void wait_link_cleared_then_start_next_sequence_slot04_0f(Object *obj) {
    if (obj->linked_object_3 == 0) {
        obj->active_flag_4 = 0;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x44);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_call_end_handler_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_sequence_0_slot04_0f(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_9_slot04_0f(Object *obj) {
    data_801c5b08_slot04_0f[obj->state_3](obj);
}

void start_direction_sequence_adding_meter_six_b_slot04_0f(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, obj);
    a = 0x41;
    if (obj->field_49 != 0) {
        a = 0x5d;
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
}

void spawn_linked_object_table2_on_step_flag_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x19;
            p->table_index = 2;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index + 6;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&obj->pos_x_frac;
            y = *(s32 *)&obj->pos_y_frac;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = y;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->input_matched_flag = 1;
            obj->linked_object_3 = (s32)p;
            obj->countdown = 3;
            obj->link_flags++;
        }
        play_side_sound_at(obj, obj->side, 0x11);
    }
}

void wait_link_cleared_then_advance_state_sequence_slot04_0f(Object *obj) {
    if (obj->linked_object_3 == 0) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x45);
    } else {
        update_sequence(obj);
    }
}

void update_with_countdown_or_finish_slot04_0f(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags < 0) {
        call_reset_object_to_state_1_0_sequence_0_slot04_0f(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t == 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(obj);
    tail:
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_10_slot04_0f(Object *obj) {
    data_801c5b18_slot04_0f[obj->state_3](obj);
}

void advance_state_3_set_sequence_by_active_state_slot04_0f(Object *obj) {
    int a;
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter((GameState *)game_state.config, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void start_effects_on_step_flag_high_byte_slot04_0f(Object *obj) {
    int a = 0;
    int b = 0x55;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state == 0) {
            a = 2;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_two_byte_blocks_of_object_slot04_0f(obj);
    }
    update_sequence(obj);
}

void count_down_then_set_cursor_timer_and_push_other_slot04_0f(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        a = ((Slot04bObj *)obj)->field_c6;
        obj->state_3++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        a += 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_countdown_with_press_count_slot04_0f(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown == 0) {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        init_object_after_hit_and_spawn_three_effects(obj);
    } else {
        if (obj->pressed_bits != 0) {
            if (((Slot04bObj *)obj)->saved_count == 0) {
                ((Slot04bObj *)obj)->saved_count = obj->hit_count_3;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        push_other_by_vel_x(obj);
    }
}
