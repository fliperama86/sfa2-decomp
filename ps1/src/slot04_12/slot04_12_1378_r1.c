/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c4a20_slot04_12[];
extern ObjectFn data_801c4a40_slot04_12[];
extern ObjectFn data_801c4a6c_slot04_12[];
extern ObjectFn data_801c4a98_slot04_12[];
extern u8 data_801c4aa8_slot04_12[];
extern u8 data_801c4ab8_slot04_12[];
extern u32 data_801c4abc_slot04_12[];
extern ObjectFn data_801c4b3c_slot04_12[];
extern s32 data_801c4b50_slot04_12[];

int is_not_state_2_five(Object *object);
void start_action_if_buttons_pressed(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void add_to_meter_clamped(Object *object, short delta);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);

void update_direction_hold_sequence_slot04_12(Object *obj);
void func_801b19bc_slot04_12(Object *obj);
void start_end_direction_sequence_slot04_12(Object *obj);
void func_801b1a54_slot04_12(Object *obj);

void start_action_variant_9_lock_other_slot04_12(Object *obj) {
    ref_other.p = obj->other;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->direction_index = 4;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p->field_6b = 0x14;
    ref_other.p->other_lock = 0x18;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1e);
}


int try_start_meter_action_state_8_variant_a_slot04_12(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state != 0) {
                if (obj->field_7e == 0) {
                    if ((u8)func_801418bc(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0xa;
                    }
                }
            } else {
                if ((u8)is_not_state_2_five(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 0xa;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            }
        }
    }
    return r;
}

void clear_two_nine_byte_groups_unless_kind_4_slot04_12(Object *obj) {
    int z = 0;
    u32 i;
    u8 *p;
    int c;

    p = (u8 *)obj + 0x2c0;
    for (i = 0, c = z; i < 9; i++) {
        *p++ = c;
    }
    if (obj->kind != 4) {
        p = (u8 *)obj + 0x2c8;
        for (i = 0; i < 9; i++) {
            *p++ = z;
        }
    }
}

void func_801b156c_slot04_12(Object *obj) {
    data_801ad398 = data_801c4a20_slot04_12[obj->field_15a](obj);
}

int return_true_always_slot04_12(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_12(Object *obj) {
    return obj->link_flags == 0;
}

int func_801b15c8_slot04_12(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int func_801b15dc_slot04_12(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int func_801b15f0_slot04_12(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_remaining_count_slot04_12(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1610_slot04_12(Object *obj) {
    data_801c4a40_slot04_12[obj->field_15a](obj);
}

void func_801b1650_slot04_12(Object *obj) {
    data_801c4a6c_slot04_12[obj->field_15a](obj);
}

void func_801b1690_slot04_12(Object *obj) {
    data_801c4a98_slot04_12[obj->state_3](obj);
}

void func_801b16d0_slot04_12(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->input_matched_flag = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->direction_index;
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_or_enter_next_state_3_slot04_12(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->countdown = 0xf;
        obj->state_3++;
        if (obj->mode_flag_2 == 0) {
            update_direction_hold_sequence_slot04_12(obj);
        } else {
            obj->countdown = data_801c4aa8_slot04_12[obj->input_dir_second];
            func_801b1a54_slot04_12(obj);
        }
    }
}

void update_direction_hold_sequence_slot04_12(Object *obj) {
    s16 a;
    int s;
    u8 t;
    u8 g;
    u8 u;
    FrameRecord *fr;

    s = (obj->step_flags << 16) >> 24;
    if (obj->mode_flag_2 != 0) {
        func_801b1a54_slot04_12(obj);
    } else {
        if ((*(u32 *)&game_state.leader_bits & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
            start_end_direction_sequence_slot04_12(obj);
            return;
        }
        if ((s & 0x7f) != 0) {
            obj->step_flags = obj->step_flags & 0x80ff;
            reset_hold_flags_and_other_hit_count(obj);
            add_one_to_ec4_if_object_unflagged(&game_state, obj);
            if (obj->input_matched_flag == 0) {
                obj->input_matched_flag = 0xff;
            }
        }
        if ((obj->pressed_bits & 0x68) == 0) {
            func_801b19bc_slot04_12(obj);
            return;
        }
        if (obj->pressed_bits & 0x40) {
            a = 0;
        } else if (obj->pressed_bits & 0x20) {
            a = 2;
        } else {
            a = 4;
        }
        if ((s16)obj->countdown <= data_801c4ab8_slot04_12[a >> 1]) {
            func_801b19bc_slot04_12(obj);
            return;
        }
        obj->countdown = 0xf;
        if (obj->direction_index == a) {
            update_sequence(obj);
            return;
        }
        obj->direction_index = a;
        u = obj->step_flags;
        g = obj->field_49;
        if (g != 0) {
            a += 0x40;
        } else {
            a += 0x21;
        }
        set_sequence_by_side_and_mode(obj, a);
        obj->sequence = &obj->sequence[u];
        obj->step_flags = obj->sequence->flags;
        obj->step_timer = obj->sequence->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        t = fr->sequence_param;
        obj->sequence_changed_flag = 1;
        obj->frame_sound_index = t;
    }
}

void func_801b19bc_slot04_12(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        start_end_direction_sequence_slot04_12(obj);
    }
}

void start_end_direction_sequence_slot04_12(Object *obj) {
    u16 a;

    obj->state_3++;
    a = 0x21;
    obj->active_flag_4 = 0;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    set_sequence_by_side_and_mode(obj, a + obj->direction_index + 1);
}

void func_801b1a54_slot04_12(Object *obj) {
    u32 *w = (u32 *)&game_state.leader_bits;
    u16 t;

    if ((*w & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
        goto tail;
    }
    t = obj->step_flags;
    if ((t & 0x7f00) == 0 || (t & 0x7f00) != 0x100) {
        update_sequence(obj);
        return;
    }
    obj->step_flags = t & 0x80ff;
    reset_hold_flags_and_other_hit_count(obj);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->input_matched_flag == 0) {
        obj->input_matched_flag = 0xff;
    }
    if (obj->input_dir_second != 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown != 0) {
            update_sequence(obj);
            return;
        }
    } else {
        if (!(u8)test_random_bit_in_table(obj, data_801c4abc_slot04_12)) {
            update_sequence(obj);
            return;
        }
    }
tail:
    obj->state_3++;
    obj->active_flag_4 = 0;
    set_sequence_by_side_and_mode(obj, obj->direction_index + 0x22);
}

void update_sequence_or_end_via_ref_other_slot04_12(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void func_801b1bfc_slot04_12(Object *obj) {
    data_801c4b3c_slot04_12[obj->state_3](obj);
}

void start_launch_with_velocity_table_a_slot04_12(Object *obj) {
    u8 one = 1;
    u16 a;

    obj->active_flag_4 = one;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = data_801c4b50_slot04_12[obj->direction_index * 2];
    obj->accel_x = data_801c4b50_slot04_12[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c4b50_slot04_12[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c4b50_slot04_12[obj->direction_index * 2 + 3];
    obj->active_state = one;
    if (obj->field_49 != 0) {
        a = (obj->direction_index >> 1) + 0x46;
    } else {
        a = 0x27;
    }
    set_sequence_by_side_and_mode(obj, a);
}
