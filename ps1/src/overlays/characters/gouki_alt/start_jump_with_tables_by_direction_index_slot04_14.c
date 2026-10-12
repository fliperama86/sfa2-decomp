/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c62d4_slot04_14[];
extern s32 data_801c62e0_slot04_14[];
extern u16 data_801c62ec_slot04_14[];
extern ObjectFn data_801c62f4_slot04_14[];
extern s32 data_801c6304_slot04_14[];
extern ObjectFn data_801c6310_slot04_14[];

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
int apply_velocity_and_check_ground(Object *object);
void set_sequence_by_side(Object *object, int arg);
void land_set_ground_states_and_sequence_slot04_14(Object *obj);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
u8 move_with_gravity_return_above_ground_slot04_14(Object *obj);
void land_on_ground_play_sound_set_landing_sequence_slot04_14(Object *obj);

void start_jump_with_tables_by_direction_index_slot04_14(Object *obj) {
    int i = 1;

    obj->active_flag_4 = i;
    obj->active_state = i;
    obj->state_3 = i;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    i = obj->direction_index >> 1;
    *(s32 *)&obj->vel_y = 0x40000;
    obj->accel_y = -0x6000;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    if (obj->field_49 != 0) {
        obj->accel_y = data_801c62d4_slot04_14[i];
    }
    obj->vel_x = -data_801c62e0_slot04_14[(i & 0xff)];
    if (obj->facing_side != 0) {
        obj->vel_x = data_801c62e0_slot04_14[(i & 0xff)];
    }
    set_sequence_by_side_and_mode(obj, data_801c62ec_slot04_14[(i & 0xff)]);
}

void move_until_apex_then_start_fall_sequence_slot04_14(Object *obj) {
    u16 a;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x320);
        if (obj->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        ((Slot04bObj *)obj)->countdown_2 = 1;
        if (obj->direction_index != 0) {
            ((Slot04bObj *)obj)->countdown_2 = obj->direction_index;
        }
        a += obj->direction_index >> 1;
        set_sequence_by_side_and_mode(obj, a);
    } else {
        update_sequence(obj);
    }
}

void update_repeat_sequence_with_countdown_2_slot04_14(Object *obj) {

    if ((s16)obj->step_flags & 0x8000) {
        play_sound_at(obj, obj->side, 0x320);
        ((Slot04bObj *)obj)->countdown_2--;
        if (((Slot04bObj *)obj)->countdown_2 == 0) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void move_then_land_sound_and_reset_or_update_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (move_with_gravity_return_above_ground_slot04_14(obj) == 0) {
        play_object_sound_by_kind(obj);
        s1->other_state_code = 5;
        s1->pos_y_frac = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_7_slot04_14(Object *obj) {
    data_801c62f4_slot04_14[obj->state_3](obj);
}

void start_bounce_with_sound_and_sequence_slot04_14(Object *obj) {
    s32 d;

    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->vel_y < 0) {
        d = obj->accel_y;
        obj->accel_y = d >> 1;
    } else {
        obj->accel_y = -0x4800;
    }
    d = data_801c6304_slot04_14[obj->direction_index >> 1];
    if (obj->facing_side != 0) {
        if (obj->vel_x < 0) {
            d = -d;
        }
    } else {
        if (obj->vel_x <= 0) {
            d = -d;
        }
    }
    obj->vel_x = d + obj->vel_x;
    play_sound_at(obj, obj->side, 0x320);
    ((Slot04bObj *)obj)->countdown_2 = 1;
    if (obj->direction_index != 0) {
        ((Slot04bObj *)obj)->countdown_2 = obj->direction_index;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x23);
}

void update_airborne_repeat_countdown_or_land_slot04_14(Object *obj) {

    if (move_with_gravity_return_above_ground_slot04_14(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            play_sound_at(obj, obj->side, 0x320);
            ((Slot04bObj *)obj)->countdown_2--;
            if (((Slot04bObj *)obj)->countdown_2 == 0) {
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, 0x1f);
                return;
            }
        }
        update_sequence(obj);
    } else {
        land_on_ground_play_sound_set_landing_sequence_slot04_14(obj);
    }
}

void update_airborne_until_sequence_end_or_land_slot04_14(Object *obj) {
    if (move_with_gravity_return_above_ground_slot04_14(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_3++;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        land_on_ground_play_sound_set_landing_sequence_slot04_14(obj);
    }
}

void update_fall_land_set_states_and_sequence_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (move_with_gravity_return_above_ground_slot04_14(obj) == 0) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 3, 2);
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        play_object_sound_by_kind(obj);
        s1->other_state_code = 5;
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}

void land_on_ground_play_sound_set_landing_sequence_slot04_14(Object *obj) {
    obj->state_3++;
    play_object_sound_by_kind(obj);
    obj->active_flag_3 = 0;
    obj->active_state = 0;
    obj->pos_y_frac = 0;
    obj->pos_y = ((Slot04bObj *)obj)->ground_y;
    set_sequence_by_side(obj, 0x11);
}

u8 move_with_gravity_return_above_ground_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->ground_y > obj->pos_y;
}

void call_table_function_by_state_3_8_slot04_14(Object *obj) {
    data_801c6310_slot04_14[obj->state_3](obj);
}

void advance_state_3_add_meter_set_sequence_slot04_14(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x26);
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void airborne_spawn_projectile_type_1_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        land_set_ground_states_and_sequence_slot04_14(obj);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 1) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0x1a;
                p->active_flag_2 = 0;
                p->health = 0;
                p->table_index = 0;
                p->sound_side = obj->sound_side;
                p->team_id = obj->team_id;
                p->vel_table_index = obj->direction_index >> 1;
                p->facing_side = obj->facing_side;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->image_row = obj->image_row;
                p->scroll_factor = obj->scroll_factor;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->ground_y;
                p->spawn_x = 0x60;
                p->parent = obj;
                p->spawn_y = 0x1e0;
                ((Slot04bObj *)p)->ground_y = t;
                p->field_90 = obj->field_90;
                p->data_ptr_a = obj->data_ptr_a;
                p->data_ptr_b = obj->data_ptr_b;
                obj->linked_object_3 = (s32)p;
                obj->link_flags++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void airborne_spawn_projectile_type_2_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        land_set_ground_states_and_sequence_slot04_14(obj);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 2) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0x14;
                p->table_index = 2;
                p->active_flag_2 = 0;
                p->health = 0;
                p->sound_side = obj->sound_side;
                p->team_id = obj->team_id;
                p->vel_table_index = (obj->direction_index >> 1) + 3;
                p->facing_side = obj->facing_side;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->image_row = obj->image_row;
                p->scroll_factor = obj->scroll_factor;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->ground_y;
                p->spawn_x = 0x60;
                p->parent = obj;
                p->spawn_y = 0x1e0;
                ((Slot04bObj *)p)->ground_y = t;
                p->field_90 = obj->field_90;
                p->data_ptr_a = obj->data_ptr_a;
                p->data_ptr_b = obj->data_ptr_b;
                obj->linked_object_3 = (s32)p;
                obj->link_flags++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}
