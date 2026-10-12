/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca400_slot04_16[];
extern u8 data_801ca430_slot04_16[];
extern u16 data_801ca438_slot04_16[];
extern Slot04_16Reca444 data_801ca444_slot04_16[];
extern s16 data_801ca474_slot04_16[];
extern ObjectFnInt data_801ca47c_slot04_16[];
extern u8 data_801ca4ac_slot04_16[];
extern u8 data_801ca514_slot04_16[];
extern SequenceStep *data_801ca550_slot04_16[];
extern ObjectFn data_801ca55c_slot04_16[];
extern ObjectFn data_801ca56c_slot04_16[];
extern ObjectRef data_80190468;

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_three_child_records_at_other(Object *object);
void spawn_two_effects_at_other_ground(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void clear_or_set_flag_by_pos_x_limit(Object *o);
void try_force_other_then_spawn_children_or_end_slot04_16(Object *obj);
void set_vertical_motion_from_table_slot04_16(Object *obj);
void load_velocities_from_table_slot04_16(Object *obj);
void load_velocities_from_table_2_slot04_16(Object *obj);

void update_launch_other_hit_with_velocity_slot04_16(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->state_3++;
        o->step_flags = o->step_flags & 0xff00;
        ref_other.p = o->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(o, 0x22, 0xf, -0x200, 0, 0, 0);
        ref_other.p = o->other;
        if ((s16)ref_other.p->health < 0) {
            o->field_167 = (o->direction_index >> 1) + 0xc;
        }
        o->vel_x = 0x48000;
        o->vel_y = 0x80000;
        o->accel_y = 0xffff7800;
        o->accel_x = 0;
        if (o->facing_side != 0) {
            o->vel_x = -o->vel_x;
        }
    }
}

void update_airborne_or_land_set_state_3_to_8_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = 8;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x33);
    }
}

void call_table_function_by_state_3_18_slot04_16(Object *obj) {
    data_801ca400_slot04_16[obj->state_3](obj);
}

void start_sequence_29_with_effects_slot04_16(Object *obj) {
    ((Slot04bObj *)obj)->table_index_2 = 0;
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter((GameState *)game_state.config, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, 0x29);
}

void update_spawn_children_on_step_flag_high_byte_slot04_16(Object *obj) {
    int t = -1;

    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            t = 1;
        }
        obj->field_165 = t;
        spawn_children_per_direction_index(obj, -0x13, 0x53);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void release_other_when_step_flag_high_byte_clear_slot04_16(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        obj->other_lock = 0;
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 0;
        set_sequence_by_side_and_mode(obj, 0x3f);
        try_force_other_then_spawn_children_or_end_slot04_16(obj);
    }
}

void try_force_other_then_spawn_children_or_end_slot04_16(Object *obj) {
    s16 t = *(u16 *)(data_801ca430_slot04_16 + (obj->direction_index & 0xfe));

    if ((u8)try_force_other_state_if_box_in_range(obj, -0x26, (s16)(t - 0x26)) == 0) {
        obj->state_3 = 0xa;
        update_sequence(obj);
    } else {
        obj->state_3++;
        if (obj->direction_index != 0) {
            play_side_sound_at(obj, obj->side, 8);
        } else {
            play_side_sound_at(obj, obj->side, 4);
        }
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        spawn_three_child_records_at_other(obj);
        set_sequence_by_side_and_mode(obj, 0x2a);
    }
}

void update_multi_step_attack_with_flag_codes_slot04_16(Object *o) {
    s16 t;
    u16 x;
    int one;
    int m;
    int d;

    update_sequence(o);
    t = o->step_flags;
    if (t < 0) {
        if (o->direction_index != 0 && ((Slot04bObj *)o)->table_index_2 == 0) {
            ((Slot04bObj *)o)->table_index_2 = 1;
            set_sequence_by_side_and_mode(o, 0x2b);
        } else {
            one = 1;
            o->active_state = one;
            ((Slot04bObj *)o)->field_298 = 0;
            o->state_3++;
            set_sequence_by_side_and_mode(o, 0x2c);
            ((Slot04bObj *)o)->table_index_2 = 0;
            if (o->direction_index == 4) {
                ((Slot04bObj *)o)->table_index_2 = one;
            }
            set_vertical_motion_from_table_slot04_16(o);
        }
        return;
    }
    if ((t & 0xff00) != 0) {
        if ((t & 0xff00) != 0x100) {
            o->step_flags = t & 0xff;
            if (o->direction_index == 2) {
                play_side_sound_at(o, o->side, 4);
            }
        } else {
            o->step_flags = t & 0xff;
            if (o->direction_index == 0) {
                play_side_sound_at(o, o->side, 5);
            }
        }
    }
    x = o->step_flags;
    if ((x & 0xff) == 0) {
        return;
    }
    if ((x & 0xff) == 1) {
        m = -0x100;
        apply_damage_to_other_return_defeated(o, (s16)(*(u16 *)((u8 *)data_801ca438_slot04_16 + ((((Slot04bObj *)o)->table_index_2 + o->direction_index) << 1)) | m), 0);
        o->step_flags = o->step_flags & m;
        game_state.config->setup_value = 0x3c;
        play_sound_at(o, o->side, 0x319);
        if (o->direction_index != 0) {
            if (((Slot04bObj *)o)->table_index_2 == 0) {
                if (o->direction_index != 4) {
                    return;
                }
                play_side_sound_at(o, o->side, 4);
            } else {
                play_side_sound_at(o, o->side, 5);
            }
        }
        spawn_two_effects_at_other_ground(o);
    } else if ((x & 0xff) == 2) {
        d = 0x58;
        o->step_flags = x & 0xff00;
        if (o->facing_side != 0) {
            d = -0x58;
        }
        o->pos_x = o->pos_x - d;
    } else if ((x & 0xff) == 3) {
        o->step_flags = x & 0xff00;
        if (o->direction_index == 0) {
            set_sequence_by_side_and_mode(o, 0x2b);
        }
    }
}

void set_vertical_motion_from_table_slot04_16(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[4];

    obj->vel_y = data_801ca444_slot04_16[((Slot04bObj *)obj)->table_index_2].a;
    obj->accel_y = data_801ca444_slot04_16[((Slot04bObj *)obj)->table_index_2].b;
    obj->field_f4 = data_801ca444_slot04_16[((Slot04bObj *)obj)->table_index_2].c;
    obj->vel_x = 0;
    obj->accel_x = 0;
}

void update_rise_then_set_accel_y_from_field_slot04_16(Object *obj) {
    apply_velocity_and_check_ground(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->accel_y = obj->field_f4;
        update_sequence(obj);
    }
}

void update_fall_land_choose_sequence_by_direction_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else if (obj->direction_index == 4 && ((Slot04bObj *)obj)->table_index_2 != 2) {
        obj->state_3 = 0xb;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff00;
        set_sequence_by_side_and_mode(obj, 0x2d);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        set_sequence_by_side_and_mode(obj, 0x2e);
    }
}

void update_throw_by_step_flag_byte_with_damage_slot04_16(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 0) {
        update_sequence(obj);
    } else if ((t & 0xff) == 0xf) {
        obj->step_flags = t & 0xff00;
        data_80190468.p->vec_table_index = obj->direction_index * 4 + 0x3c;
        spawn_two_effects_at_other_ground(obj);
        update_sequence(obj);
        play_side_sound_at(obj, obj->side, 6);
        if ((u8)apply_damage_to_other_return_defeated(obj, *(s16 *)((u8 *)data_801ca474_slot04_16 + (obj->direction_index & 0xfe)), 0) != 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            spawn_game_state_kind_object(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    } else {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0x23, 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        obj->vel_x = 0x38000;
        obj->vel_y = 0x60000;
        obj->accel_y = -0x8000;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void update_fall_land_with_sound_variant_e_slot04_16(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x2f);
    }
}

void update_sequence_or_toggle_facing_set_other_code_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side = obj->facing_side ^ 1;
        load_velocities_from_table_2_slot04_16(obj);
    }
}

void update_sequence_or_finish_sequence_4_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_16(obj);
    }
}

void update_damage_on_flag_or_start_next_sequence_slot04_16(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if ((t & 0xff) == 0xf) {
            apply_damage_to_other_return_defeated(obj, -0xed, 0);
            obj->step_flags = obj->step_flags & 0xff00;
            data_80190468.p->vec_table_index = obj->direction_index * 4 + 0x3c;
            play_sound_at(obj, obj->side, 0x319);
            spawn_two_effects_at_other_ground(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3 = 5;
        obj->active_state = 1;
        ((Slot04bObj *)obj)->field_298 = 0;
        ((Slot04bObj *)obj)->table_index_2 = 2;
        set_vertical_motion_from_table_slot04_16(obj);
        set_sequence_by_side_and_mode(obj, 0x2c);
    }
}

void func_801b6370_slot04_16(Object *obj) {
    data_801ad398 = data_801ca47c_slot04_16[obj->field_15a](obj);
}

int has_remaining_count_slot04_16(Object *obj) {
    return obj->remaining_count != 0;
}

int is_meter_at_least_threshold_slot04_16(Object *obj) {
    return *(s16 *)&obj->meter >= 0x30;
}

int is_meter_at_least_threshold_2_slot04_16(Object *obj) {
    return *(s16 *)&obj->meter >= 0x30;
}

int return_true_always_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_2_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_3_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_4_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_5_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_6_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_7_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_8_slot04_16(Object *obj) {
    return 1;
}

int return_true_always_9_slot04_16(Object *obj) {
    return 1;
}

void clear_seven_eight_byte_blocks_slot04_16(Object *obj) {
    s16 i;
    u16 j;

    for (i = 0; i < 7; i++) {
        ref_first.p = (Object *)((u8 *)obj + 0x2b0 + (i << 3));
        for (j = 0; j < 8; j++) {
            *(*(u8 **)&ref_first.p)++ = 0;
        }
    }
}

void call_handler_by_state_0_slot04_16(Object *obj) {
    data_801ca55c_slot04_16[obj->state_0](obj);
}

void init_child_object_from_parent_tables_slot04_16(Object *obj) {
    u8 t;

    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    obj->ot_depth = 0;
    ref_other.p = obj->parent;
    t = ref_other.p->field_49;
    ((Slot04bObj *)obj)->data_table_ptr = data_801ca514_slot04_16;
    ((Slot04bObj *)obj)->data_pointer = (s32)data_801ca4ac_slot04_16;
    obj->active_state = 0;
    ((Slot04bObj *)obj)->field_5c = 0;
    obj->field_49 = t;
    set_sequence_entry(obj, data_801ca550_slot04_16[obj->vel_table_index >> 1]);
}

void call_handler_by_state_1_then_offset_slot04_16(Object *obj) {
    data_801ca56c_slot04_16[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void advance_state_0_when_parent_frame_param_zero_slot04_16(Object *obj) {
    ref_other.p = obj->parent;
    if (ref_other.p->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}
