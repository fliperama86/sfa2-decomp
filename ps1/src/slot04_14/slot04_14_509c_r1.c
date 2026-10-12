/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c64c0_slot04_14[];
extern u8 data_801c64cc_slot04_14[];
extern u8 data_801c64d4_slot04_14[];
extern ObjectFn data_801c64dc_slot04_14[];

void func_80146960(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
Block172 *pop_block_from_pool(void);
void set_sequence_by_side(Object *object, int index);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void push_other_by_vel_x(Object *object);
void func_80142c70(Object *object);
void nudge_other_x_by_table_every_fourth_tick_slot04_14(Object *object);
void clear_object_byte_block_slot04_14(Object *object);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b5a90_slot04_14(Object *object);
void func_801b5c34_slot04_14(Object *object);

void func_801b509c_slot04_14(Object *obj) {
    update_sequence(obj);
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_49 != 0) {
            obj->active_flag_4 = 0;
            obj->active_state = 0;
        }
        obj->vel_x = 0x20000;
        obj->pos_y = obj->ground_y;
        obj->accel_x = data_801c64c0_slot04_14[obj->direction_index >> 1];
        set_sequence_by_side_and_mode(obj, 0x55);
        play_sound_at(obj, obj->side, 0x324);
    }
}

void update_slide_until_vel_x_negative_slot04_14(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x56);
    } else {
        update_sequence(obj);
        nudge_other_x_by_table_every_fourth_tick_slot04_14(obj);
    }
}

void update_sequence_or_clear_flags_and_reset_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_157 = 0;
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b5278_slot04_14(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->facing_side = obj->facing_side ^ 1;
        obj->pos_y = obj->pos_y + 0x27;
        if (obj->facing_side != 0) {
            obj->pos_x = obj->pos_x - 0x11;
        } else {
            obj->pos_x = obj->pos_x + 0x11;
        }
    }
}

void rise_until_apex_then_start_fall_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->vel_y = 0;
        obj->accel_x = 0;
        obj->vel_x = 0;
        obj->accel_y = -0x9000;
        set_sequence_by_side_and_mode(obj, 0x58);
    } else {
        update_sequence(obj);
    }
}

void update_jump_until_landing_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0 && obj->pos_y >= obj->ground_y) {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        func_80146960(obj);
        game_state.state_param = data_801c64cc_slot04_14[obj->direction_index & 0xfe];
        play_sound_at(obj, obj->side, 0x319);
        set_sequence_by_side_and_mode(obj, 0x59);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_hit_other_and_launch_slot04_14(Object *obj) {
    Object *p;
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        p = obj->other;
        p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, 6, 0xf, *(s16 *)(data_801c64d4_slot04_14 + (obj->direction_index & 0xfe)), 4, 1, 0);
        obj->vel_x = 0x28000;
        obj->vel_y = 0x70000;
        obj->accel_x = 0;
        obj->accel_y = -0x5400;
        if ((s16)p->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x22;
            }
        }
    }
}

void update_airborne_fall_until_ground_slot04_14(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->active_state = 0;
            obj->pos_y = obj->ground_y;
            play_object_sound_by_kind(obj);
            reset_object_to_state_1_0_sequence_0(obj);
            return;
        }
    }
    update_sequence(obj);
}

void update_knockback_arc_land_with_sound_slot04_14(Object *obj) {
    u16 t;
    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        play_sound_at(obj, obj->side, 0x320);
    }
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= obj->ground_y) {
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x5d);
    }
}

void func_801b5708_slot04_14(Object *obj) {
    data_801c64dc_slot04_14[obj->state_3](obj);
}

void func_801b5748_slot04_14(Object *obj) {
    Object *p;
    int a;
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    } else {
        p = (Object *)pop_block_from_pool();
        a = 0x2b;
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x7d;
            p->parent = obj;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_0e = obj->field_0e;
            obj->parent = p;
            p->field_02 = 0x12;
            p->mode_id = 0x20;
            p->sound_side = obj->sound_side;
        }
    }
    set_sequence_by_side(obj, a);
}

void update_sequence_trigger_effects_on_flags_slot04_14(Object *obj) {
    int a;
    int b;
    if ((s16)obj->step_flags != 0) {
        a = 1;
        b = 0x47;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = 0xd;
            b = 0x39;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_object_byte_block_slot04_14(obj);
    }
    update_sequence(obj);
}

void count_down_high_byte_init_timers_slot04_14(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->hit_count_3 = 0;
        obj->countdown = obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_hold_with_input_count_slot04_14(Object *obj) {
    Object *p;
    s16 t;
    u16 u;
    obj->hit_count_3 = obj->hit_count_3 + 1;
    update_sequence(obj);
    u = obj->countdown - 1;
    obj->countdown = u;
    if ((u8)u == 0) {
        if (obj->active_state != 0) {
            set_object_states_0_to_3_slot04_14(obj, 1, 0, 3, 1);
        } else {
            p = obj->parent;
            if (p->field_02 == 0x12) {
                p->state_0 = 2;
                p->state_3 = 0;
                p->state_2 = 0;
                p->state_1 = 0;
            }
        }
        func_80142c70(obj);
    }
    if ((obj->pressed_bits & 0xff00) != 0) {
        if (((Slot04bObj *)obj)->saved_count == 0) {
            ((Slot04bObj *)obj)->saved_count = obj->hit_count_3;
        }
    } else {
        t = obj->countdown;
        if ((t & 0xff00) != 0) {
            obj->countdown = t - 0x100;
        }
        push_other_by_vel_x(obj);
    }
}

void call_handler_by_direction_input_slot04_14(Object *obj) {
    if (obj->input_dir_second == 0 || obj->direction_index != 4) {
        func_801b5a90_slot04_14(obj);
    } else {
        func_801b5c34_slot04_14(obj);
    }
}
