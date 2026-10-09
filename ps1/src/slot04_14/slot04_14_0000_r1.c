/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b8580_slot04_14[];
extern u8 data_801ba928_slot04_14[];
extern u32 data_801c60c8_slot04_14[];
extern ObjectFn data_801c6168_slot04_14[];
extern ObjectFn data_801c6170_slot04_14[];
extern ObjectFn data_801c617c_slot04_14[];
extern ObjectFn data_801c6184_slot04_14[];
extern ObjectFn data_801c6190_slot04_14[];

Object *func_8011f32c(void);
Block172 *func_8011f1e0(void);
void func_80130678(Object *object, int index);
void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80142a14(Object *object);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b0000_slot04_14(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b8580_slot04_14;
    obj->field_9c = data_801ba928_slot04_14;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c60c8_slot04_14[i];
    }
    game_state.field_13d = 0;
    game_state.field_13c = 0;
    obj->field_252 = 0;
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x1a;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->field_28 = (u32)c;
        c->field_66 = s;
        c->field_0d = obj->field_0d;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}

void func_801b0128_slot04_14(Object *obj) {
    data_801c6168_slot04_14[obj->field_06](obj);
}

void func_801b0168_slot04_14(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    func_80130678(obj, 0x30);
}

void func_801b0194_slot04_14(Object *obj) {
    s16 t = obj->field_3a;
    Object *c;
    u8 e;

    if (t < 0) {
        func_801b62a8_slot04_14(obj, 1, 0, 0, 0);
        func_80130678(obj, 0);
    } else {
        if ((t & 0xff00) == 0x100) {
            obj->field_3a = t & 0x80ff;
            game_state.field_63 = 0x18;
            func_801204f4(obj, obj->side, 0xe);
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->field_00 = 1;
                c->field_02 = 0x7d;
                c->field_3c = obj;
                c->field_0c = obj->field_0c;
                c->field_0d = obj->field_0d;
                e = obj->field_0e;
                obj->field_3c = c;
                c->field_02 = 0x12;
                c->field_08 = 0x20;
                c->field_0e = e;
                c->field_66 = obj->field_66;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b028c_slot04_14(Object *obj) {
    data_801c6170_slot04_14[obj->field_128 >> 1](obj);
}

void func_801b02d0_slot04_14(Object *obj) {
    data_801c617c_slot04_14[obj->field_129 >> 1](obj);
}

void func_801b0314_slot04_14(Object *obj) {
    data_801c6184_slot04_14[obj->field_07](obj);
}

void func_801b0354_slot04_14(Object *obj) {
    int one = 1;

    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b62a8_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->field_159 = one;
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_29a = one;
            obj->field_278 = one;
            func_801307e0(obj, 0x27);
            return;
        }
    }
    obj->field_159 = one;
    func_80130dc0(obj);
}

void func_801b0470_slot04_14(Object *obj) {
    func_80142a14(obj);
}

void func_801b0490_slot04_14(Object *obj) {
    s16 t = obj->field_3a;
    int d;

    if (t < 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            d = -0x20000;
            if (obj->field_0b != 0) {
                d = 0x20000;
            }
            *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
        }
        func_80130efc(obj);
    }
}

void func_801b0500_slot04_14(Object *obj) {
    data_801c6190_slot04_14[obj->field_07](obj);
}

void func_801b0540_slot04_14(Object *obj) {
    int one = 1;

    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b62a8_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0) {
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            obj->field_4c = -0x36000;
            if (obj->field_0b != 0) {
                obj->field_4c = 0x36000;
            }
            obj->field_50 = 0x40000;
            obj->field_58 = -0x6000;
            func_80130ec0(obj);
            obj->field_29a = one;
            obj->field_278 = one;
            obj->field_159 = one;
            func_801307e0(obj, 0x40);
            return;
        }
    }
    obj->field_159 = one;
    func_80130dc0(obj);
}
