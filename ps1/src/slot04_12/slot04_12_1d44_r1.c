/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4b80_slot04_12[];
extern ObjectFn data_801c4b8c_slot04_12[];
extern s32 data_801c4b9c_slot04_12[];
extern ObjectFn data_801c4bcc_slot04_12[];
extern s32 data_801c4bdc_slot04_12[];
extern u8 data_801c4c0c_slot04_12[];

void start_action_if_buttons_pressed(Object *object);
void update_meter_by_mode_or_table(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
s32 apply_facing_velocity_with_x_stop_slot04_12(Object *object);

void update_slide_until_step_flag_slot04_12(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 0;
        obj->state_3++;
    } else {
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    update_sequence(obj);
}

void copy_vel_y_to_vel_x_on_step_end_slot04_12(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xb);
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    update_sequence(obj);
}

void update_sequence_or_finish_and_reset_other_code_slot04_12(Object *obj) {
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_4_slot04_12(Object *obj) {
    data_801c4b80_slot04_12[obj->state_3](obj);
}

void begin_state_3_sequence_by_half_direction_slot04_12(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x2d;
    if (obj->field_49 != 0) {
        a = 0x49;
    }
    set_sequence_by_side_and_mode(obj, a + (obj->direction_index >> 1));
}

void update_sequence_spawn_linked_child_on_flag_byte_slot04_12(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->countdown = *(u8 *)&obj->countdown + 0x500;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1c;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->health = 0;
            p->parent = obj;
            p->pos_y = y;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
}

void update_countdown_high_byte_and_sequence_slot04_12(Object *obj) {
    s16 t = obj->countdown;

    if (t & 0xff00) {
        t = t - 0x100;
        obj->countdown = t;
        if ((t & 0xff00) == 0) {
            obj->active_flag_4 = 0;
        }
    }
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_5_slot04_12(Object *obj) {
    data_801c4b8c_slot04_12[obj->state_3](obj);
}

void start_table_motion_shifted_by_facing_slot04_12(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 10);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->facing_side != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x + 0x18;
    } else {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x - 0x18;
    }
    obj->vel_x = data_801c4b9c_slot04_12[obj->direction_index * 2];
    obj->accel_x = data_801c4b9c_slot04_12[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c4b9c_slot04_12[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c4b9c_slot04_12[obj->direction_index * 2 + 3];
    play_side_sound_at(obj, obj->side, 7);
    if (obj->field_49 != 0) {
        a = (obj->direction_index >> 1) + 0x4c;
    } else {
        a = obj->direction_index + 0x30;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_advance_on_flag_80_clear_slot04_12(Object *obj) {
    if ((obj->step_flags & 0x80) == 0) {
        obj->active_state = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_air_motion_land_start_sequence_slot04_12(Object *obj) {
    if (apply_facing_velocity_with_x_stop_slot04_12(obj) >= 0) {
        update_sequence(obj);
    } else {
        if (obj->field_49 != 0) {
            obj->accel_y = 0xffff0000;
        } else {
            obj->accel_y = -0x6000;
        }
        if (obj->pos_y < obj->ground_y) {
            update_sequence(obj);
        } else {
            obj->state_3++;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->active_flag_3 = 0;
            obj->active_flag_4 = 0;
            obj->pos_y = (u16)obj->ground_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, obj->direction_index + 0x31);
        }
    }
}

void update_sequence_or_end_via_ref_other_2_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_6_slot04_12(Object *obj) {
    data_801c4bcc_slot04_12[obj->state_3](obj);
}

void start_motion_from_direction_table_seq_half_index_slot04_12(Object *obj) {
    u16 a;

    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown + 0x400;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    obj->vel_x = data_801c4bdc_slot04_12[obj->direction_index * 2];
    obj->accel_x = data_801c4bdc_slot04_12[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c4bdc_slot04_12[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c4bdc_slot04_12[obj->direction_index * 2 + 3];
    a = (obj->direction_index >> 1) + 0x50;
    set_sequence_by_side_and_mode(obj, a);
}

void count_down_high_byte_then_spawn_children_slot04_12(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        obj->countdown = *(u8 *)&obj->countdown + 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -6, 0x4a);
    }
}

void count_down_then_set_other_lock_sound_5_slot04_12(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->other_lock = data_801c4c0c_slot04_12[3];
        } else {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
            obj->other_lock = data_801c4c0c_slot04_12[obj->direction_index >> 1];
        }
        obj->field_165 = 0;
        play_side_sound_at(obj, obj->side, 5);
    }
}

void slide_x_then_end_sequence_reset_slot04_12(Object *obj) {
    u16 t;

    if (obj->vel_x != 0) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x - obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
        }
    }
    t = obj->step_flags;
    if (t & 0xff) {
        obj->step_flags = t & 0xff00;
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
