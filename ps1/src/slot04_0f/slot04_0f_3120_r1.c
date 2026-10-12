/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b44_slot04_0f[];
extern u8 data_801c5b5c_slot04_0f[];
extern ObjectFn data_801c5b60_slot04_0f[];
extern u8 data_801c5b94_slot04_0f[];
void set_sequence_by_side(Object *object, int index);
int apply_velocity_and_check_ground(Object *object);
void func_801428e4(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void func_80145d20(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
int check_hit_other_active_state_nonzero(Object *object, s16 a, s16 b, s16 c, u16 d);
void update_mode_change_select_row_slot04_0f(Object *object);
void simulate_landing_x_to_saved_pos_x_slot04_0f(Object *object);
void move_with_facing_flipped_x_slot04_0f(Object *object);

void func_801b3120_slot04_0f(Object *obj) {
    update_sequence(obj);
    update_mode_change_select_row_slot04_0f(obj);
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        simulate_landing_x_to_saved_pos_x_slot04_0f(obj);
        play_side_sound_at(obj, obj->side, 0xf);
    } else if (obj->ground_y < obj->pos_y) {
        apply_velocity_and_check_ground(obj);
    }
}

void func_801b31b0_slot04_0f(Object *obj) {
    update_sequence(obj);
    update_mode_change_select_row_slot04_0f(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_15b = 0;
        obj->step_applied_flag = 0;
        obj->round_count = 0;
        obj->field_69 = 0;
        *(s32 *)&obj->pos_x_frac = *(s32 *)&((Slot04bObj *)obj)->saved_pos_x_frac;
        obj->pos_y = obj->ground_y;
        obj->facing_side = obj->saved_facing;
    }
}

void reset_states_on_negative_step_flags_slot04_0f(Object *obj) {
    update_sequence(obj);
    update_mode_change_select_row_slot04_0f(obj);
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 1;
        obj->state_1 = 1;
        obj->state_2 = 2;
        obj->state_3 = 0;
        obj->active_state = 0;
    }
}

void simulate_landing_x_to_saved_pos_x_slot04_0f(Object *obj) {
    s32 *dst = (s32 *)&((Slot04bObj *)obj)->saved_pos_x_frac;
    s32 x = *(s32 *)&obj->pos_x_frac;
    s32 vx = obj->vel_x;
    s32 y = *(s32 *)&obj->pos_y_frac;
    s32 vy = obj->vel_y;
    s32 ax = obj->accel_x;
    s32 ay = obj->accel_y;
    s16 h = obj->ground_y;

    do {
        x += vx;
        vx += ax;
        y -= vy;
        vy += ay;
    } while (!(h < y));
    *dst = x;
}

void func_801b32c0_slot04_0f(Object *obj) {
    data_801c5b44_slot04_0f[obj->state_3](obj);
}

void func_801b3300_slot04_0f(Object *obj) {
    obj->state_3++;
    obj->countdown = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter((GameState *)game_state.config, obj);
    set_sequence_by_side_and_mode(obj, 0x4a);
}

void func_801b3360_slot04_0f(Object *obj) {
    int a;

    obj->countdown -= 1;
    if (*(u8 *)&obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0xe, 0x60);
    }
    update_sequence(obj);
}

void func_801b33e0_slot04_0f(Object *obj) {
    int a = 0;

    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c5b5c_slot04_0f[a];
        obj->variant_index = 0x16;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x4b);
    } else {
        update_sequence(obj);
    }
}

void spawn_object_type_1b_on_step_flag_slot04_0f(Object *obj) {
    Object *p;
    u8 t;
    s32 w;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1b;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            t = obj->direction_index;
            p->active_flag_2 = 0;
            p->vel_table_index = t;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->scroll_factor = obj->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&obj->pos_x_frac;
            w = *(s32 *)&obj->pos_y_frac;
            p->health = 0xff;
            p->variant_index = 0x16;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = w;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
}

void set_sequence_4e_on_negative_step_flags_slot04_0f(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags < 0) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x4e);
    }
}

void update_sequence_or_finish_2_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3640_slot04_0f(Object *obj) {
    data_801c5b60_slot04_0f[obj->state_3](obj);
}

void func_801b3680_slot04_0f(Object *obj) {
    obj->state_3++;
    obj->countdown = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x4f);
}

void func_801b36e0_slot04_0f(Object *obj) {
    int a;

    obj->countdown -= 1;
    if (*(u8 *)&obj->step_flags != 0) {
        a = -1;
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0, 0x61);
    }
    update_sequence(obj);
}

void count_down_then_set_other_lock_slot04_0f(Object *obj) {
    u8 a = 0;

    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c5b94_slot04_0f[a];
    }
    update_sequence(obj);
}

void launch_with_fixed_velocity_on_step_flag_slot04_0f(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->vel_y = -0x60000;
        obj->accel_x = 0x500;
        obj->accel_y = 0x3000;
        obj->state_3++;
        obj->vel_x = -0x24000;
        obj->step_flags = obj->step_flags & 0xff00;
    }
}

void update_rise_check_hit_or_start_fall_slot04_0f(Object *obj) {
    update_sequence(obj);
    if (obj->vel_y <= 0) {
        move_with_facing_flipped_x_slot04_0f(obj);
        if ((s16)obj->step_flags < 0) {
            if (check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x30)) {
                obj->state_3 = 7;
                play_sound_at(obj, player_left.side, 0x31a);
                play_side_sound_at(obj, obj->side, 0xa);
                set_sequence_by_side_and_mode(obj, 0x51);
            }
        }
    } else {
        obj->vel_x = -0x19b00;
        obj->vel_y = 0x3000;
        obj->accel_x = 0x500;
        obj->accel_y = 0x3000;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x50);
    }
}

void move_with_facing_flipped_x_slot04_0f(Object *obj) {
    int v;

    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    v = obj->vel_x;
    if (obj->facing_side != 0) {
        v = -v;
    }
    *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
}

void func_801b39a0_slot04_0f(Object *obj) {
    update_sequence(obj);
    move_with_facing_flipped_x_slot04_0f(obj);
    if (obj->ground_y <= obj->pos_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    }
}

void update_sequence_or_finish_3_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void start_fall_when_step_flags_negative_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->vel_y = 0x10000;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->accel_y = 0x6000;
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b3aa4_slot04_0f(Object *obj) {
    move_with_facing_flipped_x_slot04_0f(obj);
    if (obj->ground_y <= obj->pos_y) {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        obj->countdown = obj->direction_index >> 1;
        set_sequence_by_side_and_mode(obj, 0x52);
    } else {
        update_sequence(obj);
    }
}
