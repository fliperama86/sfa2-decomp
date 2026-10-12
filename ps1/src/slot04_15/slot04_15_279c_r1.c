/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2e8c_slot04_15[];
extern u8 data_801c2ea4_slot04_15[];
extern u8 data_801c2ea8_slot04_15[];
extern ObjectFn data_801c3114_slot04_15[];
extern ObjectFn data_801c3128_slot04_15[];
extern ObjectFn data_801c3130_slot04_15[];

void spawn_children_per_direction_index(Object *object, int a, int b);
void set_position_from_other_box(Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void update_sequence_by_pressed_bytes(Object *object);
void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);
int drain_gauge_by_other_input(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void set_sequence_by_side(Object *object, int index);
void func_801b33c8_slot04_15(Object *object);
void move_x_by_facing_with_accel_slot04_15(Object *obj);
void func_801b2ea4_slot04_15(Object *obj);
void func_801b303c_slot04_15(Object *obj);
void func_801b30c8_slot04_15(Object *obj, s16 a);

void slide_with_deceleration_and_launch_slot04_15(Object *obj) {
    int i;
    u8 t;
    u16 a;
    s32 *tbl = data_801c2e8c_slot04_15;

    t = obj->step_flags;
    obj->other_state_code = 2;
    update_sequence(obj);
    if ((t & 0x80) != 0) {
        i = obj->direction_index;
        obj->accel_x = -0x8000;
        obj->accel_y = 0x6000;
        obj->state_3++;
        obj->vel_x = tbl[i];
        obj->vel_y = -tbl[i + 1];
    } else {
        a = obj->step_flags;
        i = a & 0xff;
        if (i == 2) {
            obj->step_flags = a & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if ((u8)obj->step_flags == 0) {
            i = 0;
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = obj->direction_index >> 1;
                    i += 1;
                }
                ((Slot04bObj *)obj)->table_value = data_801c2ea4_slot04_15[i];
            }
            i = obj->vel_x;
            if (obj->facing_side == 0) {
                i = -i;
            }
            *(s32 *)&obj->pos_x_frac = i + *(s32 *)&obj->pos_x_frac;
            obj->vel_x = obj->vel_x + obj->accel_x;
            if (obj->vel_x < 0) {
                obj->vel_x = 0;
                obj->accel_x = 0;
            }
        }
    }
}

void move_until_vel_x_negative_then_advance_slot04_15(Object *obj) {
    int v;

    obj->other_state_code = 2;
    if (obj->vel_x >= 0) {
        if ((u8)obj->step_flags == 0) {
            obj->active_state = 1;
            *(s32 *)&obj->pos_y_frac += obj->vel_y;
            obj->vel_y += obj->accel_y;
        }
        v = obj->vel_x;
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
    }
    update_sequence(obj);
}

void rise_until_vel_y_positive_slot04_15(Object *obj) {
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y > 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_fall_landing_snap_to_ground_slot04_15(Object *obj) {
    s16 y;

    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (obj->pos_y >= y) {
        obj->active_state = 0;
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->active_flag_3 = 0;
        obj->state_3++;
        play_object_sound_by_kind(obj);
    } else if ((u8)obj->step_flags != 0) {
        return;
    }
    update_sequence(obj);
}

void update_sequence_or_end_set_other_code_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b2ae8_slot04_15(Object *obj) {
    data_801c3114_slot04_15[obj->state_3](obj);
}

void func_801b2b28_slot04_15(Object *obj) {
    obj->field_157 = 1;
    obj->state_3++;
    func_80145d20(obj);
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3b);
}

void func_801b2b90_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        int a = -1;

        obj->state_3++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x1a, 0x44);
    }
}

void start_dash_on_step_end_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        ((Slot04bObj *)obj)->table_value = data_801c2ea8_slot04_15[i];
        obj->vel_x = 0x30000;
        obj->accel_x = 0;
    }
}

void func_801b2c94_slot04_15(Object *obj) {
    u16 a;

    update_sequence(obj);
    a = obj->step_flags;
    if ((a & 0xff) != 0) {
        obj->step_flags = a & 0xff00;
        obj->state_3++;
        obj->accel_x = -0x4000;
    }
    move_x_by_facing_with_accel_slot04_15(obj);
}

void func_801b2cf0_slot04_15(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags >= 0) {
        move_x_by_facing_with_accel_slot04_15(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void move_x_by_facing_with_accel_slot04_15(Object *obj) {
    int v = obj->vel_x;

    if (v >= 0) {
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac = v + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
}

void func_801b2d94_slot04_15(Object *obj) {
    data_801c3128_slot04_15[obj->state_3](obj);
}

void func_801b2dd4_slot04_15(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x1c);
}

void update_sequence_or_finish_sequence_4_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_15(Object *obj) {
    if (obj->input_dir_second != 0) {
        func_801b33c8_slot04_15(obj);
    } else {
        func_801b2ea4_slot04_15(obj);
    }
}

void func_801b2ea4_slot04_15(Object *obj) {
    data_801c3130_slot04_15[obj->state_3](obj);
}

void func_801b2ee4_slot04_15(Object *obj) {
    obj->gauge = 0xb4;
    obj->countdown = 0;
    ((Slot04bObj *)obj)->field_1cd = 0;
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x18);
    set_position_from_other_box(obj);
}

void func_801b2f54_slot04_15(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;
    u16 t;
    int k;

    update_sequence_by_pressed_bytes(obj);
    update_sequence(obj);
    set_position_from_other_box(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        obj->active_state = 1;
        play_side_sound_at(obj, obj->side, 0x15);
        spawn_effect_at_object_offset(obj, 1, 0, 0x4e);
        k = 0;
        if (o->field_1cd == 0) {
            o->field_1cd = 0xff;
            k = 6;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, k, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x16);
            func_801b30c8_slot04_15(obj, -0x200);
        } else {
            o->countdown = o->countdown + 1;
            func_801b303c_slot04_15(obj);
        }
    } else {
        func_801b303c_slot04_15(obj);
    }
}

void func_801b303c_slot04_15(Object *obj) {
    if (*(u8 *)&obj->countdown != 0) {
        if (obj->other->field_15b == 0) {
            func_801b30c8_slot04_15(obj, 2);
        } else if ((u8)drain_gauge_by_other_input(obj) != 0) {
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side_and_mode(obj, 0x19);
            set_position_from_other_box(obj);
        }
    }
}

void func_801b30c8_slot04_15(Object *obj, s16 a) {
    int x = 0x40000;
    int t;

    obj->state_3 = 6;
    t = obj->facing_side ^ 1;
    obj->facing_side = t;
    if (t != 0) {
        x = -0x40000;
    }
    obj->vel_x = x;
    obj->vel_y = -0x40000;
    obj->accel_y = 0x6000;
    set_other_hit_reaction_state_0(obj, 0x1e, 0xf, a, 0, 0, 0);
    set_sequence_by_side(obj, 0x14);
}
