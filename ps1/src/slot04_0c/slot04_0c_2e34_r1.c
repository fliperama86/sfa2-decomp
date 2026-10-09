/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bdab4_slot04_0c[];
extern ObjectFn data_801bdb0c_slot04_0c[];
extern ObjectFn data_801bdb18_slot04_0c[];
extern u8 data_801bdafc_slot04_0c[];
extern s8 data_801bdb04_slot04_0c[];
extern s16 data_801bd7fc_slot04_0c[];

void func_80138ae8(GameState *state, Object *object);
void func_80131468(Object *object);
void func_801483a4(Object *object, int a, int b);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80130678(Object *object, int arg);
void func_801b337c_slot04_0c(Object *obj);

void func_801b2e34_slot04_0c(Object *obj) {
    data_801bdab4_slot04_0c[obj->field_07](obj);
}

void func_801b2e74_slot04_0c(Object *obj) {
    obj->field_17b = 1;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, -0x30);
    func_80138ae8(&game_state, obj);
    obj->field_159 = 0;
    obj->field_157 = 1;
    obj->field_255 = 0;
    obj->field_12a = 0;
    func_801307e0(obj, 0x1b);
}

void func_801b2ee8_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x32;
        obj->field_07 = obj->field_07 + 1;
        func_801483a4(obj, -0x25, 0x41);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}

void func_801b2f58_slot04_0c(Object *obj) {
    func_80130efc(obj);
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_46 = 1;
        obj->field_165 = 0;
        obj->field_07 = obj->field_07 + 1;
    }
}

void func_801b2fb0_slot04_0c(Object *obj) {
    int v = obj->field_46;
    int w = (v & 0xff00) | ((v & 0xff) - 1);
    obj->field_46 = w;
    if ((w & 0xff) == 0) {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_157 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -0x30000;
        } else {
            obj->field_4c = 0x30000;
        }
        func_801307e0(obj, 0x3f);
    }
    func_80130efc(obj);
}

void func_801b303c_slot04_0c(Object *obj) {
    Config *c;
    s16 x;
    u8 t;
    int idx;

    if ((s16)obj->field_3a & 0x8000) {
        c = game_state.config;
        if ((c->field_4d | c->field_4e | c->field_04) != 0) {
            func_80131468(obj);
        } else {
            obj->field_45 = 0;
            obj->field_157 = 0;
            x = obj->field_46;
            t = obj->field_07 + 1;
            idx = (x & 0xff00) >> 8;
            obj->field_07 = t;
            obj->field_0b = obj->field_158;
            obj->field_46 = (x & -0x100) | (s8)data_801bdafc_slot04_0c[idx];
            idx = data_801bdb04_slot04_0c[idx];
            func_801204f4(obj, obj->side, data_801bd7fc_slot04_0c[idx]);
            obj->field_46 = obj->field_46 + 0x100;
            func_801307e0(obj, 0x3c);
        }
    } else {
        func_801b337c_slot04_0c(obj);
    }
}

void func_801b3154_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_46 = 0x30;
        obj->field_45 = 0;
        obj->field_157 = 1;
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x3d);
    } else {
        func_801b337c_slot04_0c(obj);
    }
}

void func_801b31b4_slot04_0c(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_157 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -0x1c000;
        } else {
            obj->field_4c = 0x1c000;
        }
        obj->field_50 = 0x80000;
        obj->field_54 = 0;
        obj->field_58 = -0x5000;
    }
}

void func_801b3234_slot04_0c(Object *obj) {
    Config *c;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->pos_y < obj->field_70) {
        if (*(u8 *)&obj->field_3a == 0) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        c = game_state.config;
        if ((c->field_4d | c->field_4e | c->field_04) != 0) {
            func_801312b8(obj);
        } else {
            obj->field_46 = 0x70;
            obj->field_0b = obj->field_158;
            obj->field_29c = 2;
            func_801307e0(obj, 0x3e);
        }
    }
}

void func_801b3330_slot04_0c(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b337c_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        *(s32 *)&obj->field_10 += obj->field_4c;
    }
    func_80130efc(obj);
}

void func_801b33c0_slot04_0c(Object *obj) {
    data_801bdb0c_slot04_0c[obj->field_07](obj);
}

void func_801b3400_slot04_0c(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd == 0) {
        if (obj->field_c2 & 0x8000) {
            obj->field_0b = 1;
        }
    }
    func_801307e0(obj, 0x18);
}

void func_801b3480_slot04_0c(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b34e4_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3534_slot04_0c(Object *obj) {
    data_801bdb18_slot04_0c[obj->field_06](obj);
}

void func_801b3574_slot04_0c(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b35a8_slot04_0c(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}
