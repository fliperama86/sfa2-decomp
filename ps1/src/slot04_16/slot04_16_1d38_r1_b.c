/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void spawn_offset_effect_and_play_sounds(Object *object);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void push_other_by_vel_x(Object *object);
void func_80142c70(Object *object);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void set_sequence_by_side(Object *object, int index);
void load_velocities_from_table_slot04_16(Object *obj);
void clear_seven_eight_byte_blocks_slot04_16(Object *obj);

extern ObjectFn data_801ca168_slot04_16[];
extern ObjectFn data_801ca198_slot04_16[];
extern ObjectFn data_801ca1a8_slot04_16[];
extern ObjectFn data_801ca1b0_slot04_16[];
extern ObjectFn data_801ca1c0_slot04_16[];
extern ObjectFn data_801ca1c8_slot04_16[];

int func_801b235c_slot04_16(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
            return 0;
        }
    }
    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

void func_801b23e8_slot04_16(Object *obj) {
    data_801ca168_slot04_16[obj->field_15a](obj);
}

void func_801b2428_slot04_16(Object *obj) {
    data_801ca198_slot04_16[obj->state_3](obj);
}

void func_801b2468_slot04_16(Object *obj) {
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

void func_801b24d4_slot04_16(Object *obj) {
    s16 a;
    int b;
    if ((s16)obj->step_flags & 0xff00) {
        a = 0;
        b = 0x55;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -0x24;
            b = 0x53;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_seven_eight_byte_blocks_slot04_16(obj);
    }
    update_sequence(obj);
}

void update_countdown_high_byte_advance_state_slot04_16(Object *obj) {
    u8 a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            obj->countdown = (obj->countdown >> 8 << 8) + 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_countdown_then_call_finish_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;
    int t;

    obj->counter++;
    update_sequence(o);
    a = o->countdown;
    t = a - 1;
    o->countdown = t;
    if ((t & 0xff) == 0) {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    } else {
        if (o->pressed_bits != 0) {
            if (obj->saved_count == 0) {
                obj->saved_count = obj->counter;
            }
        } else if ((s16)(t & -0x100) != 0) {
            o->countdown = a - 0x101;
            if ((o->countdown & 0x100) == 0) {
                return;
            }
        } 
        push_other_by_vel_x(o);
    }
}

void func_801b26e8_slot04_16(Object *obj) {
    data_801ca1a8_slot04_16[obj->state_3](obj);
}

void func_801b2728_slot04_16(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x45);
}

void update_sequence_or_finish_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        load_velocities_from_table_slot04_16(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b27bc_slot04_16(Object *obj) {
    data_801ca1b0_slot04_16[obj->state_3](obj);
}

void advance_state_3_on_step_flag_then_update_slot04_16(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void update_check_range_start_grab_slot04_16(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        if ((u8)func_8013f8c4(obj, -0x26, 0x3a) != 0) {
            obj->state_3++;
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            obj->state_3 = 3;
            set_sequence_by_side_and_mode(obj, 0x21);
            return;
        }
    }
    update_sequence(obj);
}

void update_spawn_and_hit_other_by_step_flag_slot04_16(Object *obj) {
    u16 t = obj->step_flags;
    u8 u;
    if ((t & 0xff) == 2) {
        obj->step_flags = t & 0xff00;
        spawn_effect_at_object_offset(obj, 1, -0x46, 0x45);
        play_sound_at(obj, obj->side ^ 1, 0x304);
    }
    u = obj->step_flags;
    if (u == 1) {
        obj->state_3 = obj->state_3 + 1;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0, 0x11, 0x11, 4, 0, u);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = 5;
        }
    }
    update_sequence(obj);
}

void update_shift_x_or_finish_to_other_state_slot04_16(Object *obj) {
    s16 t;
    int a;

    update_sequence(obj);
    t = obj->step_flags;
    if (t >= 0) {
        if ((u8)t == 3) {
            a = 0x30;
            obj->step_flags = t & 0xff00;
            if (obj->facing_side == 0) {
                a = -0x30;
            }
            obj->pos_x = a + obj->pos_x;
        }
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_16(obj);
    }
}

void func_801b2a78_slot04_16(Object *obj) {
    data_801ca1c0_slot04_16[obj->state_3](obj);
}

void advance_state_3_on_step_flag_then_update_2_slot04_16(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void update_sequence_or_set_other_code_5_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_16(obj);
    }
}

void func_801b2b6c_slot04_16(Object *obj) {
    data_801ca1c8_slot04_16[obj->state_3](obj);
}
