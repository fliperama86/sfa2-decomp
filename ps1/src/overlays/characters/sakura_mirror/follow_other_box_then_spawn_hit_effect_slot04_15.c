/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c2eac_slot04_15[];
extern u8 data_801c2ecc_slot04_15[];
extern ObjectFn data_801c314c_slot04_15[];
extern ObjectFn data_801c3160_slot04_15[];
extern ObjectFn data_801c3170_slot04_15[];

Block172 *pop_block_from_pool(void);
void set_sequence_by_side(Object *object, int arg);
void set_position_from_other_box(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void apply_velocity_with_gravity_slot04_15(Object *obj);
void count_down_then_set_side_bit_slot04_15(Object *object, GameState *g);

void follow_other_box_then_spawn_hit_effect_slot04_15(Object *obj) {
    Object *o;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
        set_position_from_other_box(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        spawn_effect_at_object_offset(obj, 1, 0x18, 0x47);
        o = obj->other;
        ref_other.p = o;
        play_sound_at(o, ref_other.p->side, 0x306);
        set_sequence_by_side_and_mode(obj, 0x1a);
    }
}

void update_sequence_stop_and_hit_other_on_flag_slot04_15(Object *obj) {
    Object *o;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->vel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0x6000;
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0x1e, 0xf, 1, 0, 1, 1);
        o = obj->other;
        if (o->field_15b != 0) {
            o->field_243 = 0;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}

void fall_until_ground_then_land_sequence_a_slot04_15(Object *obj) {
    apply_velocity_with_gravity_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x1b);
    }
}

void update_sequence_or_flip_facing_finish_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void update_airborne_sequence_land_on_ground_slot04_15(Object *obj) {
    s16 y;

    apply_velocity_with_gravity_slot04_15(obj);
    if (((Slot04bObj *)obj)->saved_pos_x <= 0 || obj->pos_y < (y = obj->ground_y)) {
        update_sequence(obj);
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_table_function_by_state_3_16_slot04_15(Object *obj) {
    data_801c314c_slot04_15[obj->state_3](obj);
}

void start_state_sound_input_facing_snap_to_other_box_slot04_15(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x20);
    set_position_from_other_box(obj);
}

void update_sequence_launch_other_on_flag_slot04_15(Object *obj) {
    Object *o;
    int x;

    update_sequence(obj);
    set_position_from_other_box(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_effect_at_object_offset(obj, 2, -0x49, 0x30);
        x = 0x36000;
        if (obj->facing_side != 0) {
            x = -0x36000;
        }
        obj->vel_y = -0x40000;
        obj->vel_x = x;
        obj->accel_y = 0x6000;
        set_other_hit_reaction_state_0(obj, 0x1d, 5, 0xf, 0, 0, 0);
        o = obj->other;
        ref_other.p = o;
        if ((s16)obj->health >= 0) {
            play_sound_at(o, ref_other.p->side, 0x30f);
        } else {
            play_sound_at(o, ref_other.p->side, 0x346);
        }
    }
}

void move_and_switch_sequence_near_ground_slot04_15(Object *obj) {
    apply_velocity_with_gravity_slot04_15(obj);
    if (((Slot04bObj *)obj)->saved_pos_x < 0 || (s16)(obj->ground_y - 0x50) >= obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x21);
    }
}

void fall_until_ground_then_land_sequence_b_slot04_15(Object *obj) {
    apply_velocity_with_gravity_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}

void update_sequence_or_finish_sequence_5_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void apply_velocity_with_gravity_slot04_15(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void load_velocities_from_table_4_slot04_15(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void load_velocities_from_table_5_slot04_15(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_handler_by_state_2_2_slot04_15(Object *obj) {
    data_801c3160_slot04_15[obj->state_2](obj);
}

void advance_state_2_and_restore_facing_slot04_15(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void advance_state_2_when_timer_zero_slot04_15(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void choose_random_substate_and_set_sequence_slot04_15(Object *object) {
    u16 a;
    ((Slot04bObj *)object)->countdown = 0x3c;
    object->state_2 = object->state_2 + 1;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    a = object->input_bits & 0xf000;
    if (((Slot04bObj *)object)->field_5c >= 0x90 && a == 0x1000) {
        a = 1;
    } else {
        a = get_random_byte();
        a &= 0x1f;
        if (((Slot04bObj *)object)->field_5c < 0x80) {
            a = data_801c2eac_slot04_15[a];
        } else {
            a = data_801c2ecc_slot04_15[a];
        }
    }
    a = (s8)get_input_mapped_value_or_default(object, (s8)a); object->substate_index = a; a += 0x23; set_sequence_by_side(object, (s16)a);
}

void call_handler_by_substate_index_slot04_15(Object *object) {
    data_801c3170_slot04_15[object->substate_index](object);
}

void update_sequence_with_random_loop_and_countdown_slot04_15(Object *object) {
    int a;
    s8 r;

    if ((s16)object->step_flags < 0) {
        r = get_random_byte();
        a = r;
        if ((a & 3) != 0) {
            a = 0x32;
        } else {
            a = 0x33;
        }
        set_sequence_by_side(object, (u8)a);
    }
    count_down_then_set_side_bit_slot04_15(object, &game_state);
    update_sequence(object);
}

void tick_countdown_and_update_sequence_slot04_15(Object *object) {
    count_down_then_set_side_bit_slot04_15(object, &game_state);
    update_sequence(object);
}

void count_down_then_set_side_bit_slot04_15(Object *object, GameState *g) {
    if (((Slot04bObj *)object)->countdown != 0) {
        ((Slot04bObj *)object)->countdown--;
        if (((Slot04bObj *)object)->countdown == 0) {
            g->side_bits_2 |= 1 << object->side;
        }
    }
}

void update_sequence_spawn_offset_object_slot04_15(Object *object) {
    Object *p;
    int dx;
    u16 e;
    u8 d;

    update_sequence(object);
    if (object->step_flags & 0xff) {
        object->step_flags = object->step_flags & 0xff00;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x77;
            p->parent = object;
            p->field_0c = object->field_0c;
            p->image_row = object->image_row;
            p->field_0e = object->field_0e;
            p->facing_side = object->facing_side;
            p->field_1c = object->field_1c;
            e = object->cycle_value;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->cycle_value = e;
            d = object->image_row;
            p->field_02 = 0x17;
            p->mode_id = 0x20;
            p->image_row = d;
            p->field_90 = object->field_90;
            p->data_ptr_a = object->data_ptr_a;
            p->data_ptr_b = object->data_ptr_b;
            p->sound_side = object->sound_side;
            *(s32 *)&p->pos_x_frac = *(s32 *)&object->pos_x_frac;
            *(s32 *)&p->pos_y_frac = *(s32 *)&object->pos_y_frac;
            p->pos_y = p->pos_y - 0x63;
            dx = -0x3b;
            if (object->facing_side != 0) {
                dx = 0x3b;
            }
            p->pos_x = dx + p->pos_x;
        }
    }
    count_down_then_set_side_bit_slot04_15(object, &game_state);
}
