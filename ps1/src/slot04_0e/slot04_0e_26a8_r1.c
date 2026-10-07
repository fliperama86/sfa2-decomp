/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c5f90_slot04_0e[];
extern ObjectFn data_801c5fc8_slot04_0e[];
extern ObjectFn data_801c6000_slot04_0e[];
extern s32 data_801c601c_slot04_0e[];
extern s32 data_801c6028_slot04_0e[];
extern ObjectFn data_801c6034_slot04_0e[];

Object *func_8011f0e8(void);
void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b2c00_slot04_0e(Object *obj);
void func_801b2ca4_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
Dir func_801b6b64_slot04_0e(Slot04bObj *obj);
void func_801b6d7c_slot04_0e(Slot04bObj *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);

void func_801b26a8_slot04_0e(Object *obj) {
    data_801ad398 = data_801c5f90_slot04_0e[obj->field_15a](obj);
}

int func_801b26f0_slot04_0e(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b26fc_slot04_0e(Object *obj) {
    return 1;
}

int func_801b2704_slot04_0e(Object *obj) {
    return 0;
}

int func_801b270c_slot04_0e(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b2720_slot04_0e(Object *obj) {
    if (obj->field_7e == 0) {
        return obj->field_177 != 0;
    }
    return 1;
}

int func_801b2744_slot04_0e(Object *obj) {
    return 0;
}

void func_801b274c_slot04_0e(Object *obj) {
    data_801c5fc8_slot04_0e[obj->field_15a](obj);
}

void func_801b278c_slot04_0e(Object *obj) {
    data_801c6000_slot04_0e[obj->field_07](obj);
}

void func_801b27cc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    o->field_17b = 1;
    obj->field_1de = 0;
    o->field_50 = 0;
    o->field_58 = 0;
    o->field_07++;
    func_80141f28(o, 3);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    if (o->field_49 == 0) {
        a = 0x1d;
    } else {
        a = 0x58;
    }
    a += o->field_12a >> 1;
    func_801307e0(o, a);
}

void func_801b285c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    if (obj->field_1de == 0) {
        func_801b2ca4_slot04_0e(o);
    }
    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
        obj->field_3a = 0;
        o->field_4c = 0;
        o->field_54 = 0;
        o->field_50 = data_801c601c_slot04_0e[o->field_12a >> 1];
        o->field_58 = data_801c6028_slot04_0e[o->field_12a >> 1];
    }
    func_80130efc(o);
}

void func_801b2910_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    if (obj->field_1de == 0) {
        func_801b2ca4_slot04_0e(o);
    }
    func_801b4914_slot04_0e(o);
    if (o->field_70 < o->pos_y) {
        func_801b2c00_slot04_0e(o);
    } else if (o->pos_y >= (s16)(o->field_70 - 0x18) || obj->field_1de == 0) {
        func_80130efc(o);
    } else {
        a = 0x5b;
        o->field_07++;
        if (o->field_49 == 0) {
            a = 0x52;
        }
        a += o->field_12a >> 1;
        func_801307e0(o, a);
    }
}

void func_801b29e0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s16 y;

    if (obj->field_3a == 1) {
        p = func_8011f0e8();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xe;
            p->field_03 = 0;
            p->field_66 = o->field_66;
            p->field_65 = o->field_65;
            p->field_49 = o->field_49;
            p->field_ac = o->field_48;
            p->field_ad = 0;
            p->field_0e = o->field_0e;
            p->field_0b = o->field_0b;
            p->field_0c = o->field_0c;
            p->field_0d = o->field_0d;
            p->field_26 = o->field_26;
            p->pos_x = o->pos_x;
            y = o->pos_y;
            p->field_5c = 0;
            p->field_3c = o;
            p->pos_y = y;
            o->field_240++;
            o->field_14c = (s32)p;
            p->field_90 = o->field_90;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
        }
        obj->field_3a = 0;
        obj->field_47 = 2;
        o->field_07++;
        func_801204f4(o, o->field_02, 0x16);
        func_801204f4(o, o->field_02, 9);
    }
    func_80130efc(o);
}

void func_801b2b40_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_47 -= 1;
    if (obj->field_47 & 0x80) {
        o->field_58 = -0x8000;
        o->field_38 = 1;
        o->field_07++;
        o->field_50 = 0;
        if (o->field_49 != 0) {
            o->field_58 = 0xfffe0000;
        }
        func_80130efc(o);
    }
}

void func_801b2ba8_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->field_70 >= obj->pos_y) {
        func_80130efc(obj);
    } else {
        func_801b2c00_slot04_0e(obj);
    }
}

void func_801b2c00_slot04_0e(Object *obj) {
    obj->field_07 = 6;
    obj->field_45 = 0;
    obj->field_14 = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801209c4(obj);
    func_801307e0(obj, 0x20);
}

void func_801b2c50_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2ca4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Dir buf;
    int t;

    if (o->field_cd != 0) {
        t = func_801b6bd4_slot04_0e(o);
    } else {
        t = obj->field_134;
    }
    if (t != 0) {
        buf = func_801b6b64_slot04_0e((Slot04bObj *)o);
        o->field_48 = buf.first;
    }
}

void func_801b2d0c_slot04_0e(Object *obj) {
    data_801c6034_slot04_0e[obj->field_07](obj);
}

void func_801b2d4c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u16 a;

    obj->field_1de = 0;
    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 2);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    if (o->field_49 != 0) {
        a = 0x5e;
    } else {
        a = 0x2c;
    }
    func_801307e0(o, a);
    func_801204f4(o, obj->field_a6, 9);
}

void func_801b2dd4_slot04_0e(Object *o) {
    int d;
    int a;

    if ((s16)o->field_3a >= 0) {
        func_80130efc(o);
    } else {
        o->field_45 = 1;
        o->field_58 = -0x4800;
        o->field_07++;
        o->field_50 = 0x68000;
        d = *(s32 *)&o->other->field_10;
        d -= *(s32 *)&o->field_10;
        if (d > 0) {
            d = -d;
        }
        d += 0x1800000;
        if (o->field_0b != 0) {
            d = -d;
        }
        a = 0x5f;
        d >>= 3;
        if (o->field_49 == 0) {
            a = 0x2d;
            d >>= 1;
        }
        o->field_4c = d;
        o->field_54 = 0;
        func_801307e0(o, a);
    }
}
