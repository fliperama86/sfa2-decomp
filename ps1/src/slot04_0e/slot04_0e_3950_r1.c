/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6084_slot04_0e[];
extern ObjectFn data_801c6088_slot04_0e[];
extern s32 data_801c609c_slot04_0e[];
extern u8 data_801c60a8_slot04_0e[];
extern u8 data_801c60ac_slot04_0e[];
extern SequenceStep data_801c0a00_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801483a4(Object *object, int a, int b);

void func_801b3b80_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
s16 func_8013fc18(Object *obj, int a, int b);
void func_801b4100_slot04_0e(Object *obj);
void func_801b420c_slot04_0e(Object *obj);
void func_801b4a3c_slot04_0e(Object *obj);
void func_801b4b40_slot04_0e(Object *obj);

void func_801b3950_slot04_0e(Object *obj) {
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
        func_801b3b80_slot04_0e(obj);
        t = 3;
        if (obj->field_4b == 0) {
            t = obj->direction_index >> 1;
        }
        obj->other_lock = data_801c6084_slot04_0e[t];
        func_801307e0(obj, 0x34);
    } else if ((u8)obj->step_flags != 2) {
        ((Slot04bObj *)obj)->mode = 2;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x25, 0x3a);
        play_side_sound_at(obj, obj->side, 8);
    }
}

void func_801b3aa0_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if ((u8)obj->step_flags == 1) {
        if (((Slot04bObj *)obj)->countdown == 2) {
            play_side_sound_at(obj, obj->side, 7);
        }
        ((Slot04bObj *)obj)->field_47 -= 1;
        if (((Slot04bObj *)obj)->field_47 & 0x80) {
            obj->state_3++;
            func_801307e0(obj, 0x35);
            return;
        }
        ((Slot04bObj *)obj)->mode = 0;
        func_801b3b80_slot04_0e(obj);
    }
    update_sequence(obj);
}

void func_801b3b40_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b3b80_slot04_0e(Object *obj) {
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

void func_801b3c9c_slot04_0e(Object *obj) {
    data_801c6088_slot04_0e[obj->state_3](obj);
}

void func_801b3cdc_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r;

    obj->state_3++;
    r = &data_801c62b4_slot04_0e[obj->side];
    r->offset_x = 0;
    r->object = 0;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (((Slot04bObj *)obj)->field_1da == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, 0x43);
}

void func_801b3d68_slot04_0e(Object *obj) {
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
        func_801307e0(obj, a + 0x46);
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
        func_801483a4(obj, -0x1d, 5);
        play_side_sound_at(obj, obj->side, 5);
    }
}

void func_801b3eb0_slot04_0e(Object *obj) {
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
    func_801b4a3c_slot04_0e(obj);
    if ((s16)obj->step_flags < 0) {
        func_801b420c_slot04_0e(obj);
        return;
    }
    pos = r->offset_x;
    u = (u16)obj->pos_x + (pos >> 16);
    pos &= 0xffff;
    u <<= 16;
    pos |= u;
    d = (pos >> 16) - *(u16 *)(data_801aa5d4 + 0x12);
    if (d >= 0x181) {
        func_801b420c_slot04_0e(obj);
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
    res = func_8013fc18(obj, pos >> 16, *(s16 *)(data_801c60ac_slot04_0e + (obj->direction_index & 0xfe)));
    if (res == 0) {
        update_sequence(obj);
        return;
    }
    if (res < 0) {
        func_801b4100_slot04_0e(obj);
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
    func_801b4b40_slot04_0e(obj);
}
