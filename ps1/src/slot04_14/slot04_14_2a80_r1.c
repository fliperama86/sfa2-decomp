/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6324_slot04_14[];
extern ObjectFn data_801c6334_slot04_14[];
extern s32 data_801c6350_slot04_14[];
extern s32 data_801c6354_slot04_14[];
extern u8 data_801c6368_slot04_14[];
extern ObjectFn data_801c636c_slot04_14[];
int func_80130184(Object *object);
void func_80130678(Object *object, int index);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_80146998(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801482e0(Object *object, int a, int b);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b2b4c_slot04_14(Object *object);

void func_801b2a80_slot04_14(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_159 = 0;
            obj->field_07++;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_801b2b4c_slot04_14(obj);
    }
}

void func_801b2b00_slot04_14(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2b4c_slot04_14(Object *obj) {
    obj->field_159 = 0;
    obj->field_45 = 0;
    obj->pos_y = obj->field_70;
    func_801b62a8_slot04_14(obj, 1, 0, 3, 2);
    func_80130678(obj, 0x11);
}

void func_801b2ba4_slot04_14(Object *obj) {
    data_801c6324_slot04_14[obj->field_07](obj);
}

void func_801b2be4_slot04_14(Object *obj) {
    obj->field_07++;
    obj->field_46 = *(u8 *)&obj->field_46 | 0x3200;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x28);
}

void func_801b2c48_slot04_14(Object *obj) {
    obj->field_46 -= 0x100;
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x1e, 0x30);
        func_801482e0(obj, -0x2c, 0x30);
    }
    func_80130efc(obj);
}

void func_801b2cdc_slot04_14(Object *obj) {
    int a;

    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((s16)obj->field_46 < 0) {
        a = 0;
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            a = 6;
            obj->other->field_6b = 0xa;
        }
        obj->field_27b = a;
        func_801307e0(obj, 0x46);
    } else {
        func_80130efc(obj);
    }
}

/* The call of func_8011f0e8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 5 instruction slots. */
void func_801b2d54_slot04_14(Object *obj) {
    Object *p;
    s16 t;
    u16 y;

    func_80130efc(obj);
    t = obj->field_3a;
    if (!(t & 0x8000)) {
        if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        p = ((Object *(*)(void))func_8011f0e8)();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x14;
            p->field_03 = 6;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_4b = obj->field_4b;
            p->field_ac = (obj->field_12a >> 1) + 9;
            p->field_5c = obj->field_12a + 3;
            p->field_ad = 1;
            p->field_0b = obj->field_0b;
            p->field_0e = obj->field_0e;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            y = obj->field_70;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->field_70 = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0xb);
        }
        }
    } else {
        func_801312b8(obj);
    }
}

void func_801b2ecc_slot04_14(Object *obj) {
    s16 t;

    t = obj->field_3a;
    if (t < 0) {
        func_801312b8(obj);
    } else if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        func_80146998(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2f28_slot04_14(Object *obj) {
    data_801c6334_slot04_14[obj->field_07](obj);
}

void func_801b2f68_slot04_14(Object *obj) {
    *(s32 *)&obj->field_4c = 0x80000;
    obj->field_54 = -0x8000;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x2a);
}

void func_801b2fd8_slot04_14(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x12, 0x30);
    }
    func_80130efc(obj);
}

void func_801b3054_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    u8 i = 0;
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x80) {
        obj->field_54 = -0x8000;
        obj->field_58 = -0x6000;
        obj->field_07++;
        obj->field_4c = data_801c6350_slot04_14[obj->field_12a];
        obj->field_50 = data_801c6354_slot04_14[obj->field_12a];
    } else {
        if ((t & 0xff) == 2) {
            obj->field_3a = t & 0xff00;
            obj->field_4c = 0x80000;
            obj->field_54 = -0x8000;
        }
        if (*(u8 *)&obj->field_3a == 0) {
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->field_12a >> 1) + 1;
                }
                obj->field_27b = data_801c6368_slot04_14[i];
            } else {
                if (obj->field_0b != 0) {
                    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
                } else {
                    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
                }
                obj->field_4c = obj->field_4c + obj->field_54;
                if (obj->field_4c < 0) {
                    obj->field_54 = 0;
                    obj->field_4c = 0;
                }
            }
        }
    }
}

void func_801b31d0_slot04_14(Object *obj) {
    if (obj->field_4c >= 0) {
        if (*(u8 *)&obj->field_3a == 0) {
            obj->field_45 = 1;
            *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
            obj->field_50 = obj->field_50 + obj->field_58;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
    } else {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b3294_slot04_14(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b32ec_slot04_14(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->field_3a != 0) {
            return;
        }
    } else {
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}

void func_801b3384_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b33cc_slot04_14(Object *obj) {
    data_801c636c_slot04_14[obj->field_07](obj);
}
