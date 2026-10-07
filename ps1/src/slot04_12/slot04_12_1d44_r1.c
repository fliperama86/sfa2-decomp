/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c4b80_slot04_12[];
extern ObjectFn data_801c4b8c_slot04_12[];
extern s32 data_801c4b9c_slot04_12[];
extern ObjectFn data_801c4bcc_slot04_12[];
extern s32 data_801c4bdc_slot04_12[];
extern u8 data_801c4c0c_slot04_12[];

Object *func_8011f0e8(void);
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
s32 func_801b3a90_slot04_12(Object *object);

void func_801b1d44_slot04_12(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 0;
        obj->field_07++;
    } else {
        if (obj->field_4c < 0) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
    }
    func_80130efc(obj);
}

void func_801b1de8_slot04_12(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xb);
        obj->field_4c = obj->field_50;
        obj->field_54 = obj->field_58;
    }
    func_80130efc(obj);
}

void func_801b1e50_slot04_12(Object *obj) {
    if (!((s16)obj->field_3a & 0x8000)) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_50 = 0;
        obj->field_58 = 0;
        func_801312b8(obj);
    }
}

void func_801b1ec0_slot04_12(Object *obj) {
    data_801c4b80_slot04_12[obj->field_07](obj);
}

void func_801b1f00_slot04_12(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    a = 0x2d;
    if (obj->field_49 != 0) {
        a = 0x49;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
}

/* The call of func_8011f0e8 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_801b1f78_slot04_12(Object *obj) {
    Object *p;
    s16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->field_46 = *(u8 *)&obj->field_46 + 0x500;
        p = ((Object *(*)(Object *))func_8011f0e8)(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x1c;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_5c = 0;
            p->field_3c = obj;
            p->pos_y = y;
            obj->field_14c = (s32)p;
            obj->field_240++;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
}

void func_801b20a8_slot04_12(Object *obj) {
    s16 t = obj->field_46;

    if (t & 0xff00) {
        t = t - 0x100;
        obj->field_46 = t;
        if ((t & 0xff00) == 0) {
            obj->field_17b = 0;
        }
    }
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b2148_slot04_12(Object *obj) {
    data_801c4b8c_slot04_12[obj->field_07](obj);
}

void func_801b2188_slot04_12(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 10);
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x + 0x18;
    } else {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x - 0x18;
    }
    obj->field_4c = data_801c4b9c_slot04_12[obj->field_12a * 2];
    obj->field_54 = data_801c4b9c_slot04_12[obj->field_12a * 2 + 1];
    obj->field_50 = data_801c4b9c_slot04_12[obj->field_12a * 2 + 2];
    obj->field_58 = data_801c4b9c_slot04_12[obj->field_12a * 2 + 3];
    func_801204f4(obj, obj->side, 7);
    if (obj->field_49 != 0) {
        a = (obj->field_12a >> 1) + 0x4c;
    } else {
        a = obj->field_12a + 0x30;
    }
    func_801307e0(obj, a);
}

void func_801b22c8_slot04_12(Object *obj) {
    if ((obj->field_3a & 0x80) == 0) {
        obj->field_45 = 1;
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b230c_slot04_12(Object *obj) {
    if (func_801b3a90_slot04_12(obj) >= 0) {
        func_80130efc(obj);
    } else {
        if (obj->field_49 != 0) {
            obj->field_58 = 0xffff0000;
        } else {
            obj->field_58 = -0x6000;
        }
        if (obj->pos_y < obj->field_70) {
            func_80130efc(obj);
        } else {
            obj->field_07++;
            obj->field_14 = 0;
            obj->field_45 = 0;
            obj->field_159 = 0;
            obj->field_17b = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, obj->field_12a + 0x31);
        }
    }
}

void func_801b23b4_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b2428_slot04_12(Object *obj) {
    data_801c4bcc_slot04_12[obj->field_07](obj);
}

void func_801b2468_slot04_12(Object *obj) {
    u16 a;

    obj->field_07++;
    obj->field_46 = *(u8 *)&obj->field_46 + 0x400;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    obj->field_4c = data_801c4bdc_slot04_12[obj->field_12a * 2];
    obj->field_54 = data_801c4bdc_slot04_12[obj->field_12a * 2 + 1];
    obj->field_50 = data_801c4bdc_slot04_12[obj->field_12a * 2 + 2];
    obj->field_58 = data_801c4bdc_slot04_12[obj->field_12a * 2 + 3];
    a = (obj->field_12a >> 1) + 0x50;
    func_801307e0(obj, a);
}

void func_801b2548_slot04_12(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        obj->field_46 = *(u8 *)&obj->field_46 + 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -6, 0x4a);
    }
}

void func_801b25dc_slot04_12(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_27b = data_801c4c0c_slot04_12[3];
        } else {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
            obj->field_27b = data_801c4c0c_slot04_12[obj->field_12a >> 1];
        }
        obj->field_165 = 0;
        func_801204f4(obj, obj->side, 5);
    }
}

void func_801b2688_slot04_12(Object *obj) {
    u16 t;

    if (obj->field_4c != 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c - obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_4c = 0;
        }
    }
    t = obj->field_3a;
    if (t & 0xff) {
        obj->field_3a = t & 0xff00;
        obj->field_4c = obj->field_50;
        obj->field_54 = obj->field_58;
    }
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
