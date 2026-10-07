/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b44_slot04_0f[];
extern u8 data_801c5b5c_slot04_0f[];
extern ObjectFn data_801c5b60_slot04_0f[];
extern u8 data_801c5b94_slot04_0f[];
Object *func_8011f0e8(void);
void func_80130678(Object *object, int index);
int func_80130184(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b2270_slot04_0f(Object *object);
void func_801b3280_slot04_0f(Object *object);
void func_801b394c_slot04_0f(Object *object);

void func_801b3120_slot04_0f(Object *obj) {
    func_80130efc(obj);
    func_801b2270_slot04_0f(obj);
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        func_801b3280_slot04_0f(obj);
        func_801204f4(obj, obj->side, 0xf);
    } else if (obj->field_70 < obj->pos_y) {
        func_80130184(obj);
    }
}

void func_801b31b0_slot04_0f(Object *obj) {
    func_80130efc(obj);
    func_801b2270_slot04_0f(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_07++;
        obj->field_15b = 0;
        obj->field_247 = 0;
        obj->field_6a = 0;
        obj->field_69 = 0;
        *(s32 *)&obj->field_10 = *(s32 *)&((Slot04bObj *)obj)->field_330;
        obj->pos_y = obj->field_70;
        obj->field_0b = obj->field_158;
    }
}

void func_801b3228_slot04_0f(Object *obj) {
    func_80130efc(obj);
    func_801b2270_slot04_0f(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 1;
        obj->field_05 = 1;
        obj->field_06 = 2;
        obj->field_07 = 0;
        obj->field_45 = 0;
    }
}

void func_801b3280_slot04_0f(Object *obj) {
    s32 *dst = (s32 *)&((Slot04bObj *)obj)->field_330;
    s32 x = *(s32 *)&obj->field_10;
    s32 vx = obj->field_4c;
    s32 y = *(s32 *)&obj->field_14;
    s32 vy = obj->field_50;
    s32 ax = obj->field_54;
    s32 ay = obj->field_58;
    s16 h = obj->field_70;

    do {
        x += vx;
        vx += ax;
        y -= vy;
        vy += ay;
    } while (!(h < y));
    *dst = x;
}

void func_801b32c0_slot04_0f(Object *obj) {
    data_801c5b44_slot04_0f[obj->field_07](obj);
}

void func_801b3300_slot04_0f(Object *obj) {
    obj->field_07++;
    obj->field_46 = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38((GameState *)game_state.config, obj);
    func_801307e0(obj, 0x4a);
}

void func_801b3360_slot04_0f(Object *obj) {
    int a;

    obj->field_46 -= 1;
    if (*(u8 *)&obj->field_3a != 0) {
        a = -1;
        obj->field_07++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0xe, 0x60);
    }
    func_80130efc(obj);
}

void func_801b33e0_slot04_0f(Object *obj) {
    int a = 0;

    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c5b5c_slot04_0f[a];
        obj->field_48 = 0x16;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x4b);
    } else {
        func_80130efc(obj);
    }
}

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_801b3484_slot04_0f(Object *obj) {
    Object *p;
    u8 t;
    s32 w;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        p = ((Object *(*)(Object *))func_8011f0e8)(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x1b;
            p->field_03 = 0;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_4b = obj->field_4b;
            t = obj->field_12a;
            p->field_ad = 0;
            p->field_ac = t;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_26 = obj->field_26;
            *(s32 *)&p->field_10 = *(s32 *)&obj->field_10;
            w = *(s32 *)&obj->field_14;
            p->field_5c = 0xff;
            p->field_48 = 0x16;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            *(s32 *)&p->field_14 = w;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
}

void func_801b35b4_slot04_0f(Object *obj) {
    func_80130efc(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_07++;
        func_801307e0(obj, 0x4e);
    }
}

void func_801b3600_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3640_slot04_0f(Object *obj) {
    data_801c5b60_slot04_0f[obj->field_07](obj);
}

void func_801b3680_slot04_0f(Object *obj) {
    obj->field_07++;
    obj->field_46 = 0x32;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x4f);
}

void func_801b36e0_slot04_0f(Object *obj) {
    int a;

    obj->field_46 -= 1;
    if (*(u8 *)&obj->field_3a != 0) {
        a = -1;
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0, 0x61);
    }
    func_80130efc(obj);
}

void func_801b376c_slot04_0f(Object *obj) {
    u8 a = 0;

    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c5b94_slot04_0f[a];
    }
    func_80130efc(obj);
}

void func_801b37f8_slot04_0f(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_50 = -0x60000;
        obj->field_54 = 0x500;
        obj->field_58 = 0x3000;
        obj->field_07++;
        obj->field_4c = -0x24000;
        obj->field_3a = obj->field_3a & 0xff00;
    }
}

void func_801b3870_slot04_0f(Object *obj) {
    func_80130efc(obj);
    if (obj->field_50 <= 0) {
        func_801b394c_slot04_0f(obj);
        if ((s16)obj->field_3a < 0) {
            if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x30)) {
                obj->field_07 = 7;
                func_80120554(obj, player_left.side, 0x31a);
                func_801204f4(obj, obj->side, 0xa);
                func_801307e0(obj, 0x51);
            }
        }
    } else {
        obj->field_4c = -0x19b00;
        obj->field_50 = 0x3000;
        obj->field_54 = 0x500;
        obj->field_58 = 0x3000;
        obj->field_07++;
        func_801307e0(obj, 0x50);
    }
}

void func_801b394c_slot04_0f(Object *obj) {
    int v;

    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    v = obj->field_4c;
    if (obj->field_0b != 0) {
        v = -v;
    }
    *(s32 *)&obj->field_10 = v + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
}

void func_801b39a0_slot04_0f(Object *obj) {
    func_80130efc(obj);
    func_801b394c_slot04_0f(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
}

void func_801b3a14_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3a54_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_50 = 0x10000;
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_58 = 0x6000;
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b3aa4_slot04_0f(Object *obj) {
    func_801b394c_slot04_0f(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        obj->field_46 = obj->field_12a >> 1;
        func_801307e0(obj, 0x52);
    } else {
        func_80130efc(obj);
    }
}
