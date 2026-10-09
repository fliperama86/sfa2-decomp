/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c63c4_slot04_14[];
extern ObjectFn data_801c63cc_slot04_14[];
extern ObjectFn data_801c63d4_slot04_14[];
extern ObjectFn data_801c6420_slot04_14[];
extern s32 data_801c646c_slot04_14[];

void func_80145e70(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801483a4(Object *object, int a, int b);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80126130(void);
void func_80126244(void);
void func_80155d4c(int idx, int side);
void func_80155eac(int idx, int side);
void func_80140598(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused);
void func_80131468(Object *object);
u8 func_801b61e8_slot04_14(Object *obj);
void func_801b3fec_slot04_14(Object *obj);
void func_801b41bc_slot04_14(Object *obj);

void func_801b3db8_slot04_14(Object *obj) {
    obj->field_12a = 4;
    obj->field_255 = 4;
    obj->field_159 = 0;
    obj->field_07++;
    obj->field_46 = (u8)obj->field_46 | 0x3200;
    obj->field_48 = obj->field_0b;
    func_80145e70(obj);
    func_80141f28(obj, -0x90);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3a);
}

void func_801b3e38_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        obj->field_4c = -0x80000;
        obj->field_46 = (obj->field_46 & 0xff00) | 0x30;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x80000;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x10, 0x58);
    }
    func_80130efc(obj);
}

void func_801b3ee4_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if (obj->field_46 & 0x8000) {
        obj->field_165 = 0;
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xc);
    }
    func_80130efc(obj);
}

void func_801b3f44_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((obj->field_46 & 0xff) != 0) {
        *(s32 *)&obj->field_10 += obj->field_4c;
        if (((obj->field_164 >> obj->field_48) & 1) == 0) {
            if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
                func_801b3fec_slot04_14(obj);
            } else {
                func_80130efc(obj);
            }
            return;
        }
    }
    func_801312b8(obj);
}

void func_801b3fec_slot04_14(Object *obj) {
    obj->field_07++;
    obj->field_46 = (obj->field_46 & 0xff00) | 0x10;
    func_80120554(obj->other, obj->other->side, 0x31a);
    func_801307e0(obj, 0x3b);
}

void func_801b4048_slot04_14(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((obj->field_46 & 0xff) == 0) {
        obj->field_07++;
        obj->field_46 = (obj->field_46 & 0xff00) | 0x3b;
        func_80130efc(obj);
    }
}

void func_801b409c_slot04_14(Object *obj) {
    Object *p = obj->other;

    func_80126130();
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((obj->field_46 & 0xff) == 0) {
        obj->field_07++;
        func_80120554(p, p->side, 0x306);
        func_80126244();
        func_801b41bc_slot04_14(obj);
    } else if (func_801b61e8_slot04_14(obj) != 0) {
        if (obj->field_cd == 0) {
            obj->field_be = 0;
            obj->field_bf = 0xff;
            func_80155d4c(4, (s8)obj->side);
            func_80155eac(4, obj->side);
        }
        p->field_6a++;
    }
}

void func_801b4178_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b41bc_slot04_14(Object *obj) {
    Object *p = obj->other;

    p->field_15b = 1;
    func_80140598(obj, 0, 3, 0x1f, 0x1c, 0, 0);
    if ((s16)p->field_5c < 0) {
        obj->field_167 = 0xf;
        game_state.field_6b = 6;
        func_80147000(obj);
    }
}

void func_801b4240_slot04_14(Object *obj) {
    data_801c63c4_slot04_14[obj->field_07](obj);
}

void func_801b4280_slot04_14(Object *obj) {
    obj->field_157 = 0;
    obj->field_159 = 0;
    obj->field_17b = 1;
    obj->field_07 = 1;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    func_801307e0(obj, 0x3f);
}

void func_801b42cc_slot04_14(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            game_state.field_63 = 0x18;
            func_801204f4(obj, obj->side, 0xe);
        }
        func_80130efc(obj);
    }
}

void func_801b4348_slot04_14(Object *obj) {
    data_801c63cc_slot04_14[obj->field_07](obj);
}

void func_801b4388_slot04_14(Object *obj) {
    obj->field_17b = 1;
    obj->field_45 = 1;
    obj->field_07 = 1;
    obj->field_29c = 2;
    obj->field_159 = 0;
    func_80141f28(obj, 1);
    func_80138ae8(&game_state, obj);
    obj->field_4c = -0x40000;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x42);
}

void func_801b440c_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        func_80131468(obj);
    } else {
        *(s32 *)&obj->field_10 += obj->field_4c;
        func_80130efc(obj);
    }
}

void func_801b4464_slot04_14(Object *obj) {
    data_801c63d4_slot04_14[obj->field_07](obj);
}

void func_801b44a4_slot04_14(Object *obj) {
    data_801c6420_slot04_14[obj->field_07](obj);
}

void func_801b44e4_slot04_14(Object *obj) {
    int one = 1;
    int a;

    obj->field_17b = one;
    obj->field_07 = one;
    obj->field_157 = 0;
    func_80141f28(obj, 1);
    func_80138ae8(&game_state, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x6c;
        obj->field_225 = one;
    }
    func_801307e0(obj, a);
}

void func_801b4558_slot04_14(Object *obj) {
    int k;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07++;
        k = obj->field_12a << 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = data_801c646c_slot04_14[(u8)k];
        obj->field_54 = data_801c646c_slot04_14[(u8)(k | 1)];
        obj->field_50 = data_801c646c_slot04_14[(u8)(k + 2)];
        obj->field_58 = data_801c646c_slot04_14[(u8)(k + 3)];
    }
}

void func_801b4634_slot04_14(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->pos_y = obj->pos_y - 0x28;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_46 = (u8)obj->field_46 | 0xa00;
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 += obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 -= obj->field_4c;
    }
    obj->field_4c += obj->field_54;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}
