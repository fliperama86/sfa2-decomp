/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void push_other_by_vel_x(Object *object);
void init_object_after_hit_and_spawn_three_effects(Object *object);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void set_sequence_by_side(Object *object, int index);
void call_table_function_by_state_3_10_slot04_11(Object *obj);
void spawn_effect_by_kind_and_active_state_slot04_11(Object *obj);
void spawn_effect_objects_offset_by_active_state_slot04_11(Object *obj);
void clear_two_byte_blocks_of_object_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj);
int apply_facing_velocity_with_x_stop_slot04_11(Object *obj);

extern ObjectFn data_801c77c0_slot04_11[];
extern ObjectFn data_801c77c8_slot04_11[];
extern s32 data_801c77d4_slot04_11[];
extern ObjectFn data_801c77f4_slot04_11[];

void update_sequence_or_load_velocity_table_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_by_kind_or_end_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        call_table_function_by_state_3_10_slot04_11(obj);
    } else if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_10_slot04_11(Object *obj) {
    data_801c77c0_slot04_11[obj->state_3](obj);
}

void set_fixed_velocity_and_advance_state_3_slot04_11(Object *obj) {
    obj->vel_x = 0x50000;
    obj->accel_x = -0x3000;
    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->state_3++;
}

void update_sequence_with_motion_and_x_step_slot04_11(Object *obj) {
    int a;
    u16 b;

    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
        if ((obj->step_flags & 0x80) == 0) {
            apply_facing_velocity_with_x_stop_slot04_11(obj);
        }
        a = obj->step_flags & 0x7f;
        if (a != 0) {
            b = a;
            obj->step_flags = obj->step_flags & 0xff80;
            if (obj->facing_side == 0) {
                b = -a;
            }
            obj->pos_x = b + obj->pos_x;
        }
    }
}

void call_table_function_by_state_3_11_slot04_11(Object *obj) {
    data_801c77c8_slot04_11[obj->state_3](obj);
}

void init_velocity_from_kind_table_slot04_11(Object *obj) {
    int k;

    obj->state_3++;
    k = obj->kind != 0x11;
    obj->vel_x = data_801c77d4_slot04_11[(k << 2) + 0];
    obj->accel_x = data_801c77d4_slot04_11[(k << 2) + 1];
    obj->vel_y = data_801c77d4_slot04_11[(k << 2) + 2];
    obj->accel_y = data_801c77d4_slot04_11[(k << 2) + 3];
    obj->active_state = 1;
    update_sequence(obj);
}

void update_slide_land_on_ground_slot04_11(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0 && apply_facing_velocity_with_x_stop_slot04_11(obj) < 0 && obj->pos_y >= (s16)((Slot04bObj *)obj)->ground_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    }
    update_sequence(obj);
}

void update_sequence_or_end_with_other_code_5_3_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_12_slot04_11(Object *obj) {
    data_801c77f4_slot04_11[obj->state_3](obj);
}

void start_sequence_by_active_state_slot04_11(Object *obj) {
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

void start_special_on_step_flag_high_byte_slot04_11(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->countdown = 1;
        obj->state_3++;
        spawn_effect_by_kind_and_active_state_slot04_11(obj);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_two_byte_blocks_of_object_slot04_11(obj);
        play_side_sound_at(obj, obj->side, 7);
    }
    update_sequence(obj);
}

void countdown_then_init_cursor_timer_slot04_11(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->state_3++;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_countdown_hold_sequence_slot04_11(Object *obj) {
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
        if (((Slot04bObj *)obj)->field_134 != 0) {
            if (((Slot04bObj *)obj)->saved_count == 0) {
                ((Slot04bObj *)obj)->saved_count = obj->hit_count_3;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        push_other_by_vel_x(obj);
    }
}

void spawn_effect_by_kind_and_active_state_slot04_11(Object *obj) {
    int a;
    int b;

    if (obj->kind == 0x13) {
        spawn_effect_objects_offset_by_active_state_slot04_11(obj);
    } else {
        a = -3;
        b = 0x4f;
        if (obj->active_state != 0) {
            a = -0xd;
            b = 0x44;
        }
        spawn_effect_objects_at_offset(obj, a, b);
    }
}

void spawn_effect_objects_offset_by_active_state_slot04_11(Object *obj) {
    if (obj->active_state != 0) {
        spawn_effect_objects_at_offset(obj, -0xe, 0x47);
    } else {
        spawn_effect_objects_at_offset(obj, 8, 0x47);
    }
}
