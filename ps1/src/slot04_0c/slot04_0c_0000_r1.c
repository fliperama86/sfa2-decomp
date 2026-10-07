/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4824_slot04_0c[];
extern u8 data_801b5bf4_slot04_0c[];
extern u32 data_801bd828_slot04_0c[];
extern u16 data_801bd8c8_slot04_0c[];
extern ObjectFn data_801bd8cc_slot04_0c[];
extern s16 data_801bd8d8_slot04_0c[];
extern ObjectFn data_801bd8dc_slot04_0c[];
extern ObjectFn data_801bd8e4_slot04_0c[];
extern ObjectFn data_801bd8f0_slot04_0c[];

Object *func_8011f32c(void);
void func_80130678(Object *object, u16 arg);
void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80142a14(Object *object);

void func_801b01ac_slot04_0c(Object *obj);
void func_801b0300_slot04_0c(Object *obj);
void func_801b0340_slot04_0c(Object *obj);
void func_801b0430_slot04_0c(Object *obj);
void func_801b0590_slot04_0c(Object *obj);

void func_801b0000_slot04_0c(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    if (game_state.field_42 == 0) {
        obj->pos_x = data_801bd8c8_slot04_0c[obj->side];
    }
    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b4824_slot04_0c;
    obj->field_9c = data_801b5bf4_slot04_0c;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801bd828_slot04_0c[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0xc;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        obj->field_28 = (u32)c;
        c->field_66 = obj->side;
        c->field_0d = obj->field_0d;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}

void func_801b013c_slot04_0c(Object *obj) {
    data_801bd8cc_slot04_0c[obj->field_06](obj);
}

void func_801b017c_slot04_0c(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_4c = 0x80000;
    func_801b01ac_slot04_0c(obj);
}

void func_801b01ac_slot04_0c(Object *obj) {
    int a = 0x32;

    if (obj->field_0b == 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    }
    if (obj->pos_x != data_801bd8d8_slot04_0c[obj->side]) {
        func_80130efc(obj);
    } else {
        obj->field_46 = 0x48;
        obj->field_06 = obj->field_06 + 1;
        if (obj->other->kind == 0xb) {
            a = 0x33;
        }
        func_80130678(obj, a);
    }
}

void func_801b0260_slot04_0c(Object *obj) {
    s16 t;

    func_80130efc(obj);
    t = obj->field_46;
    t -= 1;
    obj->field_46 = t;
    if (t == 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    }
}

void func_801b02c0_slot04_0c(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b0300_slot04_0c(obj);
    } else {
        func_801b0590_slot04_0c(obj);
    }
}

void func_801b0300_slot04_0c(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b0340_slot04_0c(obj);
    } else {
        func_801b0430_slot04_0c(obj);
    }
}

void func_801b0340_slot04_0c(Object *obj) {
    data_801bd8dc_slot04_0c[obj->field_07](obj);
}

void func_801b0380_slot04_0c(Object *obj) {
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        obj->field_159 = 1;
        obj->field_07 = obj->field_07 + 1;
        func_80130dc0(obj);
    }
}

void func_801b0410_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0430_slot04_0c(Object *obj) {
    u16 t;

    data_801bd8e4_slot04_0c[obj->field_07](obj);
    t = obj->field_3a;
    if ((t & 0x7f00) != 0) {
        obj->field_3a = t & 0x80ff;
        func_80120554(obj, obj->side, 0x31f);
    }
}

void func_801b04a4_slot04_0c(Object *obj) {
    int one = 1;

    obj->field_159 = one;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a == 4) {
        obj->field_07 = 2;
        obj->field_278 = one;
    }
    func_80130dc0(obj);
}

void func_801b04f0_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0510_slot04_0c(Object *obj) {
    s16 t = obj->field_3a;
    int d = 0x80000;

    if ((t & 0x8000) != 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            obj->field_3a = t & 0xff00;
            if (obj->field_0b == 0) {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - d;
            } else {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + d;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b0590_slot04_0c(Object *obj) {
    obj->field_157 = 1;
    data_801bd8f0_slot04_0c[obj->field_07](obj);
}

void func_801b05d4_slot04_0c(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b0604_slot04_0c(Object *obj) {
    func_80142a14(obj);
}

void func_801b0624_slot04_0c(Object *obj) {
    int x = 0xc;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        x = 0x12;
    }
    if (obj->field_129 != 0) {
        x += 3;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + x));
}
