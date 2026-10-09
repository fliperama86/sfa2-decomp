/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_80141f28(Object *object, short delta);
void func_80130678(Object *object, int arg);
void func_80146478(Object *object, u8 a, int dx, int dy);
int func_80140cd8(Object *object, int a, int b);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b48fc_slot04_17(Object *obj);
u8 func_801b4974_slot04_17(Object *obj);
void func_801b4b18_slot04_17(Object *obj);
void func_801b5100_slot04_17(Object *obj);
void func_801b50a8_slot04_17(Object *obj);
void func_801b5134_slot04_17(Object *obj);
int func_801b56e0_slot04_17(Object *obj);
int func_801b5724_slot04_17(Object *obj);
void func_801b57f0_slot04_17(Object *obj);
void func_801b58b4_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);

extern u8 data_801ce6cc_slot04_17[];
extern u8 data_801ce6dc_slot04_17[];
extern ObjectFn data_801ce6ec_slot04_17[];
extern ObjectFn data_801ce6f4_slot04_17[];
extern ObjectFn data_801ce6fc_slot04_17[];
extern s32 data_801ce708_slot04_17[];
extern ObjectFn data_801ce728_slot04_17[];
extern ObjectFn data_801ce738_slot04_17[];
extern ObjectFn data_801ce740_slot04_17[];
extern u8 data_801ce74c_slot04_17[];

void func_801b47b0_slot04_17(Object *obj) {
    Config *c;

    func_801b48fc_slot04_17(obj);
    func_801b56e0_slot04_17(obj);
    if (obj->pos_y >= obj->field_70) {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        func_801209c4(obj);
        if (obj->field_12e < obj->field_12f && ((c = game_state.config)->field_4d | c->field_4e | c->field_04) == 0 && func_8012f56c(obj) == 0) {
            obj->field_07 = 4;
            func_801307e0(obj, 0x4c);
        } else {
            obj->field_07++;
            func_801307e0(obj, 0x6f);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b489c_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b48fc_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->field_cd == 0) {
        if (*(u16 *)&obj->field_1c0 == 0) {
            if (o->field_134 != 0) {
                *(u16 *)&obj->field_1c0 = o->field_134 & 0x68;
            }
        }
    }
}

u8 func_801b4944_slot04_17(Object *obj, int a) {
    if (obj->field_0b != 0) {
        return data_801ce6cc_slot04_17[a >> 3];
    }
    return a;
}

u8 func_801b4974_slot04_17(Object *obj) {
    return data_801ce6dc_slot04_17[func_80151184() & 0xf];
}

void func_801b49a8_slot04_17(Object *obj) {
    func_801b58b4_slot04_17(obj);
}

void func_801b49c8_slot04_17(Object *obj) {
    data_801ce6ec_slot04_17[obj->field_07](obj);
}

void func_801b4a08_slot04_17(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801307e0(obj, 0x20);
}

void func_801b4a54_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4a98_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        func_801b4b18_slot04_17(obj);
    } else if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4b18_slot04_17(Object *obj) {
    data_801ce6f4_slot04_17[obj->field_07](obj);
}

void func_801b4b58_slot04_17(Object *obj) {
    obj->field_4c = 0x50000;
    obj->field_54 = -0x3000;
    obj->field_50 = 0;
    obj->field_58 = 0;
    obj->field_07++;
}

void func_801b4b80_slot04_17(Object *obj) {
    int a;
    u16 b;

    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
        if ((obj->field_3a & 0x80) == 0) {
            func_801b5724_slot04_17(obj);
        }
        a = obj->field_3a & 0x7f;
        if (a != 0) {
            b = a;
            obj->field_3a = obj->field_3a & 0xff80;
            if (obj->field_0b == 0) {
                b = -a;
            }
            obj->pos_x = b + obj->pos_x;
        }
    }
}

void func_801b4c40_slot04_17(Object *obj) {
    data_801ce6fc_slot04_17[obj->field_07](obj);
}

void func_801b4c80_slot04_17(Object *obj) {
    int k;

    obj->field_07++;
    k = obj->kind != 0x11;
    obj->field_4c = data_801ce708_slot04_17[(k << 2) + 0];
    obj->field_54 = data_801ce708_slot04_17[(k << 2) + 1];
    obj->field_50 = data_801ce708_slot04_17[(k << 2) + 2];
    obj->field_58 = data_801ce708_slot04_17[(k << 2) + 3];
    obj->field_45 = 1;
    func_80130efc(obj);
}

void func_801b4d10_slot04_17(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0 && func_801b5724_slot04_17(obj) < 0 && obj->pos_y >= (s16)((Slot04bObj *)obj)->field_70) {
        obj->field_07++;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
    func_80130efc(obj);
}

void func_801b4da0_slot04_17(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4e00_slot04_17(Object *obj) {
    data_801ce728_slot04_17[obj->field_07](obj);
}

void func_801b4e40_slot04_17(Object *obj) {
    int a;

    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->field_45 == 0) {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b4eac_slot04_17(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->field_46 = 1;
        obj->field_07++;
        func_801b50a8_slot04_17(obj);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b5134_slot04_17(obj);
        func_801204f4(obj, obj->side, 7);
    }
    func_80130efc(obj);
}

void func_801b4f40_slot04_17(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->field_07++;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b4fc8_slot04_17(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 == 0) {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 3;
            obj->field_07 = 1;
        }
        func_80142c70(obj);
    } else {
        if (((Slot04bObj *)obj)->field_134 != 0) {
            if (((Slot04bObj *)obj)->field_27d == 0) {
                ((Slot04bObj *)obj)->field_27d = obj->field_27c;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        func_80142fe8(obj);
    }
}

void func_801b50a8_slot04_17(Object *obj) {
    int a;
    int b;

    if (obj->kind == 0x13) {
        func_801b5100_slot04_17(obj);
    } else {
        a = -3;
        b = 0x4f;
        if (obj->field_45 != 0) {
            a = -0xd;
            b = 0x44;
        }
        func_801495f8(obj, a, b);
    }
}

void func_801b5100_slot04_17(Object *obj) {
    if (obj->field_45 != 0) {
        func_801495f8(obj, -0xe, 0x47);
    } else {
        func_801495f8(obj, 8, 0x47);
    }
}

void func_801b5134_slot04_17(Object *obj) {
    int z = 0;
    int c;
    int i;
    u8 *p;

    p = (u8 *)obj + 0x2b0;
    for (i = 0, c = z; i < 0x40; i++) {
        *p++ = c;
    }
    p = (u8 *)obj + 0x184;
    for (i = 0; i < 0x18; i++) {
        *p++ = z;
    }
}

void func_801b5178_slot04_17(Object *obj) {
    func_801b5134_slot04_17(obj);
}

void func_801b5198_slot04_17(Object *obj) {
    data_801ce738_slot04_17[obj->field_129 >> 1](obj);
}

void func_801b51dc_slot04_17(Object *obj) {
    data_801ce740_slot04_17[obj->field_07](obj);
}

void func_801b521c_slot04_17(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 8);
    obj->field_0b = 0;
    if (!(obj->field_cd != 0 ? (s16)((u16)func_801b5890_slot04_17(obj)->pos_x + 0xc0) < obj->pos_x : (obj->field_c2 & 0x8000) != 0)) {
        obj->field_0b++;
    }
    func_801307e0(obj, 0x1a);
}

void func_801b52e8_slot04_17(Object *obj) {
    int i;
    s16 c;
    s16 d;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[6];

    ref_other.p = obj->other;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
    } else if (*(u8 *)&obj->field_3a != 0) {
        i = ((*(u8 *)&obj->field_3a + 0xff) << 4) & 0x3f0;
        obj->field_3a = 0;
        c = *(u16 *)(data_801ce74c_slot04_17 + i + 8);
        d = *(u16 *)(data_801ce74c_slot04_17 + i + 12);
        func_80146478(obj, obj->field_12a >> 1, *(s16 *)(data_801ce74c_slot04_17 + i), *(s16 *)(data_801ce74c_slot04_17 + i + 4));
        if (func_80140cd8(obj, c, d)) {
            func_80120554(obj, obj->side, 0x336);
            obj->field_07++;
            func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
        } else {
            func_80120554(obj, obj->side, 0x304);
            ref_other.p = obj->other;
            if (ref_other.p->field_15b == 0) {
                obj->field_07++;
                func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
            }
        }
    }
    func_80130efc(obj);
}
