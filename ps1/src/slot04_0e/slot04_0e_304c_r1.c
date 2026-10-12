/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6050_slot04_0e[];

void start_action_if_buttons_pressed(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);

void land_on_ground_start_sequence_forty_eight_slot04_0e(Object *obj);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
u8 is_second_input_direction_active_slot04_0e(Object *obj);
void halve_vel_x_add_meter_set_sequence_slot04_0e(Object *obj);
BytePair get_input_byte_pair_or_stored_pair_slot04_0e(Slot04bObj *obj);
u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj);

void set_direction_bytes_from_byte_pair_slot04_0e(Object *obj);
void nudge_pos_x_by_input_bits_slot04_0e(Object *obj, int arg);
void start_direction_sequence_or_special_slot04_0e(Object *obj);
void update_air_motion_land_or_pick_landing_sequence_slot04_0e(Object *obj);

void update_airborne_move_and_land_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->state_flag == 0) {
        set_direction_bytes_from_byte_pair_slot04_0e(o);
    }
    apply_velocity_and_accel_to_position_slot04_0e(o);
    nudge_pos_x_by_input_bits_slot04_0e(o, 3);
    if (obj->state_flag != 0) {
        o->state_3 = o->state_3 + 1;
        o->field_28a = 1;
        start_direction_sequence_or_special_slot04_0e(o);
    } else if (o->ground_y < o->pos_y) {
        land_on_ground_start_sequence_forty_eight_slot04_0e(o);
    } else {
        update_sequence(o);
    }
}

void move_with_input_nudge_update_or_land_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    nudge_pos_x_by_input_bits_slot04_0e(obj, 5);
    if (obj->ground_y < obj->pos_y) {
        land_on_ground_start_sequence_forty_eight_slot04_0e(obj);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_or_clear_flag_and_end_move_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        obj->field_28a = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void nudge_pos_x_by_input_bits_slot04_0e(Object *obj, int arg) {
    u16 d = arg;

    if (obj->mode_flag_2 == 0 && (obj->input_bits & 0xa000) != 0) {
        if (obj->input_bits & 0x8000) {
            d = -d;
        }
        *(u16 *)&obj->pos_x = d + *(u16 *)&obj->pos_x;
    }
}

void start_direction_sequence_or_special_slot04_0e(Object *obj) {
    u16 a;
    u16 h;

    if (is_second_input_direction_active_slot04_0e(obj) != 0) {
        halve_vel_x_add_meter_set_sequence_slot04_0e(obj);
    } else {
        h = obj->direction_index >> 1;
        add_to_meter_clamped(obj, h);
        a = 0x15;
        if (obj->input_dir_second == 0) {
            a = 0x12;
        }
        set_sequence_by_side_and_mode(obj, a += h);
    }
}

void set_direction_bytes_from_byte_pair_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair buf;
    int k;

    if (o->mode_flag_2 != 0) {
        k = call_handler_by_limit_hi_returning_slot04_0e(o);
    } else {
        k = obj->field_134;
    }
    if (k != 0) {
        buf = get_input_byte_pair_or_stored_pair_slot04_0e((Slot04bObj *)o);
        o->direction_index = buf.first;
        o->input_dir_second = buf.second;
    }
}

void call_table_function_by_state_3_8_slot04_0e(Object *obj) {
    data_801c6050_slot04_0e[obj->state_3](obj);
}

void func_801b3338_slot04_0e(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 2);
    obj->accel_y -= 0x6000;
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    update_air_motion_land_or_pick_landing_sequence_slot04_0e(obj);
}
