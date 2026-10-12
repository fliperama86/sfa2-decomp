/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void start_action_if_buttons_pressed(Object *object);
void load_velocities_from_table_slot04_10(Object *obj);
void func_801b4444_slot04_10(Object *obj);
void func_801b3d40_slot04_10(Object *obj);
extern ObjectFn data_801c6a84_slot04_10[];
extern u16 data_801c6aa8_slot04_10[];

void update_launch_attack_by_step_flag_table_slot04_10(Object *obj) {
    s16 a[12] = { 0x17, 0, 0x1b, 0, 0x1f, 0x12, 0x14, 0xf, 0x16, 0xf, 0x18, 0xf };
    s16 b[3] = { 0x20, 0x21, 0x22 };
    s32 c[12] = { 0x38000, 0x60000, 0, -0x8000, 0x30000, 0x70000, 0, -0x8400, 0x48000, 0x80000, 0, -0x8800 };
    u16 t;
    s16 *p;
    int k;
    u8 i;

    t = obj->step_flags;
    if ((u8)t == 0) {
        update_sequence(obj);
        return;
    }
    if ((u8)t != 0xf) {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        game_state.other_object = obj->other;
        game_state.other_object->field_15b = 1;
        set_other_hit_reaction_state_0(obj, ((u8 *)b)[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 1);
        game_state.other_object = obj->other;
        if ((s16)game_state.other_object->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        obj->vel_x = c[(obj->direction_index >> 1) * 4 + 0];
        obj->vel_y = c[(obj->direction_index >> 1) * 4 + 1];
        obj->accel_y = c[(obj->direction_index >> 1) * 4 + 3];
        obj->accel_x = 0;
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
        }
    } else {
        obj->step_flags = t & 0xff00;
        game_state.config->setup_value = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        k = obj->field_49 != 0 ? 6 : 0; p = (s16 *)((obj->direction_index + k) * 2 + (u32)a);
        if ((u8)apply_damage_to_other_return_defeated(obj, p[0], p[1]) != 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
                game_state.config->countdown = 0;
                spawn_game_state_kind_object(obj);
            }
        }
        play_sound_at(obj, obj->side, 0x319);
    }
}

void func_801b3b6c_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 0;
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x1f);
    }
}

void update_or_end_with_other_state_code_5_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        game_state.other_object = obj->other;
        game_state.other_object->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    }
}

void func_801b3c5c_slot04_10(Object *obj) {
    data_801c6a84_slot04_10[obj->state_3](obj);
}

void func_801b3c9c_slot04_10(Object *obj) {
    obj->active_flag_4 = 1;
    game_state.other_object = obj->other;
    game_state.other_object->other_state_code = 0;
    obj->state_3++;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x39);
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(game_state.config, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    func_801b3d40_slot04_10(obj);
}

void func_801b3d40_slot04_10(Object *obj) {
    s16 t = *(u16 *)((u8 *)data_801c6aa8_slot04_10 + (obj->direction_index & 0xfe));
    t -= 0x26;
    if ((u8)func_8013f8c4(obj, -0x26, t) == 0) {
        obj->field_15a = 9;
        func_801b4444_slot04_10(obj);
    } else {
        obj->state_3++;
        add_to_meter_clamped(obj, 0x14);
        add_one_to_ec4_if_object_unflagged(game_state.config, obj);
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        set_sequence_by_side_and_mode(obj, 0x22);
    }
}

void update_hit_and_shift_by_step_flag_table_slot04_10(Object *obj) {
    u16 tbl[8] = { 4, 5, 6, 0, 6, 8, 9, 0 };
    int d;
    int k;
    u8 i;
    int m;
    u16 *p;
    u8 st;

    update_sequence(obj);
    st = obj->step_flags;
    p = tbl;
    if (st != 0) {
        if (st == 1) {
            m = -0x100;
            i = obj->direction_index >> 1;
            k = (obj->field_49 != 0) << 2;
            i += k;
            apply_damage_to_other_return_defeated(obj, (s16)(p[i] | m), 0);
            obj->step_flags &= m;
            game_state.config->setup_value = 0x3c;
            play_sound_at(obj, obj->side, 0x319);
            func_80146960(obj);
        } else if (st == 2) {
            obj->state_3++;
            obj->step_flags &= 0xff00;
            d = 0x60;
            if (obj->facing_side != 0) {
                d = -0x60;
            }
            obj->pos_x -= d;
        }
    }
}

void launch_from_table_on_step_flag_3_slot04_10(Object *obj) {
    s32 tbl[12] = { 0x24000, 0x90000, 0, -0x8000, 0x24000, 0xa0000, 0, -0x8400, 0x24000, 0xb0000, 0, -0x8800 };

    update_sequence(obj);
    if ((u8)obj->step_flags == 3) {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        obj->vel_x = tbl[obj->direction_index * 2];
        obj->vel_y = tbl[obj->direction_index * 2 + 1];
        obj->accel_y = tbl[obj->direction_index * 2 + 3];
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}
