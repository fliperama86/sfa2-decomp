/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
int spawn_random_offset_effect_index_two(Object *object);

void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
void clear_active_flag_and_finish_state_slot04_0e(Object *obj);
void start_state_1_sequence_unless_paused_slot04_0e(Object *obj);
void start_hop_velocities_or_land_by_flag_slot04_0e(Object *obj);
void land_on_ground_set_state_3_seven_slot04_0e(Object *obj);

void clear_active_flag_and_finish_state_slot04_0e(Object *obj) {
    obj->active_flag_4 = 0;
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void start_state_1_sequence_unless_paused_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    if (game_state.pause_flag_2 | game_state.field_4d) {
        clear_active_flag_and_finish_state_slot04_0e(o);
    } else {
        o->state_3 = 1;
        obj->state_flag = 0;
        o->active_flag_4 = 1;
        add_to_meter_clamped(o, 6);
        a = 0x69;
        if (o->field_49 == 0) {
            a = 0x26;
        }
        set_sequence_by_side_and_mode(o, a);
    }
}

void update_move_with_bound_push_check_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->active_flag != 0) {
        start_hop_velocities_or_land_by_flag_slot04_0e(o);
    } else {
        obj->field_47 = obj->field_47 - 1;
        if (obj->field_47 & 0x80) {
            land_on_ground_set_state_3_seven_slot04_0e(o);
        } else {
            t = 2;
            if (o->facing_side == 0) {
                t = 1;
            }
            if (o->bound_push_flag == t) {
                start_hop_velocities_or_land_by_flag_slot04_0e(o);
            } else {
                obj->countdown = (obj->countdown + 1) & 7;
                if (obj->countdown == 0) {
                    spawn_random_offset_effect_index_two(o);
                }
                update_sequence(o);
            }
        }
    }
}

void start_hop_velocities_or_land_by_flag_slot04_0e(Object *obj) {
    if (obj->field_49 != 0) {
        land_on_ground_set_state_3_seven_slot04_0e(obj);
    } else {
        obj->active_state = 1;
        obj->vel_y = 0x80000;
        obj->accel_y = -0x8000;
        obj->vel_x = 0x20000;
        obj->accel_x = 0x1000;
        obj->state_3++;
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        update_sequence(obj);
    }
}

void land_on_ground_set_state_3_seven_slot04_0e(Object *obj) {
    obj->state_3 = 7;
    obj->active_state = 0;
    obj->pos_x_frac = 0;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 0;
    obj->pos_y = (u16)obj->ground_y;
    set_sequence_by_side_and_mode(obj, 0x2b);
}
