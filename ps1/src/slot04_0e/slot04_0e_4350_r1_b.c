/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c60e0_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

void func_80146960(Object *object);
void set_other_hit_reaction_state_0(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void apply_facing_velocity_with_x_clamp_slot04_0e(Object *obj);
void move_other_by_side_record_velocity_slot04_0e(Object *obj);

void func_801b4410_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    obj->field_47 += 0xff;
    if ((obj->field_47 & 0x80) != 0) {
        obj->table_value = 0;
        o->substate_index = o->substate_index + 1;
        set_sequence_by_side_and_mode(o, 0x44);
    }
}

void launch_on_step_flag_zero_slot04_0e(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->vel_x = 0x90000;
        obj->vel_y = 0x50000;
        obj->accel_x = 0xffff4000;
        obj->accel_y = -0x8000;
        obj->substate_index = obj->substate_index + 1;
    }
    update_sequence(obj);
}

void update_rise_until_apex_then_start_fall_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    apply_facing_velocity_with_x_clamp_slot04_0e(o);
    if (o->vel_y >= 0) {
        update_sequence(o);
    } else {
        obj->field_47 = 6;
        o->vel_x = 0;
        o->accel_x = 0;
        o->vel_y = 0;
        o->accel_y = 0xffff4000;
        o->substate_index = o->substate_index + 1;
        play_side_sound_at(o, o->side, 0x14);
    }
}

void count_down_then_set_side_record_velocity_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p = o->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[o->side];

    obj->field_47 += 0xff;
    if ((obj->field_47 & 0x80) != 0) {
        apply_facing_velocity_with_x_clamp_slot04_0e(o);
        o->substate_index = o->substate_index + 1;
        r->vel_y = 0;
        r->accel_y = 0x8000;
        r->accel_x = 0;
        r->vel_x = (*(s32 *)&o->pos_x_frac - *(s32 *)&p->pos_x_frac) >> 4;
        update_sequence(o);
    }
}

void update_landing_with_two_side_sounds_slot04_0e(Object *obj) {
    move_other_by_side_record_velocity_slot04_0e(obj);
    apply_facing_velocity_with_x_clamp_slot04_0e(obj);
    if (obj->ground_y < obj->pos_y) {
        obj->step_timer = 1;
        obj->substate_index = obj->substate_index + 1;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        play_side_sound_at(obj, obj->side, 0x12);
        play_side_sound_at(obj, obj->side, 6);
    }
    update_sequence(obj);
}

void func_801b4678_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];

    move_other_by_side_record_velocity_slot04_0e(obj);
    update_sequence(obj);
    if ((s16)obj->step_flags < 0) {
        obj->substate_index = obj->substate_index + 1;
        game_state.state_param = 0x18;
        *(u8 *)&obj->step_flags = 0;
        r->vel_y = 0;
        r->vel_x = 0;
        r->accel_x = 0;
        r->accel_y = 0;
        r->vel_y = 0xfff80000;
        r->accel_y = -0x8000;
        apply_damage_to_other_return_defeated(obj, data_801c60e0_slot04_0e[obj->direction_index], data_801c60e0_slot04_0e[obj->direction_index + 1]);
        if (((Slot04bObj *)p)->field_5c < 0) {
            game_state.spawn_kind = 0;
            spawn_game_state_kind_object(obj);
        }
        set_sequence_by_side_and_mode(obj, 0x45);
    }
}

void func_801b4784_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];
    Object *q;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 2) {
        *(u8 *)&obj->step_flags = 1;
        play_side_sound_at(obj, obj->side, 0x13);
    }
    if (*(u8 *)&obj->step_flags == 0) {
        move_other_by_side_record_velocity_slot04_0e(obj);
        if (p->ground_y < p->pos_y) {
            obj->state_3 = obj->state_3 + 1;
            game_state.state_param = 0x18;
            func_80146960(obj);
            obj->substate_index = 0;
            p->field_261 = 0;
            p->pos_y_frac = 0;
            p->field_15b = 1;
            p->pos_y = p->ground_y;
            set_other_hit_reaction_state_0(obj, 4, 0xa, -0x200, 0, 0, 0);
            if (((Slot04bObj *)p)->field_5c < 0) {
                obj->field_167 = (obj->direction_index >> 1) + 0xc;
            }
            q = r->object;
            if (q != 0) {
                q->variant_index = 0x57;
                q->state_1 = q->state_1 + 1;
            }
        }
    }
}

void update_sequence_or_finish_sequence_4_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void apply_velocity_and_accel_to_position_slot04_0e(Object *o) {
    *(s32 *)&o->pos_x_frac += o->vel_x;
    o->vel_x += o->accel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
}

void apply_facing_velocity_with_x_clamp_slot04_0e(Object *obj) {
    s32 a = obj->vel_x;

    if (a != 0) {
        if (obj->facing_side == 0) {
            a = -a;
        }
        *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
        }
    }
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void move_other_by_side_record_velocity_slot04_0e(Object *obj) {
    Object *p = obj->other;
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];

    *(s32 *)&p->pos_x_frac += r->vel_x;
    r->vel_x += r->accel_x;
    *(s32 *)&p->pos_y_frac -= r->vel_y;
    r->vel_y += r->accel_y;
}
