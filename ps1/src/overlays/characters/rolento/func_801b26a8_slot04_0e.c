/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFnInt data_801c5f90_slot04_0e[];
extern ObjectFn data_801c5fc8_slot04_0e[];
extern ObjectFn data_801c6000_slot04_0e[];
extern s32 data_801c601c_slot04_0e[];
extern s32 data_801c6028_slot04_0e[];
extern ObjectFn data_801c6034_slot04_0e[];

void start_action_if_buttons_pressed(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void land_on_ground_start_sequence_thirty_two_slot04_0e(Object *obj);
void set_variant_index_from_byte_pair_slot04_0e(Object *obj);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
BytePair get_input_byte_pair_or_stored_pair_slot04_0e(Slot04bObj *obj);
void call_handler_by_handler_index_if_active_slot04_0e(Slot04bObj *obj);
u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj);

void func_801b26a8_slot04_0e(Object *obj) {
    data_801ad398 = data_801c5f90_slot04_0e[obj->field_15a](obj);
}

int is_link_flags_zero_slot04_0e(Object *obj) {
    return obj->link_flags == 0;
}

int return_true_always_slot04_0e(Object *obj) {
    return 1;
}

int return_zero_slot04_0e(Object *obj) {
    return 0;
}

int is_meter_at_least_threshold_slot04_0e(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int is_flag_set_or_remaining_count_nonzero_slot04_0e(Object *obj) {
    if (obj->field_7e == 0) {
        return obj->remaining_count != 0;
    }
    return 1;
}

int return_zero_2_slot04_0e(Object *obj) {
    return 0;
}

void func_801b274c_slot04_0e(Object *obj) {
    data_801c5fc8_slot04_0e[obj->field_15a](obj);
}

void call_table_function_by_state_3_6_slot04_0e(Object *obj) {
    data_801c6000_slot04_0e[obj->state_3](obj);
}

void start_airborne_sequence_by_direction_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    o->active_flag_4 = 1;
    obj->state_flag = 0;
    o->vel_y = 0;
    o->accel_y = 0;
    o->state_3++;
    add_to_meter_clamped(o, 3);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    call_handler_by_handler_index_if_active_slot04_0e((Slot04bObj *)o);
    if (o->field_49 == 0) {
        a = 0x1d;
    } else {
        a = 0x58;
    }
    a += o->direction_index >> 1;
    set_sequence_by_side_and_mode(o, a);
}

void launch_with_table_velocity_on_mode_0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    if (obj->state_flag == 0) {
        set_variant_index_from_byte_pair_slot04_0e(o);
    }
    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
        obj->mode = 0;
        o->vel_x = 0;
        o->accel_x = 0;
        o->vel_y = data_801c601c_slot04_0e[o->direction_index >> 1];
        o->accel_y = data_801c6028_slot04_0e[o->direction_index >> 1];
    }
    update_sequence(o);
}

void airborne_motion_land_or_switch_sequence_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    if (obj->state_flag == 0) {
        set_variant_index_from_byte_pair_slot04_0e(o);
    }
    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->ground_y < o->pos_y) {
        land_on_ground_start_sequence_thirty_two_slot04_0e(o);
    } else if (o->pos_y >= (s16)(o->ground_y - 0x18) || obj->state_flag == 0) {
        update_sequence(o);
    } else {
        a = 0x5b;
        o->state_3++;
        if (o->field_49 == 0) {
            a = 0x52;
        }
        a += o->direction_index >> 1;
        set_sequence_by_side_and_mode(o, a);
    }
}

void spawn_linked_pooled_object_on_mode_1_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s16 y;

    if (obj->mode == 1) {
        p = pop_object_from_pool(o);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xe;
            p->table_index = 0;
            p->sound_side = o->sound_side;
            p->team_id = o->team_id;
            p->field_49 = o->field_49;
            p->vel_table_index = o->variant_index;
            p->active_flag_2 = 0;
            p->field_0e = o->field_0e;
            p->facing_side = o->facing_side;
            p->field_0c = o->field_0c;
            p->image_row = o->image_row;
            p->scroll_factor = o->scroll_factor;
            p->pos_x = o->pos_x;
            y = o->pos_y;
            p->health = 0;
            p->parent = o;
            p->pos_y = y;
            o->link_flags++;
            o->linked_object_3 = (s32)p;
            p->field_90 = o->field_90;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
        }
        obj->mode = 0;
        obj->field_47 = 2;
        o->state_3++;
        play_side_sound_at(o, o->field_02, 0x16);
        play_side_sound_at(o, o->field_02, 9);
    }
    update_sequence(o);
}

void start_fall_when_countdown_expires_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_47 -= 1;
    if (obj->field_47 & 0x80) {
        o->accel_y = -0x8000;
        o->step_timer = 1;
        o->state_3++;
        o->vel_y = 0;
        if (o->field_49 != 0) {
            o->accel_y = 0xfffe0000;
        }
        update_sequence(o);
    }
}

void move_by_velocity_update_or_land_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if (obj->ground_y >= obj->pos_y) {
        update_sequence(obj);
    } else {
        land_on_ground_start_sequence_thirty_two_slot04_0e(obj);
    }
}

void land_on_ground_start_sequence_thirty_two_slot04_0e(Object *obj) {
    obj->state_3 = 6;
    obj->active_state = 0;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 0;
    obj->pos_y = (u16)obj->ground_y;
    play_object_sound_by_kind(obj);
    set_sequence_by_side_and_mode(obj, 0x20);
}

void update_sequence_or_end_move_a_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void set_variant_index_from_byte_pair_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair buf;
    int t;

    if (o->mode_flag_2 != 0) {
        t = call_handler_by_limit_hi_returning_slot04_0e(o);
    } else {
        t = obj->field_134;
    }
    if (t != 0) {
        buf = get_input_byte_pair_or_stored_pair_slot04_0e((Slot04bObj *)o);
        o->variant_index = buf.first;
    }
}

void call_table_function_by_state_3_7_slot04_0e(Object *obj) {
    data_801c6034_slot04_0e[obj->state_3](obj);
}

void start_jump_state_with_sequence_and_sound_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    obj->state_flag = 0;
    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    call_handler_by_handler_index_if_active_slot04_0e((Slot04bObj *)o);
    if (o->field_49 != 0) {
        a = 0x5e;
    } else {
        a = 0x2c;
    }
    set_sequence_by_side_and_mode(o, a);
    play_side_sound_at(o, obj->field_a6, 9);
}

void start_jump_toward_other_at_sequence_end_slot04_0e(Object *o) {
    int d;
    int a;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->accel_y = -0x4800;
        o->state_3++;
        o->vel_y = 0x68000;
        d = *(s32 *)&o->other->pos_x_frac;
        d -= *(s32 *)&o->pos_x_frac;
        if (d > 0) {
            d = -d;
        }
        d += 0x1800000;
        if (o->facing_side != 0) {
            d = -d;
        }
        a = 0x5f;
        d >>= 3;
        if (o->field_49 == 0) {
            a = 0x2d;
            d >>= 1;
        }
        o->vel_x = d;
        o->accel_x = 0;
        set_sequence_by_side_and_mode(o, a);
    }
}
