/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

extern ObjectFn data_801ca12c_slot04_16[];
extern ObjectFn data_801ca134_slot04_16[];
extern ObjectFn data_801ca140_slot04_16[];
extern ObjectFn data_801ca154_slot04_16[];
void play_side_sound_at(Object *o, int b, int c);
void play_sound_at(Object *o, int b, unsigned c);
void set_sequence_by_side_and_mode(Object *object, int arg);
void update_sequence(Object *object);
void reset_object_to_state_1_0_sequence_0(Object *object);
void add_to_meter_clamped(Object *object, short delta);
u8 func_80146840(Object *object);
u8 get_random_byte(void);
int func_801418bc(Object *object);
extern u16 box_margin[];
extern ObjectRef data_80190468;

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int apply_damage_to_other_return_defeated(Object *object, int a, int b);
void update_sequence_by_pressed_bytes(Object *object);
int drain_gauge_by_other_input(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void func_80146960(Object *object);
void set_sequence_by_side(Object *object, int index);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void func_80131638(Object *object);

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013caf0(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
u8 func_8013cd5c(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
int func_80141788(Object *object);
int is_not_state_2_five(Object *object);

/* functions of other units of this module */
int func_801b235c_slot04_16(Object *obj);
int func_801b2238_slot04_16(Object *obj);
u8 func_801b1d38_slot04_16(Object *obj);
u8 func_801b1dc4_slot04_16(Object *obj);
u8 try_start_move_7_with_dir_slot04_16(Object *obj);
u8 try_start_move_8_with_dir_slot04_16(Object *obj);
u8 try_start_move_6_with_dir_slot04_16(Object *obj);
u8 try_start_move_4_with_dir_slot04_16(Object *obj);
u8 try_start_move_5_slot04_16(Object *obj);
u8 func_801b210c_slot04_16(Object *obj);

void load_velocities_from_table_slot04_16(Object *obj);
void func_801b1340_slot04_16(Object *obj);
void func_801b1700_slot04_16(Object *obj);
u8 try_start_move_0_meter_cost_slot04_16(Object *obj);

void call_handler_by_state_2_2_slot04_16(Object *obj) {
    data_801ca12c_slot04_16[obj->state_2](obj);
}

void pick_random_sequence_advance_state_2_slot04_16(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    if ((get_random_byte() & 1) == 0 && (obj->input_bits & 0x100) == 0) {
        play_side_sound_at(obj, obj->side, 0xb);
        set_sequence_by_side_and_mode(obj, 0x45);
    } else {
        set_sequence_by_side(obj, 0x22);
    }
}

void func_801b1070_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    update_sequence(obj);
}

void load_velocities_from_table_slot04_16(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void load_velocities_from_table_2_slot04_16(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void load_velocities_from_table_3_slot04_16(Object *obj) {
    func_80131638(obj);
}

void call_handler_by_input_dir_or_state_3_slot04_16(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b1700_slot04_16(obj);
    } else if (obj->input_dir_second != 0) {
        func_801b1340_slot04_16(obj);
    } else {
        data_801ca134_slot04_16[obj->state_3](obj);
    }
}

void start_grab_sequence_1c_set_facing_slot04_16(Object *obj) {
    int t;

    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        t = (s16)(box_margin[0] + 0xc0) < obj->pos_x;
    } else {
        t = obj->input_bits & 0x2000;
    }
    if (t) {
        obj->facing_side = 1;
    }
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x1c);
}

void update_sequence_with_hit_on_step_flag_slot04_16(Object *obj) {
    u16 a = obj->step_flags;

    if ((a & 0xff) != 2) {
        if ((a & 0xff) != 1) {
            update_sequence(obj);
        } else {
            obj->state_3 = obj->state_3 + 1;
            set_other_hit_reaction_state_0(obj, 0, 0xf, -0x200, 0, 0, 0);
            update_sequence(obj);
        }
    } else {
        obj->step_flags = a & 0xff00;
        data_80190468.p->vec_table_index = 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        apply_damage_to_other_return_defeated(obj, 0x12, 0);
        play_sound_at(obj, obj->side, 0x319);
    }
}

void update_sequence_or_finish_sequence_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b1340_slot04_16(Object *obj) {
    data_801ca140_slot04_16[obj->state_3](obj);
}

void start_grab_sequence_18_reset_gauge_slot04_16(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    obj->gauge = 0x64;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->input_bits = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x18);
}

void func_801b13e8_slot04_16(Object *obj) {
    u16 a;
    s16 i;

    ref_other.p = obj->other;
    a = obj->step_flags;
    if ((a & 0xff) == 1) {
        obj->step_flags = a & 0xff00;
        if (func_80146840(obj) != 0) {
            i = -0x4f;
            if (obj->facing_side != 0) {
                i = 0x4f;
            }
            *(u16 *)&ref_other.p->pos_x += i;
            *(u16 *)&ref_other.p->pos_y -= 0x59;
        }
        i = 0;
        if (((Slot04bObj *)obj)->input_bits == 0) {
            i = 0xa;
            ((Slot04bObj *)obj)->input_bits = ((Slot04bObj *)obj)->input_bits + 1;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, (s16)i, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x15);
            obj->state_3 = 3;
            set_sequence_by_side_and_mode(obj, 0x19);
        } else {
            play_side_sound_at(obj, obj->side, 0x13);
            obj->countdown = obj->countdown + 1;
            update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0 || (ref_other.p->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            set_sequence_by_side_and_mode(obj, 0x19);
        }
        }
    } else {
        update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0 || (ref_other.p->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            set_sequence_by_side_and_mode(obj, 0x19);
        }
    }
}

void func_801b15c8_slot04_16(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        if (((Slot04bObj *)obj)->input_bits != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, t);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, 0xa, 0, 0, t);
        }
        obj->state_3 = 4;
    }
}

void update_sequence_hit_other_0a_alt_slot04_16(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 1) {
        if (((Slot04bObj *)obj)->input_bits != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 0);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, 0xa, 0, 0, 0);
        }
        obj->state_3 = 4;
    }
}

void update_sequence_or_finish_sequence_2_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_16(obj);
    }
}

void func_801b1700_slot04_16(Object *obj) {
    data_801ca154_slot04_16[obj->state_3](obj);
}

void start_grab_sequence_1a_reset_gauge_slot04_16(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    obj->gauge = 0x64;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->input_bits = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void update_hit_sequence_with_damage_step_slot04_16(Object *obj) {
    u16 a;
    int i;

    ref_other.p = obj->other;
    a = obj->step_flags;
    if ((a & 0xff) == 1) {
        obj->step_flags = a & 0xff00;
        spawn_effect_at_object_offset(obj, 1, -0x69, 0x49);
        i = 0;
        if (((Slot04bObj *)obj)->input_bits == 0) {
            i = 0xb;
            ((Slot04bObj *)obj)->input_bits = ((Slot04bObj *)obj)->input_bits + 1;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, (s16)i, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x16);
            obj->state_3 = 3;
            set_sequence_by_side_and_mode(obj, 0x1b);
        } else {
            play_side_sound_at(obj, obj->side, 0x14);
            obj->countdown = obj->countdown + 1;
            update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0 || (ref_other.p->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            set_sequence_by_side_and_mode(obj, 0x1b);
        }
        }
    } else {
        update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0 || (ref_other.p->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            set_sequence_by_side_and_mode(obj, 0x1b);
        }
    }
}

void func_801b1930_slot04_16(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        if (((Slot04bObj *)obj)->input_bits != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, t);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, 0xb, 0, 0, t);
        }
        obj->state_3 = 4;
    }
}

void update_sequence_hit_other_0b_by_mode_slot04_16(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    update_sequence(obj);
    if (o->mode == 1) {
        if (o->input_bits != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 0);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, 0xb, 0, 0, 0);
        }
        obj->state_3 = 4;
    }
}

void update_sequence_or_finish_sequence_3_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_2_slot04_16(obj);
    }
}

void select_slot04_10_action_from_inputs_slot04_16(Object *obj) {
    if (get_pressed_bit_8(obj) && (u8)func_801b235c_slot04_16(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_move_0_meter_cost_slot04_16(obj)) return;
    if (func_8013cd5c(obj, 0, 0x22) && func_801b1d38_slot04_16(obj)) return;
    if ((u8)func_8013caf0(obj, 1, 0x15) && func_801b1dc4_slot04_16(obj)) return;
    if (func_8013ccb4(obj, 2, 0x21) && try_start_move_7_with_dir_slot04_16(obj)) return;
    if (func_8013ccb4(obj, 3, 0x23) && try_start_move_8_with_dir_slot04_16(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 4) && try_start_move_6_with_dir_slot04_16(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0xd) && func_801b210c_slot04_16(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 0xe) && (u8)func_801b2238_slot04_16(obj)) return;
    if ((u8)func_8013d0c8(obj) && try_start_move_4_with_dir_slot04_16(obj)) return;
    if ((u8)func_8013d0fc(obj)) try_start_move_5_slot04_16(obj);
}

u8 try_start_move_0_meter_cost_slot04_16(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if (is_not_state_2_five(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 0;
                }
            }
        }
        return r;
    }
    return 0;
}
