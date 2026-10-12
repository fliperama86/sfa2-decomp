/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_action_if_buttons_pressed(Object *object);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void set_sequence_by_side(Object *object, int index);
void spawn_offset_effect_and_play_sounds(Object *object);
void push_other_by_vel_x(Object *object);
void func_80142c70(Object *object);
void spawn_effect_objects_at_offset(Object *object, int x, int y);
void reset_side_table_and_fields(Object *object);
void spawn_object_type_6d_for_side(Object *object);
void set_other_sequence_3_when_close(Object *object);
void func_801b3b74_slot04_15(Object *object);
void advance_state_3_start_table_sequence_alt_slot04_15(Object *obj);
void tick_countdown_or_save_counter_slot04_15(Object *object);
void apply_gravity_vertical_motion_slot04_15(Object *object);

extern u8 data_801c2df4_slot04_15[];
extern ObjectFn data_801c309c_slot04_15[];
extern ObjectFn data_801c30ac_slot04_15[];
extern ObjectFn data_801c30bc_slot04_15[];
extern ObjectFn data_801c30c4_slot04_15[];

void advance_state_3_start_table_sequence_alt_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;
    int a = 0x2b;

    o->state_3++;
    t = data_801c2df4_slot04_15[obj->table_index_3];
    o->field_225 = 1;
    o->direction_index = t * 2;
    if (o->field_49 != 0) {
        a = 0x49;
    }
    a += t;
    set_sequence_by_side_and_mode(o, (u8)a);
}

void func_801b15ac_slot04_15(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        ((Slot04bObj *)obj)->countdown = 5;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x1f;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->health = 0;
            p->pos_y = y;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            if (obj->direction_index == 0) {
                play_side_sound_at(obj, obj->side, 0x13);
            } else {
                play_side_sound_at(obj, obj->side, 0x14);
            }
        }
    }
}

void update_sequence_with_countdown_or_finish_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(o);
    } else {
        if (obj->countdown != 0) {
            obj->countdown--;
            if (obj->countdown != 0) {
                goto tail;
            }
            o->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(o);
    tail:
        update_sequence(o);
    }
}

void func_801b1788_slot04_15(Object *obj) {
    data_801c309c_slot04_15[obj->state_3](obj);
}

void start_sequence_by_active_state_slot04_15(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void update_sequence_spawn_effects_on_flag_slot04_15(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state == 0) {
            a = 1;
            b = 0x49;
        } else {
            a = -5;
            b = 0x44;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        func_801b3b74_slot04_15(obj);
    }
    update_sequence(obj);
}

void count_down_countdown_high_byte_slot04_15(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void update_counter_and_countdown_low_byte_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = (o->countdown & 0xff00) | ((o->countdown & 0xff) - 1);
    if ((t & 0xff) != 0) {
        tick_countdown_or_save_counter_slot04_15(o);
    } else {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    }
}

void tick_countdown_or_save_counter_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_134 != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    } else if (*(u8 *)&o->countdown != 0) {
        (*(u8 *)&o->countdown)--;
    }
    push_other_by_vel_x(o);
}

void func_801b1a6c_slot04_15(Object *obj) {
    data_801c30ac_slot04_15[obj->state_3](obj);
}

void func_801b1aac_slot04_15(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x2f);
    }
}

void launch_arc_on_mode_flag_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if (obj->mode != 0) {
        o->active_state = 1;
        o->state_3++;
        o->vel_x = 0x18000;
        o->vel_y = 0xfff88000;
        o->accel_y = 0x6000;
    }
}

void update_airborne_until_landing_fixed_vel_slot04_15(Object *obj) {
    int d;

    apply_gravity_vertical_motion_slot04_15(obj);
    if (obj->ground_y > obj->pos_y) {
        d = obj->vel_x;
        if (obj->facing_side == 0) {
            d = -d;
        }
        *(s32 *)&obj->pos_x_frac = d + *(s32 *)&obj->pos_x_frac;
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}

void update_sequence_or_finish_notify_other_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c64_slot04_15(Object *obj) {
    data_801c30bc_slot04_15[obj->state_3](obj);
}

void func_801b1ca4_slot04_15(Object *obj) {
    s16 t = obj->step_flags;

    if (t >= 0) {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x30);
    }
}

void update_sequence_or_finish_sequence_2_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void func_801b1d54_slot04_15(Object *obj) {
    data_801c30c4_slot04_15[obj->state_3](obj);
}
