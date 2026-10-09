/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6afc_slot04_0d[];
extern u8 data_801b9910_slot04_0d[];
extern u32 data_801c2894_slot04_0d[];
extern ObjectFn data_801c2934_slot04_0d[];
extern ObjectFn data_801c2940_slot04_0d[];
extern ObjectFn data_801c294c_slot04_0d[];
extern ObjectFn data_801c2954_slot04_0d[];
extern ObjectFn data_801c2960_slot04_0d[];
extern ObjectFn data_801c296c_slot04_0d[];

Object *func_8011f32c(void);
void func_80130678(Object *object, int arg);
void func_80130dc0(Object *object);
void func_80142a14(Object *object);

void func_801b0308_slot04_0d(Object *obj);
void func_801b03c8_slot04_0d(Object *obj);
void func_801b0408_slot04_0d(Object *obj);
void func_801b0694_slot04_0d(Object *obj);
void func_801b08bc_slot04_0d(Object *obj);

void func_801b0000_slot04_0d(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b6afc_slot04_0d;
    obj->field_9c = data_801b9910_slot04_0d;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c2894_slot04_0d[i];
    }
    if (game_state.field_42 == 0) {
        if (obj->side == 0) {
            obj->pos_x = 0x390;
        } else {
            obj->pos_x = 0x170;
        }
        obj->field_4c = 0x80000;
        obj->field_54 = -0x6000;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0xd;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->field_28 = (u32)c;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_66 = s;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}

void func_801b0140_slot04_0d(Object *obj) {
    data_801c2934_slot04_0d[obj->field_06](obj);
}

void func_801b0180_slot04_0d(Object *obj) {
    int d = obj->field_4c;

    if (obj->field_0b != 0) {
        d = -d;
    }
    *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
    if (obj->pos_x == 0x280) {
        obj->field_06 = obj->field_06 + 1;
        func_80120554(obj, obj->side, 0x324);
        func_80130678(obj, 0x30);
    } else {
        func_80130efc(obj);
    }
}

void func_801b020c_slot04_0d(Object *obj) {
    int a = obj->field_4c;

    if (obj->field_0b != 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if ((s32)obj->field_4c < 0) {
        obj->field_06 = obj->field_06 + 1;
        obj->field_10 = 0;
        obj->field_4c = 0;
        obj->field_54 = 0;
        if (obj->side == 0) {
            obj->pos_x = 0x220;
        } else {
            obj->pos_x = 0x2e0;
        }
        func_80130678(obj, 0x31);
    } else {
        func_80130efc(obj);
        func_801b0308_slot04_0d(obj);
    }
}

void func_801b02b8_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0308_slot04_0d(Object *obj) {
    u32 i;
    s8 r;

    if ((u8)obj->field_3a == 0) {
        i = game_state.field_1d;
        if (((i + obj->side) & 3) == 0) {
            i = 0;
            do {
                r = func_80148e84(obj);
                if (r == 0) {
                    break;
                }
                i++;
            } while ((u8)i < 2);
        }
    }
}

void func_801b0388_slot04_0d(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b08bc_slot04_0d(obj);
    } else {
        func_801b03c8_slot04_0d(obj);
    }
}

void func_801b03c8_slot04_0d(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b0694_slot04_0d(obj);
    } else {
        func_801b0408_slot04_0d(obj);
    }
}

void func_801b0408_slot04_0d(Object *obj) {
    data_801c2940_slot04_0d[obj->field_12a >> 1](obj);
}

void func_801b044c_slot04_0d(Object *obj) {
    data_801c294c_slot04_0d[obj->field_07](obj);
}

void func_801b048c_slot04_0d(Object *obj) {
    data_801c2954_slot04_0d[obj->field_07](obj);
}

void func_801b04cc_slot04_0d(Object *obj) {
    obj->field_07++;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b0554_slot04_0d(Object *obj) {
    func_80142a14(obj);
}

void func_801b0574_slot04_0d(Object *obj) {
    int one;

    obj->field_07 = obj->field_07 + 1;
    if ((obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        obj->field_278 = one;
    }
}

void func_801b05fc_slot04_0d(Object *obj) {
    int d = 0x18;

    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    func_80130efc(obj);
}

void func_801b0654_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b0694_slot04_0d(Object *obj) {
    data_801c2960_slot04_0d[obj->field_12a >> 1](obj);
}

void func_801b06d8_slot04_0d(Object *obj) {
    data_801c296c_slot04_0d[obj->field_07](obj);
}

void func_801b0718_slot04_0d(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x11, 0x11) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        } else if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->field_159 = 1;
            obj->field_07 = 2;
            obj->field_157 = 0;
            func_80130ec0(obj);
            obj->field_278 = 1;
            obj->field_29a = 1;
            func_801307e0(obj, 0x1f);
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
