/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void start_action_if_buttons_pressed(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);
Pooled *alloc_pooled_b(void);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void apply_vel_y_with_accel_y_override_slot04_0c(Object *obj);

extern ObjectFnInt data_801bd8f8_slot04_0c[];
extern ObjectFn data_801bd92c_slot04_0c[];
extern ObjectFn data_801bd960_slot04_0c[];
extern ObjectFn data_801bd9c4_slot04_0c[];
extern u16 data_801bd974_slot04_0c[];
extern s32 data_801bd994_slot04_0c[];
extern u16 data_801a2944[];
extern u8 data_801ad398;

void start_move_sequence_19_lock_other_slot04_0c(Object *obj) {
    u8 t;

    t = 1;
    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->field_15a = 5;
    obj->other_lock = 0x18;
    obj->facing_side = t;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x14;
    set_sequence_by_side_and_mode(obj, 0x19);
}

int try_enter_state_7_kind_1_unlinked_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->link_flags == 0) {
        if (try_action_clearing_both_flags(obj) != 0) {
            r = 1;
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 1;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            set_direction_index_from_buttons_group_a(obj);
        }
    }
    return r;
}

int try_enter_state_07_move_0_slot04_0c(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

int try_enter_state_7_kind_3_slot04_0c(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

void func_801b1088_slot04_0c(Object *obj) {
    data_801ad398 = data_801bd8f8_slot04_0c[obj->field_15a](obj);
}

int is_link_flags_zero_slot04_0c(Object *obj) {
    return obj->link_flags == 0;
}

int return_true_always_slot04_0c(Object *obj) {
    return 1;
}

int has_meter_and_no_link_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->link_flags == 0) {
        r = (s16)obj->meter >= 0x30;
    }
    return r;
}

int is_meter_at_least_threshold_slot04_0c(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

void func_801b1120_slot04_0c(Object *obj) {
    data_801bd92c_slot04_0c[obj->field_15a](obj);
}

void call_table_function_by_state_3_2_slot04_0c(Object *obj) {
    data_801bd960_slot04_0c[obj->state_3](obj);
}

void start_direction_launch_with_random_palette_job_slot04_0c(Object *o) {
    int a = 0x1c;
    Job *job;
    u16 *dst;
    u16 *src;
    int i;
    u8 idx;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    if ((get_random_byte() & 7) == 0) {
        o->other_lock = 0xff;
        if (o->field_49 == 0) {
            job = (Job *)alloc_pooled_b();
            if (job != 0) {
                job->mode = 2;
                job->kind = 1;
                src = (u16 *)(job->src = (u8 *)data_801bd974_slot04_0c);
                job->rect.x = 0x60;
                job->rect.y = o->image_row + 0x1e0;
                job->rect.w = 0x10;
                job->rect.h = 1;
                dst = data_801a2944;
                if (o->side == 0) {
                    dst += 0x50;
                }
                for (i = 0; i < 0x10; i++) {
                    *dst++ = *src++;
                }
            }
        }
    }
    idx = o->direction_index;
    o->vel_x = data_801bd994_slot04_0c[idx * 2];
    o->vel_y = data_801bd994_slot04_0c[idx * 2 + 1];
    o->accel_x = data_801bd994_slot04_0c[idx * 2 + 2];
    o->accel_y = data_801bd994_slot04_0c[idx * 2 + 3];
    if (o->field_49 != 0) {
        a = 0x30;
    }
    set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + a);
    play_sound_at(o, o->side, 0x320);
}

void set_side_flag_when_mode_zero_and_update_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->other_lock = 0;
        o->state_3++;
        set_side_flag(o);
    }
    update_sequence(o);
}

void move_x_by_facing_until_vel_x_negative_slot04_0c(Object *obj) {
    apply_vel_y_with_accel_y_override_slot04_0c(obj);
    if (obj->facing_side == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_vertical_motion_until_landing_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    apply_vel_y_with_accel_y_override_slot04_0c(o);
    if (o->pos_y < o->ground_y) {
        if (obj->mode != 0) {
            return;
        }
    } else {
        o->state_3++;
        o->active_state = 0;
        o->active_flag_3 = 0;
        o->active_flag_4 = 0;
        o->pos_y = (u16)o->ground_y;
        play_object_sound_by_kind(o);
    }
    update_sequence(o);
}

void update_sequence_set_other_state_code_slot04_0c(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        obj->other->other_state_code = 5;
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void apply_vel_y_with_accel_y_override_slot04_0c(Object *obj) {
    if (obj->field_49 != 0 && obj->vel_y < 0) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}

void call_table_function_by_state_3_3_slot04_0c(Object *obj) {
    data_801bd9c4_slot04_0c[obj->state_3](obj);
}

void start_direction_sequence_gain_meter_five_slot04_0c(Object *obj) {
    int a = 0x1f;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x36;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
