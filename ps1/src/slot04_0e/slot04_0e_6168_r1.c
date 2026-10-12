/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c61ac_slot04_0e[];
extern ObjectFn data_801c61b8_slot04_0e[];
extern u16 data_801c5d84_slot04_0e[];
extern u16 data_801a2804[];
extern u16 box_margin[];

Block172 *pop_block_from_pool(void);
void set_position_from_other_box(Object *object);
void start_action_if_buttons_pressed(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj);
BytePair get_input_byte_pair_or_stored_pair_slot04_0e(Slot04bObj *obj);
void func_801b6374_slot04_0e(Object *obj);
void func_801b650c_slot04_0e(Object *obj);
void spawn_object_with_variant_and_velocity_slot04_0e(Object *obj, u8 a, int b, int c);
void copy_sixteen_table_entries_by_image_row_slot04_0e(Object *obj);
void load_velocities_from_table_slot04_0e(Object *obj);
u8 is_attack_param_d_nonzero_slot04_0e(Object *obj);

void func_801b6168_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x2a);
    }
}

void move_until_ground_then_land_state_seven_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if (obj->ground_y >= obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = 7;
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x2b);
    }
}

void update_sequence_or_end_move_a_3_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void set_variant_index_from_input_pair_slot04_0e(Object *obj) {
    BytePair buf;
    u16 t;

    if (obj->mode_flag_2 != 0) {
        if (call_handler_by_limit_hi_returning_slot04_0e(obj) != 0) {
            buf = get_input_byte_pair_or_stored_pair_slot04_0e((Slot04bObj *)obj);
            obj->variant_index = buf.first;
            ((Slot04bObj *)obj)->state_flag = 1;
        }
    } else {
        t = obj->pressed_bits;
        if (t & 0x95) {
            buf = get_input_byte_pair_or_stored_pair_slot04_0e((Slot04bObj *)obj);
            t = obj->pressed_bits;
            obj->variant_index = buf.first;
            if ((t & 0x94) != 0x94) {
                if ((t & 1) == 0) {
                    ((Slot04bObj *)obj)->state_flag = 1;
                }
            }
        }
    }
}

void call_handler_by_input_dir_second_2_slot04_0e(Object *obj) {
    if (obj->input_dir_second != 0) {
        func_801b650c_slot04_0e(obj);
    } else {
        func_801b6374_slot04_0e(obj);
    }
}

void func_801b6374_slot04_0e(Object *obj) {
    data_801c61ac_slot04_0e[obj->state_3](obj);
}

void start_sequence_18_with_facing_and_sound_slot04_0e(Object *obj) {
    Object *o;

    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    o = obj->other;
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((u16)(box_margin[0] + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->input_bits & 0x8000) {
set:
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}

void func_801b645c_slot04_0e(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0x10, 0, 1, 0);
    }
    update_sequence(obj);
}

void update_sequence_or_flip_facing_finish_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b650c_slot04_0e(Object *obj) {
    data_801c61b8_slot04_0e[obj->state_3](obj);
}

void start_sequence_19_at_other_box_slot04_0e(Object *obj) {
    Object *o;

    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    o = obj->other;
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((u16)(box_margin[0] + 0xc0) < obj->pos_x) {
            goto set;
        }
    } else if (obj->input_bits & 0x8000) {
set:
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x19);
    set_position_from_other_box(obj);
}

void update_sequence_then_spawn_three_effects_slot04_0e(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        set_position_from_other_box(obj);
    } else {
        *(u8 *)&obj->step_flags = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->other->field_261 = 1;
        spawn_object_with_variant_and_velocity_slot04_0e(obj, 0x53, 0, 0);
        spawn_object_with_variant_and_velocity_slot04_0e(obj, 0x54, -0x70000, -0x20000);
        spawn_object_with_variant_and_velocity_slot04_0e(obj, 0x55, -0x110000, -0x30000);
    }
}

void spawn_object_with_variant_and_velocity_slot04_0e(Object *obj, u8 a, int b, int c) {
    Object *p = (Object *)pop_block_from_pool();

    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x3b;
        p->table_index = 5;
        p->ot_depth = 0;
        p->field_02 = 0x14;
        p->team_id = obj->team_id;
        p->spawn_x = 0x60;
        p->parent = obj;
        p->variant_index = a;
        p->vel_x = b;
        p->vel_y = c;
        p->spawn_y = 0x1e0;
        p->image_row = obj->image_row;
        p->mode_id = 0x20;
        p->field_90 = obj->field_90;
        p->sound_side = obj->sound_side;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
    }
}

void launch_on_step_flag_byte_slot04_0e(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->vel_x = 0xa0000;
        obj->accel_x = -0x2000;
        obj->accel_y = -0x4000;
        obj->vel_y = 0x28000;
        *(u8 *)&obj->step_flags = 0;
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
    }
}

void copy_sixteen_table_entries_by_image_row_slot04_0e(Object *obj) {
    u16 i = 0;

    do {
        data_801a2804[obj->image_row * 16 + i] = data_801c5d84_slot04_0e[i];
        i++;
    } while (i < 0x10);
    queue_image_upload_job(0, 6);
}

void load_velocities_from_table_slot04_0e(Object *obj) {
    copy_image_rows_and_load_image(obj);
}

void move_update_sequence_advance_on_flag_slot04_0e(Object *obj) {
    Object *o;

    apply_velocity_and_accel_to_position_slot04_0e(obj);
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        *(u8 *)&obj->step_flags = 0;
        o = obj->other;
        if (o->field_15b != 0) {
            copy_sixteen_table_entries_by_image_row_slot04_0e(obj);
            play_side_sound_at(obj, obj->side, 7);
        }
    }
}

void func_801b6918_slot04_0e(Object *obj) {
    Object *o;

    apply_velocity_and_accel_to_position_slot04_0e(obj);
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        *(u8 *)&obj->step_flags = 0;
        o = obj->other;
        o->field_261 = 0;
        o->pos_x_frac = 0;
        if (o->field_15b != 0) {
            set_other_hit_reaction_state_0(obj, 0, 0xc, 0x10, 0, 1, 0);
        } else {
            set_other_hit_reaction_state_0(obj, 0, 5, -1, 0, 1, 0);
        }
    }
}

void move_until_ground_then_advance_state_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if (obj->ground_y < obj->pos_y) {
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        load_velocities_from_table_slot04_0e(obj);
        obj->step_timer = 1;
        update_sequence(obj);
    }
}

void update_sequence_or_flip_facing_finish_2_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

u8 is_second_input_direction_active_slot04_0e(Object *obj) {
    u8 r;

    if (obj->input_dir_second == 0 || obj->direction_index != 2) {
        r = 0;
    } else if (obj->mode_flag_2 != 0) {
        r = is_attack_param_d_nonzero_slot04_0e(obj);
    } else {
        r = (obj->input_bits_2 >> 14) & 1;
    }
    return r;
}

u8 is_attack_param_d_nonzero_slot04_0e(Object *obj) {
    return obj->attack_param_d != 0;
}

void func_801b6afc_slot04_0e(Object *obj) {
    obj->vel_x = obj->vel_x >> 1;
    add_to_meter_clamped(obj, 1);
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void clear_80_bytes_at_tail_slot04_0e(Object *obj) {
    int i;
    u8 z = 0;
    u8 *p = (u8 *)obj + 0x2b0;

    for (i = 0x4f; i >= 0; i--) {
        *p++ = z;
    }
}
