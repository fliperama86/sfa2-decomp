/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bda50_slot04_0c[];
extern ObjectFn data_801bda54_slot04_0c[];
extern ObjectFn data_801bda5c_slot04_0c[];

void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_sequence_by_side(Object *object, int arg);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void push_other_by_vel_x(Object *object);
void clear_object_byte_block_slot04_0c(Object *object);

void slide_x_and_set_other_lock_slot04_0c(Object *obj) {
    int i = 0;
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        if (obj->field_165 != 0) {
            obj->field_165 = 0;
            if (obj->field_4b == 0) {
                obj->other->field_6b = 10;
                i = (obj->direction_index >> 1) + 1;
            }
            obj->other_lock = data_801bda50_slot04_0c[i];
        }
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3 = obj->state_3 + 1;
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    }
}

void slide_x_decelerating_until_flag_then_set_rise_slot04_0c(Object *obj) {
    update_sequence(obj);
    if (obj->step_flags & 0x80) {
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        obj->state_3 = obj->state_3 + 1;
    } else {
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    }
}

void rise_until_vel_y_negative_slot04_0c(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->vel_y < 0) {
            obj->state_3 = obj->state_3 + 1;
        }
    }
    update_sequence(obj);
}

void apply_gravity_until_landing_or_step_flag_slot04_0c(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void update_sequence_or_call_finish_on_flag_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_8_slot04_0c(Object *obj) {
    data_801bda54_slot04_0c[obj->state_3](obj);
}

void start_timed_state_input_selects_sequence_slot04_0c(Object *obj) {
    int a = 0x33;
    int s = 3;
    int one = 1;
    int t;
    obj->countdown = 0x48;
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 0;
    obj->active_flag_4 = one;
    obj->field_157 = 0;
    if (obj->mode_flag_2 == 0) {
        t = obj->input_bits & 0x4000;
    } else {
        t = obj->input_dir_second;
    }
    if (t != 0) {
        a = 0x34;
        obj->field_157 = one;
    }
    set_sequence_by_side_and_mode(obj, a);
    if (obj->field_157 == 0) {
        s = s + 1;
    }
    add_to_meter_clamped(obj, s);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
}

void tick_countdown_then_reset_object_by_flag_slot04_0c(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        if (obj->field_157 == 0) {
            reset_object_to_state_1_0_sequence_0(obj);
        } else {
            reset_object_to_state_1_0_0_2_sequence_1(obj);
        }
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_9_slot04_0c(Object *obj) {
    data_801bda5c_slot04_0c[obj->state_3](obj);
}

void start_sequence_by_active_state_with_setup_slot04_0c(Object *obj) {
    int a = 0x2b;
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    }
    set_sequence_by_side(obj, a);
}

void spawn_effects_reset_side_clear_block_on_flag_slot04_0c(Object *obj) {
    int a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = 8;
            b = 0x3f;
        }
        spawn_effect_objects_at_offset(obj, (unsigned)(a << 16) >> 16, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_object_byte_block_slot04_0c(obj);
    }
    update_sequence(obj);
}

void count_down_countdown_high_byte_slot04_0c(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}
