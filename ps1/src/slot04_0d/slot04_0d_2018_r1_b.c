/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a60_slot04_0d[];
extern ObjectFn data_801c2a74_slot04_0d[];
extern u8 data_801c2804_slot04_0d[];
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);

void func_801b22b8_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2314_slot04_0d(Object *obj) {
    data_801c2a60_slot04_0d[obj->field_07](obj);
}

void func_801b2354_slot04_0d(Object *obj) {
    obj->field_07++;
    *(u8 *)&obj->field_46 = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3e);
}

void func_801b23b4_slot04_0d(Object *obj) {
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

void func_801b2440_slot04_0d(Object *obj) {
    u8 a = 0;

    *(u8 *)&obj->field_46 -= 1;
    if (*(u8 *)&obj->field_46 & 0x80) {
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c2804_slot04_0d[a];
        func_801307e0(obj, 0x35);
    } else {
        func_80130efc(obj);
    }
}

void func_801b24d8_slot04_0d(Object *obj) {
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
            p->field_02 = 0xd;
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

void func_801b2620_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2660_slot04_0d(Object *obj) {
    data_801c2a74_slot04_0d[obj->field_07](obj);
}

void func_801b26a0_slot04_0d(Object *obj) {
    obj->field_249 = 2;
    *(s32 *)&obj->field_4c = 0x80000;
    obj->field_54 = -0x8000;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x38);
}

void func_801b2718_slot04_0d(Object *obj) {
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
