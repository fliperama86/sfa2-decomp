/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_action_if_buttons_pressed(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void func_801b1d8c_slot04_14(Object *obj);

extern ObjectFn data_801c62b8_slot04_14[];
extern ObjectFn data_801c62c4_slot04_14[];

void advance_state_3_when_mode_zero_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b1b48_slot04_14(Object *obj) {
    func_801b1d8c_slot04_14(obj);
    if (obj->pos_y < obj->ground_y) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3++;
        }
    } else {
        obj->active_flag_4 = 0;
        obj->field_49 = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void update_fall_until_landing_or_mode_set_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 y;

    func_801b1d8c_slot04_14(o);
    y = o->ground_y;
    if (o->pos_y < y) {
        if (obj->mode != 0) {
            return;
        }
    } else {
        o->state_3++;
        o->pos_y = y;
        o->active_flag_4 = 0;
        o->active_flag_3 = 0;
        o->active_state = 0;
        play_object_sound_by_kind(o);
    }
    update_sequence(o);
}

void func_801b1c8c_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void func_801b1cfc_slot04_14(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0x80000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        set_sequence_by_side_and_mode(obj, 0x60);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}

void func_801b1d8c_slot04_14(Object *obj) {
    if (obj->field_49 != 0 && obj->vel_y < 0) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}

void func_801b1dd8_slot04_14(Object *obj) {
    data_801c62b8_slot04_14[obj->state_3](obj);
}

void func_801b1e18_slot04_14(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void spawn_child_object_when_mode_set_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    u16 y;

    update_sequence(o);
    if (obj->mode != 0) {
        o->state_3++;
        o->countdown = obj->countdown | 0x400;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x14;
            p->active_flag_2 = 0;
            p->health = 0;
            p->table_index = 0;
            p->sound_side = o->sound_side;
            p->team_id = o->team_id;
            p->vel_table_index = o->direction_index >> 1;
            p->field_0e = o->field_0e;
            p->facing_side = o->facing_side;
            p->field_0c = o->field_0c;
            p->image_row = o->image_row;
            p->scroll_factor = o->scroll_factor;
            p->pos_x = o->pos_x;
            p->pos_y = o->pos_y;
            y = o->ground_y;
            p->spawn_x = 0x60;
            p->parent = o;
            p->spawn_y = 0x1e0;
            p->ground_y = y;
            p->field_90 = o->field_90;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
            o->linked_object_3 = (s32)p;
            o->link_flags++;
            play_side_sound_at(o, o->side, 0xb);
        }
    }
}

void update_sequence_with_high_byte_countdown_slot04_14(Object *obj) {
    s16 t;

    if (!((s16)obj->step_flags & 0x8000)) {
        t = obj->countdown;
        if (t & 0xff00) {
            t = t - 0x100;
            obj->countdown = t;
            if (!(t & 0xff00)) {
                obj->active_flag_4 = 0;
                start_action_if_buttons_pressed(obj);
            }
        } else {
            start_action_if_buttons_pressed(obj);
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b2068_slot04_14(Object *obj) {
    data_801c62c4_slot04_14[obj->state_3](obj);
}
