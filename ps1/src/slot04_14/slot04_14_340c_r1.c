/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6378_slot04_14[];
extern s16 data_801c6390_slot04_14[];
extern ObjectFn data_801c6398_slot04_14[];
extern ObjectFn data_801c63a8_slot04_14[];

Object *func_8011f0e8(void);
void func_80130678(Object *object, u16 arg);
void func_8013788c(Object *object);
void func_80145d20(Object *object);
void func_80157380(Object *object);
void func_801428e4(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801482e0(Object *parent, int x, int y);
int func_80130184(Object *object);
void func_801b3564_slot04_14(Object *obj);
void func_801b3654_slot04_14(Object *obj);

void func_801b340c_slot04_14(Object *obj) {
    int n;

    obj->field_17b = 1;
    obj->field_07 = 1;
    obj->field_29c = 2;
    obj->field_252 = 0xff;
    if (obj->field_cd != 0) {
        obj->field_48 = obj->field_0b ^ obj->field_21a;
    }
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    obj->field_54 = -0x3000;
    obj->field_4c = 0xb8000;
    if (obj->field_129 != 0) {
        obj->field_4c = 0x80000;
    }
    n = 0x2d;
    if (obj->field_49 != 0) {
        n = 0x64;
        obj->field_252 = 0;
    }
    func_801307e0(obj, n);
    if (obj->field_7e == 0) {
        func_80157380(obj);
        func_801b3654_slot04_14(obj);
    }
}

void func_801b34e8_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        func_801204f4(obj, obj->side, 0xc);
        func_801b3564_slot04_14(obj);
    } else {
        obj->pos_x = obj->pos_x + (s8)*(u8 *)&obj->field_3a;
        func_80130efc(obj);
    }
}

void func_801b3564_slot04_14(Object *obj) {
    Object *other = obj->other;
    int t;

    if (obj->field_48 != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    obj->field_0b = obj->field_158;
    if (((obj->field_164 >> obj->field_48) & 1) == 0) {
        t = 0x30000;
        if (obj->field_129 != 0) {
            t = 0x24000;
        }
        if (t < obj->field_4c) {
            func_80130efc(obj);
            return;
        }
    }
    obj->field_0b = obj->field_158;
    other->field_249 = 5;
    func_801312b8(obj);
    if (obj->field_7e == 0) {
        func_8013788c(obj);
    }
}

void func_801b3654_slot04_14(Object *obj) {
    if (obj->field_49 == 0) {
        func_801379bc(obj, 1);
    }
}

void func_801b3684_slot04_14(Object *obj) {
    data_801c6378_slot04_14[obj->field_07](obj);
}

void func_801b36c4_slot04_14(Object *obj) {
    obj->field_225 = 1;
    obj->field_07 = 1;
    if (obj->field_7e == 0) {
        func_80145d20(obj);
    }
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3c);
}

void func_801b3730_slot04_14(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    Object *other = obj->other;
    s16 t;
    int i;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x80) {
        ((Slot04bObj *)obj)->field_27b = 1;
        obj->field_165 = 0;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_4b != 0) {
            other->field_6b = 0xa;
            ((Slot04bObj *)obj)->field_27b = 0;
        }
    } else if ((t & 0xff) != 0) {
        if (obj->field_165 == 0) {
            obj->field_165 = 0xff;
            if (obj->field_4b != 0) {
                obj->field_165 = 1;
            }
        }
        func_80120554(obj, obj->side, 0x31c);
        i = (((u8)obj->field_3a - 1) << 2) & 0x1fc;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801482e0(obj, *(s16 *)((u8 *)data_801c6390_slot04_14 + i), *(s16 *)((u8 *)data_801c6390_slot04_14 + i + 2));
    }
}

void func_801b382c_slot04_14(Object *obj) {
    Object *p;
    u16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 1) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_46 = *(u8 *)&obj->field_46 | 0x800;
        p = func_8011f0e8();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x14;
            p->field_03 = 8;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_4b = obj->field_4b;
            p->field_ac = (obj->field_12a >> 1) + 0xc;
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
            obj->field_240 = obj->field_240 + 1;
            func_801204f4(obj, obj->side, 0xb);
        }
    }
}

void func_801b3998_slot04_14(Object *obj) {
    s16 t = obj->field_46;
    int n;

    if ((t & 0xff00) != 0) {
        t = t - 0x100;
        obj->field_46 = t;
        if ((t & 0xff00) == 0) {
            obj->field_50 = 0x74000;
            obj->field_58 = -0x5000;
            obj->field_4c = 0x10000;
            obj->field_54 = -0x280;
            if (obj->field_0b != 0) {
                obj->field_4c = -0x10000;
                obj->field_54 = 0x280;
            }
        } else {
            func_80130efc(obj);
        }
    } else if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->field_3a < 0) {
            obj->field_159 = 0;
            obj->field_07 = obj->field_07 + 1;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        n = 0x39;
        if (obj->field_48 == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    }
}

void func_801b3ab0_slot04_14(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) == 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        n = 0x39;
        if (obj->field_48 == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3b2c_slot04_14(Object *obj) {
    data_801c6398_slot04_14[obj->field_07](obj);
}

void func_801b3b6c_slot04_14(Object *obj) {
    obj->field_17b = 1;
    obj->field_07 = 1;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, obj->field_49 != 0 ? 0x68 : 0x37);
}

void func_801b3bd0_slot04_14(Object *obj) {
    u16 n = 0x32;

    if ((s16)obj->field_3a < 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_49 != 0) {
            n = 0x69;
        }
        n = (obj->field_12a >> 1) + n;
        func_801307e0(obj, n);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3c38_slot04_14(Object *obj) {
    Object *p;
    u16 y;
    int x;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_46 = *(u8 *)&obj->field_46 | 0x600;
        p = func_8011f0e8();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x14;
            p->field_03 = 4;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            x = obj->field_12a >> 1;
            p->field_ad = 0;
            p->field_ac = x + 6;
            p->field_5c = x;
            p->field_0b = obj->field_0b;
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
            obj->field_240 = obj->field_240 + 1;
            func_801204f4(obj, obj->side, 0xb);
        }
    }
}

void func_801b3d78_slot04_14(Object *obj) {
    data_801c63a8_slot04_14[obj->field_07](obj);
}
