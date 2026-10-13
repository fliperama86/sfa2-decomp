/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c6084_slot04_0e[];
extern ObjectFn data_801c6088_slot04_0e[];
extern s32 data_801c609c_slot04_0e[];
extern u8 data_801c60a8_slot04_0e[];
extern u8 data_801c60ac_slot04_0e[];
extern SequenceStep data_801c0a00_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

void update_meter_by_mode_or_table(Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_children_per_direction_index(Object *object, int a, int b);

void spawn_child_object_from_pool_slot04_0e(Object *obj);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);
s16 apply_forced_state_if_other_in_reach_or_block(Object *obj, int a, int b);
void spawn_effect_at_other_then_next_spawn_slot04_0e(Object *obj);
void spawn_ground_child_and_set_sequence_slot04_0e(Object *obj);
void spawn_ground_child_every_eighth_frame_slot04_0e(Object *obj);
void spawn_object_in_front_store_in_record_slot04_0e(Object *obj);

void update_dash_sequence_set_velocity_x_slot04_0e(Object *obj) {
    u8 t;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->vel_x = 0x50000;
        obj->state_3++;
        obj->vel_y = 0;
        obj->accel_y = 0;
        obj->accel_x = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        if (obj->facing_side != 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        ((Slot04bObj *)obj)->countdown = 0;
        t = (obj->direction_index >> 1) + 1;
        ((Slot04bObj *)obj)->field_47 = t;
        spawn_child_object_from_pool_slot04_0e(obj);
        t = 3;
        if (obj->field_4b == 0) {
            t = obj->direction_index >> 1;
        }
        obj->other_lock = data_801c6084_slot04_0e[t];
        set_sequence_by_side_and_mode(obj, 0x34);
    } else if ((u8)obj->step_flags != 2) {
        ((Slot04bObj *)obj)->mode = 2;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x25, 0x3a);
        play_side_sound_at(obj, obj->side, 8);
    }
}

void move_and_spawn_children_until_counter_expires_slot04_0e(Object *obj) {
    apply_velocity_and_accel_to_position_slot04_0e(obj);
    if ((u8)obj->step_flags == 1) {
        if (((Slot04bObj *)obj)->countdown == 2) {
            play_side_sound_at(obj, obj->side, 7);
        }
        ((Slot04bObj *)obj)->field_47 -= 1;
        if (((Slot04bObj *)obj)->field_47 & 0x80) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x35);
            return;
        }
        ((Slot04bObj *)obj)->mode = 0;
        spawn_child_object_from_pool_slot04_0e(obj);
    }
    update_sequence(obj);
}

void update_sequence_or_finish_sequence_3_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void spawn_child_object_from_pool_slot04_0e(Object *obj) {
    Object *p;

    p = pop_object_from_pool(obj);
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x18;
        p->table_index = ((Slot04bObj *)obj)->countdown;
        p->sound_side = obj->sound_side;
        p->team_id = obj->team_id;
        p->field_49 = obj->field_49;
        p->field_4b = obj->field_4b;
        p->vel_table_index = obj->direction_index;
        p->active_flag_2 = 1;
        p->field_0e = obj->field_0e;
        p->facing_side = obj->facing_side;
        p->field_0c = obj->field_0c;
        p->image_row = obj->image_row;
        p->scroll_factor = obj->scroll_factor;
        p->pos_x = obj->pos_x;
        p->pos_y = obj->pos_y;
        p->parent = obj;
        obj->linked_object_3 = (s32)p;
        ((Slot04bObj *)obj)->countdown++;
        p->field_90 = obj->field_90;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
    }
}

void call_table_function_by_state_3_12_slot04_0e(Object *obj) {
    data_801c6088_slot04_0e[obj->state_3](obj);
}

void clear_side_record_start_seq_sixty_seven_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r;

    obj->state_3++;
    r = &data_801c62b4_slot04_0e[obj->side];
    r->offset_x = 0;
    r->object = 0;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (((Slot04bObj *)obj)->field_1da == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    set_sequence_by_side_and_mode(obj, 0x43);
}

void update_sequence_set_table_velocity_x_slot04_0e(Object *obj) {
    u8 t;
    int a;
    int v;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        ((Slot04bObj *)obj)->mode = 0;
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        a = obj->direction_index >> 1;
        v = data_801c609c_slot04_0e[a];
        obj->vel_x = v;
        if (obj->facing_side == 0) {
            obj->vel_x = -v;
        }
        set_sequence_by_side_and_mode(obj, a + 0x46);
        if (obj->field_4b == 0) {
            t = obj->direction_index >> 1;
        } else {
            t = 3;
        }
        obj->other_lock = data_801c60a8_slot04_0e[t];
        play_side_sound_at(obj, obj->side, 0x11);
    } else if ((u8)obj->step_flags != 2) {
        ((Slot04bObj *)obj)->mode = 2;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x1d, 5);
        play_side_sound_at(obj, obj->side, 5);
    }
}

void update_record_offset_and_check_hit_slot04_0e(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    Slot04_0eRec62b4 *r;
    Object *o;
    SequenceStep *sq;
    int pos;
    s16 d;
    int t;
    int u;
    int w;
    s16 res;

    r = &data_801c62b4_slot04_0e[obj->side];
    if ((u8)obj->step_flags == 0x80) {
        update_sequence(obj);
        return;
    }
    r->offset_x += obj->vel_x;
    spawn_ground_child_every_eighth_frame_slot04_0e(obj);
    if ((s16)obj->step_flags < 0) {
        spawn_ground_child_and_set_sequence_slot04_0e(obj);
        return;
    }
    pos = r->offset_x;
    u = (u16)obj->pos_x + (pos >> 16);
    pos &= 0xffff;
    u <<= 16;
    pos |= u;
    d = (pos >> 16) - *(u16 *)(main_layer + 0x12);
    if (d >= 0x181) {
        spawn_ground_child_and_set_sequence_slot04_0e(obj);
        return;
    }
    pos = r->offset_x;
    w = pos >> 16;
    if (w >= 0) {
        w = -w;
        pos &= 0xffff;
        w <<= 16;
        pos |= w;
    }
    res = apply_forced_state_if_other_in_reach_or_block(obj, pos >> 16, *(s16 *)(data_801c60ac_slot04_0e + (obj->direction_index & 0xfe)));
    if (res == 0) {
        update_sequence(obj);
        return;
    }
    if (res < 0) {
        spawn_effect_at_other_then_next_spawn_slot04_0e(obj);
        return;
    }
    obj->other_lock = 0xff;
    obj->state_3++;
    obj->substate_index = 0;
    o = obj->other;
    pos = (u16)o->pos_x << 16;
    o->field_261 = 1;
    t = (pos >> 16) - (u16)obj->pos_x;
    pos &= 0xffff;
    t <<= 16;
    pos |= t;
    t = pos >> 16;
    if (t < 0) {
        t = -t;
        pos &= 0xffff;
        pos |= t << 16;
        t = pos >> 16;
    }
    t = (s16)(t - 0x10) >> 3;
    pos &= 0xffff;
    pos |= t << 16;
    if ((s16)t >= 0x2d) {
        pos &= 0xffff;
        pos |= 0x2c0000;
    }
    sq = data_801c0a00_slot04_0e;
    obj->sequence = sq + (pos >> 16);
    obj->step_timer = obj->sequence->duration;
    obj->step_flags = obj->sequence->flags;
    obj->frame = obj->frames + obj->sequence->frame_index;
    obj->frame_sound_index = 0;
    obj->sequence_changed_flag = 1;
    spawn_object_in_front_store_in_record_slot04_0e(obj);
}
