/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int apply_velocity_and_check_ground(Object *object);
int check_hit_other_active_state_nonzero(Object *object, s16 a, s16 b, s16 c, u16 d);
void spawn_three_child_records_at_other(Object *object);
void load_velocities_from_table_slot04_10(Object *object);
void move_with_x_stop_on_direction_mismatch_slot04_10(Object *object);

void airborne_move_and_pick_hit_sequence_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 0;
        obj->active_state = 1;
        move_with_x_stop_on_direction_mismatch_slot04_10(obj);
        if (obj->vel_y < 0) {
            obj->state_3++;
            obj->vel_x = 0;
            obj->accel_x = 0;
            set_sequence_by_side_and_mode(obj, 0x32);
        } else if (check_hit_other_active_state_nonzero(obj, -0x3e, 0x18, 0x40, 0x1a) != 0) {
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            spawn_three_child_records_at_other(obj);
            if (obj->direction_index == 4) {
                play_side_sound_at(obj, obj->side, 7);
                obj->state_3 = 0xe;
                set_sequence_by_side_and_mode(obj, 0x37);
            } else if (obj->direction_index == 2) {
                obj->state_3 = 9;
                set_sequence_by_side_and_mode(obj, 0x34);
            } else {
                obj->state_3 = 6;
                set_sequence_by_side_and_mode(obj, 0x31);
            }
        }
    }
}

void move_with_x_stop_on_direction_mismatch_slot04_10(Object *obj) {
    if (obj->vel_x >= 0) {
        if (obj->facing_side == 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->facing_side != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
}

void fall_until_ground_clear_flag_seq_33_a_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->pos_y = obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x33);
    }
}

void update_sequence_or_set_other_code_5_2_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    }
}
