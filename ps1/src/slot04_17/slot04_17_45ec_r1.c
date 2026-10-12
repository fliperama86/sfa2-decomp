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
void func_80142c70(Object *object);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void add_to_meter_clamped(Object *object, short delta);
void set_sequence_by_side(Object *object, int arg);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void latch_pressed_bits_into_input_bits_slot04_17(Object *obj);
u8 get_random_table_byte_slot04_17(Object *obj);
void func_801b4b18_slot04_17(Object *obj);
void func_801b5100_slot04_17(Object *obj);
void spawn_effect_by_kind_and_active_state_slot04_17(Object *obj);
void clear_two_byte_blocks_slot04_17(Object *obj);
int apply_velocity_and_acceleration_slot04_17(Object *obj);
int apply_facing_velocity_with_x_stop_slot04_17(Object *obj);
void func_801b57f0_slot04_17(Object *obj);
void call_handler_by_state_3_for_kinds_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);

extern u8 data_801ce6cc_slot04_17[];
extern u8 data_801ce6dc_slot04_17[];
extern ObjectFn data_801ce6ec_slot04_17[];
extern ObjectFn data_801ce6f4_slot04_17[];
extern ObjectFn data_801ce6fc_slot04_17[];
extern s32 data_801ce708_slot04_17[];
extern ObjectFn data_801ce728_slot04_17[];
extern ObjectFn data_801ce738_slot04_17[];
extern ObjectFn data_801ce740_slot04_17[];
extern u8 data_801ce74c_slot04_17[];

void update_land_check_step_count_sequence_slot04_17(Object *obj) {
    Config *c;

    latch_pressed_bits_into_input_bits_slot04_17(obj);
    apply_velocity_and_acceleration_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        if (obj->step_count_2 < obj->step_limit && ((c = game_state.config)->mode_flag_b | c->mode_flag_c | c->mode_flag_a) == 0 && func_8012f56c(obj) == 0) {
            obj->state_3 = 4;
            set_sequence_by_side_and_mode(obj, 0x4c);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x6f);
        }
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_end_with_other_code_5_2_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void latch_pressed_bits_into_input_bits_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->mode_flag_2 == 0) {
        if (*(u16 *)&obj->input_bits == 0) {
            if (o->pressed_bits != 0) {
                *(u16 *)&obj->input_bits = o->pressed_bits & 0x68;
            }
        }
    }
}

u8 mirror_value_by_facing_side_slot04_17(Object *obj, int a) {
    if (obj->facing_side != 0) {
        return data_801ce6cc_slot04_17[a >> 3];
    }
    return a;
}

u8 get_random_table_byte_slot04_17(Object *obj) {
    return data_801ce6dc_slot04_17[get_random_byte() & 0xf];
}

void func_801b49a8_slot04_17(Object *obj) {
    call_handler_by_state_3_for_kinds_slot04_17(obj);
}

void func_801b49c8_slot04_17(Object *obj) {
    data_801ce6ec_slot04_17[obj->state_3](obj);
}

void start_state_decrement_remaining_count_slot04_17(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    set_sequence_by_side_and_mode(obj, 0x20);
}

void update_sequence_or_load_velocity_table_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_by_kind_or_end_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        func_801b4b18_slot04_17(obj);
    } else if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4b18_slot04_17(Object *obj) {
    data_801ce6f4_slot04_17[obj->state_3](obj);
}

void set_fixed_velocity_and_advance_state_3_slot04_17(Object *obj) {
    obj->vel_x = 0x50000;
    obj->accel_x = -0x3000;
    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->state_3++;
}

void update_sequence_with_motion_and_x_step_slot04_17(Object *obj) {
    int a;
    u16 b;

    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
        if ((obj->step_flags & 0x80) == 0) {
            apply_facing_velocity_with_x_stop_slot04_17(obj);
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

void func_801b4c40_slot04_17(Object *obj) {
    data_801ce6fc_slot04_17[obj->state_3](obj);
}

void init_velocity_from_kind_table_slot04_17(Object *obj) {
    int k;

    obj->state_3++;
    k = obj->kind != 0x11;
    obj->vel_x = data_801ce708_slot04_17[(k << 2) + 0];
    obj->accel_x = data_801ce708_slot04_17[(k << 2) + 1];
    obj->vel_y = data_801ce708_slot04_17[(k << 2) + 2];
    obj->accel_y = data_801ce708_slot04_17[(k << 2) + 3];
    obj->active_state = 1;
    update_sequence(obj);
}

void update_slide_land_on_ground_slot04_17(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0 && apply_facing_velocity_with_x_stop_slot04_17(obj) < 0 && obj->pos_y >= (s16)((Slot04bObj *)obj)->ground_y) {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    }
    update_sequence(obj);
}

void update_sequence_or_end_with_other_code_5_3_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4e00_slot04_17(Object *obj) {
    data_801ce728_slot04_17[obj->state_3](obj);
}

void start_sequence_by_active_state_slot04_17(Object *obj) {
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

void start_special_on_step_flag_high_byte_slot04_17(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->countdown = 1;
        obj->state_3++;
        spawn_effect_by_kind_and_active_state_slot04_17(obj);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_two_byte_blocks_slot04_17(obj);
        play_side_sound_at(obj, obj->side, 7);
    }
    update_sequence(obj);
}

void countdown_then_init_cursor_timer_slot04_17(Object *obj) {
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

void update_countdown_hold_sequence_slot04_17(Object *obj) {
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
        func_80142c70(obj);
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

void spawn_effect_by_kind_and_active_state_slot04_17(Object *obj) {
    int a;
    int b;

    if (obj->kind == 0x13) {
        func_801b5100_slot04_17(obj);
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

void func_801b5100_slot04_17(Object *obj) {
    if (obj->active_state != 0) {
        spawn_effect_objects_at_offset(obj, -0xe, 0x47);
    } else {
        spawn_effect_objects_at_offset(obj, 8, 0x47);
    }
}

void clear_two_byte_blocks_slot04_17(Object *obj) {
    int z = 0;
    int c;
    int i;
    u8 *p;

    p = (u8 *)obj + 0x2b0;
    for (i = 0, c = z; i < 0x40; i++) {
        *p++ = c;
    }
    p = (u8 *)obj + 0x184;
    for (i = 0; i < 0x18; i++) {
        *p++ = z;
    }
}

void func_801b5178_slot04_17(Object *obj) {
    clear_two_byte_blocks_slot04_17(obj);
}

void call_handler_by_input_dir_second_slot04_17(Object *obj) {
    data_801ce738_slot04_17[obj->input_dir_second >> 1](obj);
}

void func_801b51dc_slot04_17(Object *obj) {
    data_801ce740_slot04_17[obj->state_3](obj);
}

void start_sequence_1a_set_facing_slot04_17(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 8);
    obj->facing_side = 0;
    if (!(obj->mode_flag_2 != 0 ? (s16)((u16)func_801b5890_slot04_17(obj)->pos_x + 0xc0) < obj->pos_x : (obj->input_bits & 0x8000) != 0)) {
        obj->facing_side++;
    }
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void update_attack_sequence_with_table_hit_slot04_17(Object *obj) {
    int i;
    s16 c;
    s16 d;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[6];

    ref_other.p = obj->other;
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
    } else if (*(u8 *)&obj->step_flags != 0) {
        i = ((*(u8 *)&obj->step_flags + 0xff) << 4) & 0x3f0;
        obj->step_flags = 0;
        c = *(u16 *)(data_801ce74c_slot04_17 + i + 8);
        d = *(u16 *)(data_801ce74c_slot04_17 + i + 12);
        spawn_effect_at_object_offset(obj, obj->direction_index >> 1, *(s16 *)(data_801ce74c_slot04_17 + i), *(s16 *)(data_801ce74c_slot04_17 + i + 4));
        if (apply_damage_to_other_return_defeated(obj, c, d)) {
            play_sound_at(obj, obj->side, 0x336);
            obj->state_3++;
            set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
        } else {
            play_sound_at(obj, obj->side, 0x304);
            ref_other.p = obj->other;
            if (ref_other.p->field_15b == 0) {
                obj->state_3++;
                set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 0, 1);
            }
        }
    }
    update_sequence(obj);
}
