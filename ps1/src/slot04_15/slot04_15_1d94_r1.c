/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c30d4_slot04_15[];
extern ObjectFn data_801c30e4_slot04_15[];
extern ObjectFn data_801c30f8_slot04_15[];
extern s32 data_801c2df8_slot04_15[];
extern s32 data_801c2e58_slot04_15[];
extern u8 data_801c2e88_slot04_15[];
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);

void func_801b1d94_slot04_15(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        obj->field_4c = 0x60000;
    } else {
        obj->field_4c = -0x60000;
    }
    a = 0x23;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b1e28_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 *t;
    s32 *p;
    u8 g;

    t = data_801c2df8_slot04_15;
    func_80130efc(o);
    g = obj->field_3a;
    if (g & 0x80) {
        *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    } else if (g != 0) {
        o->field_45 = 1;
        o->field_3a = o->field_3a & 0xff00;
        o->field_07++;
        p = t + o->field_12a * 2;
        o->field_4c = p[0];
        o->field_50 = p[1];
        o->field_54 = p[2];
        o->field_58 = p[3];
        o->field_50 = -o->field_50;
        o->field_58 = -o->field_58;
    }
}

void func_801b2020_slot04_15(Object *obj);

void func_801b1f00_slot04_15(Object *obj) {
    int d;

    func_801b2020_slot04_15(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x26);
    } else {
        if (obj->field_0b != 0) {
            d = obj->field_4c;
        } else {
            d = -obj->field_4c;
        }
        *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
        func_80130efc(obj);
    }
}

void func_801b1fc4_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2020_slot04_15(Object *obj) {
    s16 t = ((s16 *)&obj->field_50)[1];

    if (t > 0) {
        obj->field_58 = 0x5000;
    }
    if (obj->field_49 != 0 && t > 0) {
        obj->field_58 = 0x10000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b2078_slot04_15(Object *obj) {
    data_801c30d4_slot04_15[obj->field_07](obj);
}

void func_801b20b8_slot04_15(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->field_157 = 0;
    a = 0x43;
    if (obj->field_49 == 0) {
        a = 0x31;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b2134_slot04_15(Object *obj) {
    s32 *t = data_801c2e58_slot04_15;
    s32 *p;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07++;
        p = t + obj->field_12a * 2;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = p[0];
        obj->field_50 = p[1];
        obj->field_54 = p[2];
        obj->field_58 = p[3];
        obj->field_50 = -obj->field_50;
        obj->field_58 = -obj->field_58;
    }
}

void func_801b21e8_slot04_15(Object *obj) {
    s32 a;

    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 <= obj->pos_y) {
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->field_07++;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x34);
    } else {
        if (obj->field_0b != 0) {
            a = obj->field_4c;
        } else {
            a = -obj->field_4c;
        }
        *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
        func_80130efc(obj);
    }
}

void func_801b22c0_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b231c_slot04_15(Object *obj) {
    data_801c30e4_slot04_15[obj->field_07](obj);
}

void func_801b235c_slot04_15(Object *obj) {
    obj->field_07++;
    *(u8 *)&obj->field_46 = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3e);
}

void func_801b23bc_slot04_15(Object *obj) {
    int a;

    (*(u8 *)&obj->field_46)--;
    if (*(u8 *)&obj->field_3a != 0) {
        a = -1;
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x11, 0x40);
    }
    func_80130efc(obj);
}

void func_801b2448_slot04_15(Object *obj) {
    u8 a = 0;

    *(u8 *)&obj->field_46 -= 1;
    if (*(u8 *)&obj->field_46 & 0x80) {
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c2e88_slot04_15[a];
        func_801307e0(obj, 0x35);
    } else {
        func_80130efc(obj);
    }
}

void func_801b24e0_slot04_15(Object *obj) {
    Object *p;
    s8 t;
    s32 w;
    int one;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            one = 1;
            p->field_00 = one;
            p->field_02 = 0x1f;
            p->field_03 = 2;
            p->field_66 = obj->field_66;
            p->field_4b = obj->field_4b;
            p->field_65 = obj->field_65;
            t = obj->field_12a;
            p->field_ad = one;
            p->field_ac = t + 6;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            w = obj->field_9c;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_9c = w;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->field_5c = (t >> 1) + 2;
            p->field_3c = obj;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0x14);
        }
    }
}

void func_801b2628_slot04_15(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2668_slot04_15(Object *obj) {
    data_801c30f8_slot04_15[obj->field_07](obj);
}

void func_801b26a8_slot04_15(Object *obj) {
    obj->field_249 = 2;
    *(s32 *)&obj->field_4c = 0x80000;
    obj->field_54 = -0x8000;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x38);
}

void func_801b2720_slot04_15(Object *obj) {
    int a;

    obj->field_249 = 2;
    if (*(u8 *)&obj->field_3a != 0) {
        a = 1;
        obj->field_07++;
        if (obj->field_4b == 0) {
            a = -1;
        }
        obj->field_165 = a;
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0xf, 0x44);
    }
    func_80130efc(obj);
}
