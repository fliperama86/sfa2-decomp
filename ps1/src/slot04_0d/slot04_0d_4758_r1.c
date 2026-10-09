/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3648_slot04_0d[];
extern ObjectFn data_801c3654_slot04_0d[];
extern ObjectFn data_801c36dc_slot04_0d[];
extern ObjectFn data_801c36ec_slot04_0d[];
extern SequenceStep *data_801c0a60_slot04_0d[];

void func_80131468(Object *object);
void func_8011ffdc(Object *o);
void func_8011f240(Slab172 *s);
void func_801b4b68_slot04_0d(Object *obj);
void func_80130dc0(Object *obj);

void func_801b4758_slot04_0d(Object *obj) {
    u8 one;

    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    } else {
        one = 1;
        obj->field_159 = one;
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80130ec0(obj);
            ((Slot04bObj *)obj)->field_278 = one;
            func_801307e0(obj, 0x1f);
        } else {
            func_80130dc0(obj);
        }
    }
}

void func_801b4834_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4898_slot04_0d(Object *obj) {
    s16 t = obj->field_3a;
    s16 d;

    if (t >= 0) {
        if ((u8)t != 0) {
            d = -2;
            if (obj->field_0b != 0) {
                d = 2;
            }
            *(s32 *)&obj->field_10 += d;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b490c_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    data_801c3648_slot04_0d[obj->field_12a >> 1](obj);
}

void func_801b4954_slot04_0d(Object *obj) {
    data_801c3654_slot04_0d[obj->field_07](obj);
}

void func_801b4994_slot04_0d(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b49cc_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b4a30_slot04_0d(Object *obj) {
    u16 t;
    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    t = 0x12;
    if (obj->field_48 != 0) {
        t = 0xc;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    t = (obj->field_12a >> 1) + t;
    func_801307e0(obj, t);
    func_80120af8(obj);
}

void func_801b4ab8_slot04_0d(Object *obj) {
    data_801c36dc_slot04_0d[obj->field_04](obj);
}

void func_801b4af8_slot04_0d(Object *obj) {
    obj->field_50 = 0xfffe0000;
    obj->field_58 = 0x1000;
    obj->field_04 = obj->field_04 + 1;
    obj->field_4c = (*(s32 *)&obj->field_3c->field_10 - *(s32 *)&obj->field_10) >> 6;
    func_80130768(obj, 0, data_801c0a60_slot04_0d);
    func_801b4b68_slot04_0d(obj);
}

void func_801b4b68_slot04_0d(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04 = 2;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        data_801c36ec_slot04_0d[obj->field_05](obj);
        func_8011ffdc(obj);
    }
}

void func_801b4be0_slot04_0d(Object *obj) {
    s16 t;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    t = (u16)obj->field_3c->field_70;
    t -= 0x55;
    if (obj->pos_y < t) {
        func_80131094(obj);
    } else {
        obj->field_05 = obj->field_05 + 1;
        obj->field_46 = (obj->field_46 & 0xff00) | 4;
    }
}

void func_801b4c7c_slot04_0d(Object *obj) {
    u16 t = obj->field_46;
    t -= 1;
    obj->field_46 = t;
    if ((u8)t == 0) {
        obj->field_50 = 0xffff0000;
        obj->field_58 = 0x2000;
        obj->field_05 = obj->field_05 + 1;
    }
}

void func_801b4cb8_slot04_0d(Object *obj) {
    Object *p;
    s16 t;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    p = obj->field_3c;
    t = p->field_70;
    if (t > obj->pos_y) {
        func_80131094(obj);
    } else {
        obj->field_05 = obj->field_05 + 1;
        obj->pos_y = (u16)p->field_70;
        func_80130768(obj, 1, data_801c0a60_slot04_0d);
    }
}

void func_801b4d58_slot04_0d(Object *obj) {
    func_80131094(obj);
}

void func_801b4d78_slot04_0d(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}

void func_801b4d8c_slot04_0d(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
