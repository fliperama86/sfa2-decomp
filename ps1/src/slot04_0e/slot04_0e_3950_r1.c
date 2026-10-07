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

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        obj->field_4c = 0x50000;
        obj->field_07++;
        obj->field_50 = 0;
        obj->field_58 = 0;
        obj->field_54 = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        ((Slot04bObj *)obj)->field_46 = 0;
        t = (obj->field_12a >> 1) + 1;
        ((Slot04bObj *)obj)->field_47 = t;
        func_801b3b80_slot04_0e(obj);
        t = 3;
        if (obj->field_4b == 0) {
            t = obj->field_12a >> 1;
        }
        obj->field_27b = data_801c6084_slot04_0e[t];
        func_801307e0(obj, 0x34);
    } else if ((u8)obj->field_3a != 2) {
        ((Slot04bObj *)obj)->field_3a = 2;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x25, 0x3a);
        func_801204f4(obj, obj->side, 8);
    }
}

void func_801b3aa0_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if ((u8)obj->field_3a == 1) {
        if (((Slot04bObj *)obj)->field_46 == 2) {
            func_801204f4(obj, obj->side, 7);
        }
        ((Slot04bObj *)obj)->field_47 -= 1;
        if (((Slot04bObj *)obj)->field_47 & 0x80) {
            obj->field_07++;
            func_801307e0(obj, 0x35);
            return;
        }
        ((Slot04bObj *)obj)->field_3a = 0;
        func_801b3b80_slot04_0e(obj);
    }
    func_80130efc(obj);
}

void func_801b3b40_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b3b80_slot04_0e(Object *obj) {
    Object *p;

    p = func_8011f0e8(obj);
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x18;
        p->field_03 = ((Slot04bObj *)obj)->field_46;
        p->field_66 = obj->field_66;
        p->field_65 = obj->field_65;
        p->field_49 = obj->field_49;
        p->field_4b = obj->field_4b;
        p->field_ac = obj->field_12a;
        p->field_ad = 1;
        p->field_0e = obj->field_0e;
        p->field_0b = obj->field_0b;
        p->field_0c = obj->field_0c;
        p->field_0d = obj->field_0d;
        p->field_26 = obj->field_26;
        p->pos_x = obj->pos_x;
        p->pos_y = obj->pos_y;
        p->field_3c = obj;
        obj->field_14c = (s32)p;
        ((Slot04bObj *)obj)->field_46++;
        p->field_90 = obj->field_90;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
    }
}

void func_801b3c9c_slot04_0e(Object *obj) {
    data_801c6088_slot04_0e[obj->field_07](obj);
}

void func_801b3cdc_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r;

    obj->field_07++;
    r = &data_801c62b4_slot04_0e[obj->side];
    r->field_10 = 0;
    r->field_14 = 0;
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

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        ((Slot04bObj *)obj)->field_3a = 0;
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        a = obj->field_12a >> 1;
        v = data_801c609c_slot04_0e[a];
        obj->field_4c = v;
        if (obj->field_0b == 0) {
            obj->field_4c = -v;
        }
        func_801307e0(obj, a + 0x46);
        if (obj->field_4b == 0) {
            t = obj->field_12a >> 1;
        } else {
            t = 3;
        }
        obj->field_27b = data_801c60a8_slot04_0e[t];
        func_801204f4(obj, obj->side, 0x11);
    } else if ((u8)obj->field_3a != 2) {
        ((Slot04bObj *)obj)->field_3a = 2;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1d, 5);
        func_801204f4(obj, obj->side, 5);
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
    if ((u8)obj->field_3a == 0x80) {
        func_80130efc(obj);
        return;
    }
    r->field_10 += obj->field_4c;
    func_801b4a3c_slot04_0e(obj);
    if ((s16)obj->field_3a < 0) {
        func_801b420c_slot04_0e(obj);
        return;
    }
    pos = r->field_10;
    u = (u16)obj->pos_x + (pos >> 16);
    pos &= 0xffff;
    u <<= 16;
    pos |= u;
    d = (pos >> 16) - *(u16 *)(data_801aa5d4 + 0x12);
    if (d >= 0x181) {
        func_801b420c_slot04_0e(obj);
        return;
    }
    pos = r->field_10;
    w = pos >> 16;
    if (w >= 0) {
        w = -w;
        pos &= 0xffff;
        w <<= 16;
        pos |= w;
    }
    res = func_8013fc18(obj, pos >> 16, *(s16 *)(data_801c60ac_slot04_0e + (obj->field_12a & 0xfe)));
    if (res == 0) {
        func_80130efc(obj);
        return;
    }
    if (res < 0) {
        func_801b4100_slot04_0e(obj);
        return;
    }
    obj->field_27b = 0xff;
    obj->field_07++;
    obj->field_12c = 0;
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
    obj->field_38 = obj->sequence->duration;
    obj->field_3a = obj->sequence->flags;
    obj->frame = obj->frames + obj->sequence->frame_index;
    obj->field_4a = 0;
    obj->field_80 = 1;
    func_801b4b40_slot04_0e(obj);
}
