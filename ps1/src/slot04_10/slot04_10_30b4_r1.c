/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern ObjectFn data_801c6a64_slot04_10[];
void start_action_if_buttons_pressed(Object *object);
int apply_velocity_and_check_ground(Object *object);
void spawn_three_child_records_at_other(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void load_velocities_from_table_slot04_10(Object *object);
void add_flag_indexed_offset_to_pos_x_slot04_10(Object *object);
void check_range_then_start_hit_sequence_slot04_10(Object *object);

void update_sequence_and_spawn_pooled_object_slot04_10(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = *(u8 *)&obj->step_flags;
    if (t == 0) {
        return;
    }
    if (t != 3) {
        add_flag_indexed_offset_to_pos_x_slot04_10(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    add_flag_indexed_offset_to_pos_x_slot04_10(obj);
    game_state.other_object = pop_object_from_pool(obj);
    if (game_state.other_object != 0) {
        game_state.other_object->active_count = 1;
        game_state.other_object->field_02 = 0x10;
        game_state.other_object->sound_side = obj->sound_side;
        game_state.other_object->team_id = obj->team_id;
        game_state.other_object->vel_table_index = obj->direction_index;
        game_state.other_object->active_flag_2 = 0;
        game_state.other_object->field_0e = obj->field_0e;
        game_state.other_object->scroll_factor = obj->scroll_factor;
        game_state.other_object->pos_x = obj->pos_x;
        game_state.other_object->pos_y = obj->pos_y;
        game_state.other_object->facing_side = obj->facing_side;
        game_state.other_object->health = 0;
        game_state.other_object->parent = obj;
    }
}

void add_flag_indexed_offset_to_pos_x_slot04_10(Object *obj) {
    u16 tbl[20] = { 0xe, 0x10, 0x14, 0, 1, 3, 7, 0, 3, 5, 9, 0, 0x16, 0x18, 0x1c, 0, 8, 0x10, 0x18, 0 };
    u16 d;

    d = tbl[*(u8 *)&obj->step_flags * 4 + (obj->direction_index >> 1)];
    if (obj->facing_side == 0) {
        d = -d;
    }
    obj->pos_x = obj->pos_x + d;
    obj->step_flags = obj->step_flags & 0xff00;
}

void update_sequence_advance_state_on_flag_4_slot04_10(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 4) {
        obj->active_flag_4 = 0;
        obj->state_3 = obj->state_3 + 1;
        add_flag_indexed_offset_to_pos_x_slot04_10(obj);
    }
}

void update_sequence_or_end_move_a_2_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        load_velocities_from_table_slot04_10(obj);
    }
}

void call_table_function_by_state_3_14_slot04_10(Object *obj) {
    data_801c6a64_slot04_10[obj->state_3](obj);
}

void add_meter_start_direction_sequence_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int one = 1;

    o->active_flag_4 = one;
    obj->field_298 = 0;
    o->state_3 = o->state_3 + 1;
    game_state.other_object = o->other;
    game_state.other_object->other_state_code = 0;
    add_to_meter_clamped(o, 5);
    add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, o);
    set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x40);
    if (o->field_49 != 0) {
        o->field_225 = one;
    }
    check_range_then_start_hit_sequence_slot04_10(o);
}

void check_range_then_start_hit_sequence_slot04_10(Object *obj) {
    u16 tbl[4] = { 0x68, 0x65, 0x61, 0 };
    u16 tbl2[4] = { 0x68, 0x65, 0x61, 0 };
    u16 x;
    int d;

    x = tbl[(obj->direction_index & 0xfe) >> 1];
    if (obj->field_49 != 0) {
        x = tbl[((obj->direction_index & 0xfe) >> 1) + 4];
    }
    d = obj->other_state_code != 0;
    d <<= 4;
    d += 0x26;
    if ((u8)try_force_other_state_if_box_in_range(obj, -0x26, (s16)(x - d)) == 0) {
        obj->state_3 = 7;
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        add_to_meter_clamped(obj, 0x14);
        add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, obj);
        if (obj->field_49 != 0) {
            spawn_three_child_records_at_other(obj);
        }
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        set_sequence_by_side_and_mode(obj, 0x1d);
    }
}

void launch_from_table_with_edge_check_slot04_10(Object *obj) {
    s32 tbl[3][4] = { { 0x11000, 0x98000, 0x2000, -0x8000 }, { 0x14000, 0xac000, 0x1000, -0x6000 }, { 0x14000, 0xa8000, 0x1000, -0x4000 } };

    if (*(u8 *)&obj->step_flags != 1) {
        update_sequence(obj);
        return;
    }
    obj->state_3 = obj->state_3 + 1;
    obj->active_state = 1;
    obj->step_flags = obj->step_flags & 0xff00;
    obj->vel_x = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 0);
    obj->vel_y = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 4);
    obj->accel_x = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 8);
    obj->accel_y = *(s32 *)((((u32)obj->direction_index >> 1) << 4) + (u32)tbl + 12);
    if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        } else {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->facing_side != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}

void move_until_vel_y_negative_set_accel_y_slot04_10(Object *obj) {
    s32 tbl[3] = { -0x9000, -0xc000, -0xc000 };

    apply_velocity_and_check_ground(obj);
    if (obj->vel_y >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->accel_y = tbl[obj->direction_index >> 1];
        obj->accel_x = 0;
        update_sequence(obj);
    }
}

void update_fall_land_clear_active_state_slot04_10(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        set_sequence_by_side_and_mode(obj, 0x1e);
    }
}
