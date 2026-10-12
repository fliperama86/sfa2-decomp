/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4c78_slot04_12[];
extern ObjectFn data_801c4c80_slot04_12[];
extern ObjectFn data_801c4c90_slot04_12[];
extern ObjectFn data_801c4c9c_slot04_12[];
extern u16 box_margin[];

void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void set_sequence_by_side(Object *object, int index);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void push_other_by_vel_x(Object *object);
void func_80142c70(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80131638(Object *object);
void save_counter_on_first_press_slot04_12(Object *obj);
void clear_bytes_at_offset_2b0_block_slot04_12(Object *obj);
void func_801b3748_slot04_12(Object *obj);
void func_801b35a0_slot04_12(Object *obj);
void update_sequence_or_call_finish_on_flag_3_slot04_12(Object *obj);
int func_801b3a4c_slot04_12(Object *obj);

void func_801b30d0_slot04_12(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b3130_slot04_12(Object *obj) {
    data_801c4c78_slot04_12[obj->state_3](obj);
}

void start_state_decrement_remaining_count_slot04_12(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    set_sequence_by_side_and_mode(obj, 0x20);
}

void update_sequence_or_call_finish_on_flag_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3200_slot04_12(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b3260_slot04_12(Object *obj) {
    data_801c4c80_slot04_12[obj->state_3](obj);
}

void begin_state_3_sequence_by_active_state_slot04_12(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        play_side_sound_at(obj, obj->side, 8);
        set_sequence_by_side(obj, 0x2b);
    }
}

void update_sequence_trigger_effects_on_flag_slot04_12(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -3;
            b = 0x50;
        } else {
            a = -7;
            b = 0x47;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_bytes_at_offset_2b0_block_slot04_12(obj);
    }
    update_sequence(obj);
}

void update_countdown_set_cursor_timer_slot04_12(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown + 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void count_down_then_reset_states_slot04_12(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = o->countdown - 1;
    if ((t & 0xff) != 0) {
        save_counter_on_first_press_slot04_12(o);
    } else {
        if (o->active_state != 0) {
            o->state_0 = 1;
        }
        o->state_2 = 3;
        o->state_1 = 0;
        o->state_3 = 1;
        func_80142c70(o);
    }
}

void save_counter_on_first_press_slot04_12(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->pressed_bits != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    }
    push_other_by_vel_x(o);
}

void clear_bytes_at_offset_2b0_block_slot04_12(Object *object) {
    u8 *p = (u8 *)object + 0x2b0;
    int i;
    u8 z = 0;

    for (i = 0x50; i >= 0; i--) {
        *p++ = z;
    }
}

void call_state_3_dispatch_by_input_dir_0_slot04_12(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b3748_slot04_12(obj);
    } else {
        func_801b35a0_slot04_12(obj);
    }
}

void func_801b35a0_slot04_12(Object *obj) {
    data_801c4c90_slot04_12[obj->state_3](obj);
}

void func_801b35e0_slot04_12(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 3);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}

void func_801b3690_slot04_12(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 0xc);
        set_other_hit_reaction_state_0(obj, 4, 0xa, 0xf, 0, 0, 1);
    }
    update_sequence(obj);
}

void update_sequence_or_call_finish_on_flag_2_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3748_slot04_12(Object *obj) {
    data_801c4c9c_slot04_12[obj->state_3](obj);
}

void func_801b3788_slot04_12(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 4);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    obj->countdown = (u8)obj->countdown + 0xc00;
    set_sequence_by_side_and_mode(obj, 0x19);
}

void count_down_high_byte_advance_state_3_slot04_12(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
    }
}

void func_801b3878_slot04_12(Object *obj) {
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 2;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        set_sequence_by_side_and_mode(obj, 0x1a);
    } else {
        if (((Slot04bObj *)obj)->mode != 0) {
            obj->state_3 = obj->state_3 + 1;
            play_side_sound_at(obj, obj->side, 0xc);
            set_other_hit_reaction_state_0(obj, 4, 0xa, 0x12, 0, 0, 1);
        }
        update_sequence(obj);
    }
}

void update_air_move_land_or_sequence_slot04_12(Object *obj) {
    if (func_801b3a4c_slot04_12(obj) < 0 && obj->pos_y > obj->ground_y) {
        func_80131638(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b39a0_slot04_12(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    play_side_sound_at(obj, obj->side, 0xc);
    set_other_hit_reaction_state_0(obj, 4, 0xa, 0xf, 0, 0, 1);
    update_sequence_or_call_finish_on_flag_3_slot04_12(obj);
}

void update_sequence_or_call_finish_on_flag_3_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

int func_801b3a4c_slot04_12(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    return obj->vel_y = obj->vel_y + obj->accel_y;
}
