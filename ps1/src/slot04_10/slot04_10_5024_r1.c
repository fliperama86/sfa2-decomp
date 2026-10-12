/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;

void move_with_x_stop_on_direction_mismatch_slot04_10(Object *obj);
void load_velocities_from_table_slot04_10(Object *obj);
void load_velocities_from_table_2_slot04_10(Object *obj);
int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);

void update_throw_hit_other_then_flip_facing_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (obj->vel_y >= 0) {
            move_with_x_stop_on_direction_mismatch_slot04_10(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0x1f, 5, 0x16, 0x1a, 1, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 0;
            spawn_game_state_kind_object(obj);
        }
        obj->vel_x = 0x10000;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x10000;
        }
        if (obj->vel_y >= 0) {
            obj->vel_y = 0;
        }
        obj->facing_side ^= 1;
        set_sequence_by_side_and_mode(obj, 0x32);
    }
}

void fall_until_ground_clear_flag_seq_33_b_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x33);
    }
}

void update_sequence_or_set_other_code_5_3_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    }
}

void func_801b5250_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (obj->vel_y >= 0) {
            move_with_x_stop_on_direction_mismatch_slot04_10(obj);
        }
        update_sequence(obj);
    } else {
        obj->accel_y = 0xffff6000;
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void func_801b52e0_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x35);
    }
}

void func_801b535c_slot04_10(Object *obj) {
    u16 v = obj->step_flags;
    u8 st = v;
    if (st == 0) {
        update_sequence(obj);
    } else if (st == 0xf) {
        obj->step_flags = v & 0xff00;
        data_80190468.p->vec_table_index = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        play_side_sound_at(obj, obj->side, 0xa);
        if ((u8)apply_damage_to_other_return_defeated(obj, 0xd, 0x1b)) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            spawn_game_state_kind_object(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0x23, 0xf, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        obj->vel_x = 0x48000;
        obj->vel_y = 0x90000;
        obj->accel_x = 0;
        obj->accel_y = 0xffff7800;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void func_801b5528_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x36);
    }
}

void func_801b55a4_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side ^= 1;
        load_velocities_from_table_2_slot04_10(obj);
    }
}

void func_801b5610_slot04_10(Object *obj) {
    move_with_x_stop_on_direction_mismatch_slot04_10(obj);
    if (obj->vel_y < 0) {
        obj->accel_y = -0x8000;
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
    update_sequence(obj);
}

void update_airborne_or_land_damage_other_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        data_80190468.p->vec_table_index = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        set_sequence_by_side_and_mode(obj, 0x38);
        if ((u8)apply_damage_to_other_return_defeated(obj, 0xa, 0x1c)) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            spawn_game_state_kind_object(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    }
}
