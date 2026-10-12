/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c6bb8_slot04_10[];
extern s32 data_801c6bc0_slot04_10[];
extern ObjectFn data_801c6bf0_slot04_10[];
extern s32 data_801c6c38_slot04_10[];

int apply_velocity_and_check_ground(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void start_action_if_buttons_pressed(Object *object);
void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);
void load_velocities_from_table_slot04_10(Object *object);
void move_x_decelerate_to_zero_slot04_10(Object *object);

void start_table_launch_hit_unless_flag_zero_slot04_10(Object *obj) {
    s32 a;

    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, data_801c6bb8_slot04_10[obj->direction_index & 0xfe], 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if (((Slot04bObj *)ref_other.p)->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        a = data_801c6bc0_slot04_10[obj->direction_index * 2];
        obj->vel_y = data_801c6bc0_slot04_10[obj->direction_index * 2 + 1];
        obj->accel_y = data_801c6bc0_slot04_10[obj->direction_index * 2 + 3];
        obj->accel_x = 0;
        if (obj->facing_side != 0) {
            a = -a;
        }
        obj->vel_x = a;
    }
}

void update_fall_land_keep_active_state_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        set_sequence_by_side_and_mode(obj, 0x28);
    }
}

void signal_other_and_end_or_update_sequence_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->active_flag_4 = 0;
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        load_velocities_from_table_slot04_10(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void update_decelerate_until_step_flag_byte_slot04_10(Object *obj) {
    move_x_decelerate_to_zero_slot04_10(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = 8;
    }
    update_sequence(obj);
}

void move_x_decelerate_to_zero_slot04_10(Object *obj) {
    s32 d = obj->vel_x;

    if (obj->facing_side == 0) {
        d = -d;
    }
    *(s32 *)&obj->pos_x_frac += d;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x < 0) {
        obj->accel_x = 0;
        obj->vel_x = 0;
    }
}

void call_table_function_by_state_3_17_slot04_10(Object *obj) {
    data_801c6bf0_slot04_10[obj->state_3](obj);
}

void start_table_velocity_sequence_with_effects_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_298 = 0;
    o->state_3++;
    update_meter_by_mode_or_table(o);
    set_flag_225_add_direction_to_counter((GameState *)game_state.config, o);
    spawn_three_child_objects_at_object_spawn(o);
    o->vel_x = data_801c6c38_slot04_10[o->direction_index * 2];
    o->vel_y = data_801c6c38_slot04_10[o->direction_index * 2 + 1];
    o->accel_x = data_801c6c38_slot04_10[o->direction_index * 2 + 2];
    o->accel_y = data_801c6c38_slot04_10[o->direction_index * 2 + 3];
    if (o->facing_side == 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    set_sequence_by_side_and_mode(o, 0x30);
}

void update_sequence_spawn_children_on_high_flag_slot04_10(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        spawn_children_per_direction_index(obj, -0x24, 0x2b);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}
