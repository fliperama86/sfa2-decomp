/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c4a20_slot04_12[];
extern ObjectFn data_801c4a40_slot04_12[];
extern ObjectFn data_801c4a6c_slot04_12[];
extern ObjectFn data_801c4a98_slot04_12[];
extern u8 data_801c4aa8_slot04_12[];
extern u8 data_801c4ab8_slot04_12[];
extern u32 data_801c4abc_slot04_12[];
extern ObjectFn data_801c4b3c_slot04_12[];
extern s32 data_801c4b50_slot04_12[];

u8 func_80141788(Object *object);
int func_80141e34(Object *object);
void func_80142adc(Object *object);
void func_80146998(Object *object);
void func_80141f28(Object *object, short delta);
void func_80138ae8(GameState *state, Object *object);

void func_801b17c4_slot04_12(Object *obj);
void func_801b19bc_slot04_12(Object *obj);
void func_801b1a08_slot04_12(Object *obj);
void func_801b1a54_slot04_12(Object *obj);

void func_801b1378_slot04_12(Object *obj) {
    ref_other.p = obj->other;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 9;
    obj->field_159 = 1;
    obj->field_157 = 0;
    obj->field_12a = 4;
    obj->field_6b = 0;
    obj->field_0b = obj->field_158;
    ref_other.p->field_6b = 0x14;
    ref_other.p->field_27b = 0x18;
    func_80146998(obj);
    func_801307e0(obj, 0x1e);
}


int func_801b1414_slot04_12(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            if (obj->field_45 != 0) {
                if (obj->field_7e == 0) {
                    if (func_801418bc(obj)) {
                        r = 1;
                        obj->field_04 = 1;
                        obj->field_05 = 0;
                        obj->field_06 = 8;
                        obj->field_07 = 0;
                        obj->field_15a = 0xa;
                    }
                }
            } else {
                if ((u8)func_80141e34(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->field_04 = 1;
                        obj->field_05 = 0;
                        obj->field_06 = 8;
                        obj->field_07 = 0;
                        obj->field_15a = 0xa;
                        obj->field_0b = obj->field_158;
                    }
                }
            }
        }
    }
    return r;
}

void func_801b1510_slot04_12(Object *obj) {
    int z = 0;
    u32 i;
    u8 *p;
    int c;

    p = (u8 *)obj + 0x2c0;
    for (i = 0, c = z; i < 9; i++) {
        *p++ = c;
    }
    if (obj->kind != 4) {
        p = (u8 *)obj + 0x2c8;
        for (i = 0; i < 9; i++) {
            *p++ = z;
        }
    }
}

void func_801b156c_slot04_12(Object *obj) {
    data_801ad398 = data_801c4a20_slot04_12[obj->field_15a](obj);
}

int func_801b15b4_slot04_12(Object *obj) {
    return 1;
}

int func_801b15bc_slot04_12(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b15c8_slot04_12(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b15dc_slot04_12(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b15f0_slot04_12(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b1604_slot04_12(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1610_slot04_12(Object *obj) {
    data_801c4a40_slot04_12[obj->field_15a](obj);
}

void func_801b1650_slot04_12(Object *obj) {
    data_801c4a6c_slot04_12[obj->field_15a](obj);
}

void func_801b1690_slot04_12(Object *obj) {
    data_801c4a98_slot04_12[obj->field_07](obj);
}

void func_801b16d0_slot04_12(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->field_254 = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a;
    func_801307e0(obj, a);
}

void func_801b1744_slot04_12(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        func_80130efc(obj);
    } else {
        obj->field_46 = 0xf;
        obj->field_07++;
        if (obj->field_cd == 0) {
            func_801b17c4_slot04_12(obj);
        } else {
            obj->field_46 = data_801c4aa8_slot04_12[obj->field_129];
            func_801b1a54_slot04_12(obj);
        }
    }
}

void func_801b17c4_slot04_12(Object *obj) {
    s16 a;
    int s;
    u8 t;
    u8 g;
    u8 u;
    FrameRecord *fr;

    s = (obj->field_3a << 16) >> 24;
    if (obj->field_cd != 0) {
        func_801b1a54_slot04_12(obj);
    } else {
        if ((*(u32 *)&game_state.field_4c & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
            func_801b1a08_slot04_12(obj);
            return;
        }
        if ((s & 0x7f) != 0) {
            obj->field_3a = obj->field_3a & 0x80ff;
            func_80142c04(obj);
            func_80138ae8(&game_state, obj);
            if (obj->field_254 == 0) {
                obj->field_254 = 0xff;
            }
        }
        if ((obj->field_134 & 0x68) == 0) {
            func_801b19bc_slot04_12(obj);
            return;
        }
        if (obj->field_134 & 0x40) {
            a = 0;
        } else if (obj->field_134 & 0x20) {
            a = 2;
        } else {
            a = 4;
        }
        if ((s16)obj->field_46 <= data_801c4ab8_slot04_12[a >> 1]) {
            func_801b19bc_slot04_12(obj);
            return;
        }
        obj->field_46 = 0xf;
        if (obj->field_12a == a) {
            func_80130efc(obj);
            return;
        }
        obj->field_12a = a;
        u = obj->field_3a;
        g = obj->field_49;
        if (g != 0) {
            a += 0x40;
        } else {
            a += 0x21;
        }
        func_801307e0(obj, a);
        obj->sequence = &obj->sequence[u];
        obj->field_3a = obj->sequence->flags;
        obj->field_38 = obj->sequence->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        t = fr->field_0d;
        obj->field_80 = 1;
        obj->field_4a = t;
    }
}

void func_801b19bc_slot04_12(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        func_80130efc(obj);
    } else {
        func_801b1a08_slot04_12(obj);
    }
}

void func_801b1a08_slot04_12(Object *obj) {
    u16 a;

    obj->field_07++;
    a = 0x21;
    obj->field_17b = 0;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    func_801307e0(obj, a + obj->field_12a + 1);
}

void func_801b1a54_slot04_12(Object *obj) {
    u32 *w = (u32 *)&game_state.field_4c;
    u16 t;

    if ((*w & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
        goto tail;
    }
    t = obj->field_3a;
    if ((t & 0x7f00) == 0 || (t & 0x7f00) != 0x100) {
        func_80130efc(obj);
        return;
    }
    obj->field_3a = t & 0x80ff;
    func_80142c04(obj);
    func_80138ae8(&game_state, obj);
    if (obj->field_254 == 0) {
        obj->field_254 = 0xff;
    }
    if (obj->field_129 != 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 != 0) {
            func_80130efc(obj);
            return;
        }
    } else {
        if (!(u8)func_8014a170(obj, data_801c4abc_slot04_12)) {
            func_80130efc(obj);
            return;
        }
    }
tail:
    obj->field_07++;
    obj->field_17b = 0;
    func_801307e0(obj, obj->field_12a + 0x22);
}

void func_801b1b88_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1bfc_slot04_12(Object *obj) {
    data_801c4b3c_slot04_12[obj->field_07](obj);
}

void func_801b1c3c_slot04_12(Object *obj) {
    u8 one = 1;
    u16 a;

    obj->field_17b = one;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->field_4c = data_801c4b50_slot04_12[obj->field_12a * 2];
    obj->field_54 = data_801c4b50_slot04_12[obj->field_12a * 2 + 1];
    obj->field_50 = data_801c4b50_slot04_12[obj->field_12a * 2 + 2];
    obj->field_58 = data_801c4b50_slot04_12[obj->field_12a * 2 + 3];
    obj->field_45 = one;
    if (obj->field_49 != 0) {
        a = (obj->field_12a >> 1) + 0x46;
    } else {
        a = 0x27;
    }
    func_801307e0(obj, a);
}
