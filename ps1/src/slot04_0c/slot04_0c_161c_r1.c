/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bd9d0_slot04_0c[];
extern ObjectFn data_801bda10_slot04_0c[];
extern ObjectFn data_801bda20_slot04_0c[];
extern s32 data_801bd9e0_slot04_0c[];
Object *func_8011f0e8(void);
void func_80142adc(Object *object);
void func_80146998(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801483a4(Object *object, u16 a, u16 b);

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 3 instruction slots. */
void func_801b161c_slot04_0c(Object *obj) {
    Object *p;
    s16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_46 = 5;
        obj->field_07++;
        p = ((Object *(*)(Object *))func_8011f0e8)(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xc;
            p->field_03 = 0;
            p->field_65 = obj->field_65;
            p->field_66 = obj->field_66;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_7a = 0x60;
            p->field_5c = 0;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0x15);
        }
    }
}

void func_801b1754_slot04_0c(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        t = obj->field_46;
        if (t != 0) {
            t = t - 1;
            obj->field_46 = t;
            if (t != 0) {
                goto tail;
            }
            obj->field_17b = 0;
        }
        func_80142adc(obj);
    tail:
        func_80130efc(obj);
    }
}

void func_801b17d4_slot04_0c(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}

void func_801b183c_slot04_0c(Object *obj) {
    data_801bd9d0_slot04_0c[obj->field_07](obj);
}

void func_801b187c_slot04_0c(Object *obj) {
    int n = 0x22;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        n = 0x39;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + n);
}

void func_801b18fc_slot04_0c(Object *obj) {
    int i;
    s32 a;
    s32 b;

    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_14 = 0;
        i = obj->field_12a;
        a = data_801bd9e0_slot04_0c[i * 2];
        b = data_801bd9e0_slot04_0c[i * 2 + 1];
        obj->field_50 = data_801bd9e0_slot04_0c[i * 2 + 2];
        obj->field_58 = data_801bd9e0_slot04_0c[i * 2 + 3];
        if (obj->field_0b == 0) {
            obj->field_4c = -a;
            obj->field_54 = -b;
        } else {
            obj->field_4c = a;
            obj->field_54 = b;
        }
    }
    func_80130efc(obj);
}

void func_801b19b8_slot04_0c(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        obj->field_157 = 1;
        func_801307e0(obj, 0x28);
    }
}

void func_801b1a74_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1acc_slot04_0c(Object *obj) {
    data_801bda10_slot04_0c[obj->field_07](obj);
}

void func_801b1b0c_slot04_0c(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x29);
}

void func_801b1b60_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_801483a4(obj, 0x1e, 0x57);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}

void func_801b1be4_slot04_0c(Object *obj) {
    int s1 = 0;

    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            s1 = 4;
            obj->other->field_6b = 0xa;
        }
        obj->field_27b = s1;
    }
}

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 6 instruction slots. */
void func_801b1c54_slot04_0c(Object *obj) {
    Object *p;
    u16 t;
    s16 m;
    s16 y;

    func_80130efc(obj);
    t = obj->field_3a;
    m = t & 0xff00;
    if (m < 0) {
        func_801312b8(obj);
    } else if (m == 0 && (t & 0xff) != 0) {
        obj->field_3a = 0;
        p = ((Object *(*)(Object *))func_8011f0e8)(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xc;
            p->field_03 = 2;
            p->field_65 = obj->field_65;
            p->field_4b = obj->field_4b;
            p->field_66 = obj->field_66;
            p->field_ac = obj->field_12a + 6;
            p->field_5c = (obj->field_12a >> 1) + 2;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0x15);
        }
    }
}

void func_801b1dd0_slot04_0c(Object *obj) {
    data_801bda20_slot04_0c[obj->field_07](obj);
}
