/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern ObjectRef data_80190468;
extern ObjectFn data_801c69ac_slot04_10[];
extern ObjectFn data_801c69c0_slot04_10[];

void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void update_sequence_by_pressed_bytes(Object *object);
int drain_gauge_by_other_input(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void spawn_two_effects_at_other_ground(Object *object);
void load_velocities_from_table_slot04_10(Object *obj);

void start_grab_sequence_1c_set_facing_slot04_10(Object *obj) {
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

void update_sequence_with_hit_on_step_flag_slot04_10(Object *obj) {
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
        spawn_two_effects_at_other_ground(obj);
        update_sequence(obj);
        apply_damage_to_other_return_defeated(obj, 0x12, 0);
        play_sound_at(obj, obj->side, 0x319);
    }
}

void update_sequence_or_finish_sequence_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_5_slot04_10(Object *obj) {
    data_801c69ac_slot04_10[obj->state_3](obj);
}

void start_grab_sequence_18_reset_gauge_slot04_10(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    obj->gauge = 0x64;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->input_bits = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x18);
}

void update_repeated_damage_to_other_with_gauge_slot04_10(Object *obj) {
    u16 a;
    s16 i;

    ref_other.p = obj->other;
    a = obj->step_flags;
    if ((a & 0xff) == 1) {
        obj->step_flags = a & 0xff00;
        if (spawn_random_offset_effect_index_zero(obj) != 0) {
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

void update_sequence_hit_other_reaction_a_slot04_10(Object *obj) {
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

void update_sequence_hit_other_0a_alt_slot04_10(Object *obj) {
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

void update_sequence_or_finish_sequence_2_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_6_slot04_10(Object *obj) {
    data_801c69c0_slot04_10[obj->state_3](obj);
}

void start_grab_sequence_1a_reset_gauge_slot04_10(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    obj->gauge = 0x64;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->input_bits = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void update_hit_sequence_with_damage_step_slot04_10(Object *obj) {
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

void update_sequence_hit_other_reaction_b_slot04_10(Object *obj) {
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
