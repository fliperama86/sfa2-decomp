/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c6560_slot04_14[];
extern ObjectFn data_801c6570_slot04_14[];
extern ObjectFn data_801c6578_slot04_14[];
extern u8 data_801c660c_slot04_14[];
extern ObjectFn data_801c6614_slot04_14[];
extern s32 data_801c6630_slot04_14[];
extern ObjectFn data_801c663c_slot04_14[];

void set_sequence_and_tables_from_direction(Object *obj);
void select_air_action_or_start_air_state_slot04_14(Object *obj);
void start_air_state_pick_sequence_slot04_14(Object *obj);
void move_until_ground_then_reset_state_slot04_14(Object *obj);
void set_object_states_0_to_3_2_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void call_handler_by_state_1_slot04_14(Object *obj);
void move_until_near_ground_then_switch_state_slot04_14(Object *o);
void set_sequence_entry_by_sound_side_table_slot04_14(Object *obj, u8 a);

void update_sequence_or_reset_by_input_dir_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        if (obj->input_dir_0 == 2) {
            reset_object_to_state_1_0_0_2_sequence_1(obj);
        } else {
            reset_object_to_state_1_0_sequence_0(obj);
        }
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_sequence_with_fixed_x_step_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    u8 a;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
        return;
    }
    a = t;
    if (a != 0) {
        if (a == 2) {
            obj->step_flags = (t & -0x100) | 1;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac += 0x20000;
        } else {
            *(s32 *)&obj->pos_x_frac -= 0x20000;
        }
    }
    update_sequence(obj);
}

void call_table_function_by_state_3_23_slot04_14(Object *obj) {
    data_801c6560_slot04_14[obj->state_3](obj);
}

void start_state_hit_or_diagonal_jump_slot04_14(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if (try_force_other_state_if_box_in_range(obj, -0x14, 0x14) & 0xff) {
            set_object_states_0_to_3_2_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            set_sequence_by_side_and_mode(obj, 0x40);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void update_jump_arc_land_slot04_14(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_flag_3 = 0;
            obj->active_state = 0;
            obj->pos_y = ((Slot04bObj *)obj)->ground_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, 0x41);
            return;
        }
    }
    update_sequence(obj);
}

void update_sequence_or_call_finish_on_flag_3_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_24_slot04_14(Object *obj) {
    data_801c6570_slot04_14[obj->state_3](obj);
}

void set_state_3_one_restore_facing_slot04_14(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void select_air_action_or_start_air_state_slot04_14(Object *obj) {
    int d;

    if (obj->active_state != 0) {
        if (obj->hit_flag_a != 0) {
            if ((s16)obj->meter >= 0x30 && obj->linked_object_3 == 0) {
                set_object_states_0_to_3_2_slot04_14(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->active_flag_3 = 1;
                if (obj->side != 0) {
                    scratch_call_right(obj);
                } else {
                    scratch_call_left(obj);
                }
                return;
            }
        } else if (obj->input_dir_second != 0) {
            set_object_states_0_to_3_2_slot04_14(obj, 1, 0, 7, 0);
            obj->field_15a = 3;
            obj->active_flag_3 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        } else if (obj->linked_object_3 == 0 && (d = ((Slot04bObj *)obj)->ground_y - 0x30, obj->pos_y < (s16)d)) {
            set_object_states_0_to_3_2_slot04_14(obj, 1, 0, 7, 0);
            obj->field_15a = 4;
            obj->active_flag_3 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        }
    }
    obj->limit_lo = (obj->limit_lo & 0xfe) | 0x40;
    start_air_state_pick_sequence_slot04_14(obj);
}

void call_handler_by_limit_lo_bit_slot04_14(Object *obj) {
    if (obj->limit_lo & 1) {
        select_air_action_or_start_air_state_slot04_14(obj);
    } else {
        start_air_state_pick_sequence_slot04_14(obj);
    }
}

void start_air_state_pick_sequence_slot04_14(Object *obj) {
    int t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    play_sound_from_table_by_direction_index(obj);
    t = obj->direction_index >> 1;
    add_to_meter_clamped(obj, t);
    if (obj->variant_index != 0) {
        t += 0x12;
    } else {
        t += 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)t);
    play_sound_from_table_by_direction_index(obj);
    if (obj->pos_y < (u16)(((Slot04bObj *)obj)->ground_y - 0x50) && obj->variant_index != 0 && (obj->variant_index & 0x80) == 0 && obj->input_dir_second != 0 && obj->direction_index == 2 && obj->attack_param_d != 0) {
        set_object_states_0_to_3_2_slot04_14(obj, 1, 0, 5, 0);
        set_sequence_by_side_and_mode(obj, 0x29);
    }
}

void call_table_function_by_state_3_25_slot04_14(Object *obj) {
    data_801c6578_slot04_14[obj->state_3](obj);
}

void set_downward_dash_velocity_and_move_to_ground_slot04_14(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    move_until_ground_then_reset_state_slot04_14(obj);
}

void move_until_ground_then_reset_state_slot04_14(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    t = obj->ground_y;
    if (t <= obj->pos_y) {
        obj->pos_y = t;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void set_object_states_0_to_3_2_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void call_handler_by_state_0_slot04_14(Object *obj) {
    data_801c6614_slot04_14[obj->state_0](obj);
}

void init_child_from_parent_with_table_velocity_slot04_14(Object *obj) {
    Object *p;

    ((Slot04bObj *)obj)->data_table_ptr = data_801c660c_slot04_14;
    obj->state_0 = obj->state_0 + 1;
    p = obj->parent;
    obj->field_49 = p->field_49;
    obj->field_1c = p->field_1c;
    obj->pos_y = obj->pos_y - 0x31;
    obj->vel_y = data_801c6630_slot04_14[obj->vel_table_index];
    if (obj->facing_side != 0) {
        obj->pos_x = obj->pos_x + 0x3e;
        obj->vel_x = data_801c6630_slot04_14[obj->vel_table_index - 3];
    } else {
        obj->pos_x = obj->pos_x - 0x3e;
        obj->vel_x = -data_801c6630_slot04_14[obj->vel_table_index - 3];
    }
    if (p->side == 0) {
        ((Slot04bObj *)obj)->data_pointer = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->data_pointer = *(s32 *)0x1f800158;
    }
    set_sequence_entry_by_sound_side_table_slot04_14(obj, obj->vel_table_index);
    call_handler_by_state_1_slot04_14(obj);
}

void call_handler_by_state_1_slot04_14(Object *obj) {
    data_801c663c_slot04_14[obj->state_1](obj);
}

void move_and_advance_sequence_unless_paused_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        move_until_near_ground_then_switch_state_slot04_14(obj);
        advance_sequence_checked(obj);
        set_state_0_if_x_outside_margin_narrow(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}
