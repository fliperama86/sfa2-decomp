/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_80142adc(Object *object);
u8 func_80140cd8(Object *object, int a, int b);
void func_801b10b0_slot04_16(Object *obj);
void func_801b447c_slot04_16(Object *obj);
void func_801b3d78_slot04_16(Object *obj);
extern ObjectFn data_801ca218_slot04_16[];
extern u16 data_801ca23c_slot04_16[];
extern s32 data_801ca244_slot04_16[];
extern s16 data_801ca250_slot04_16[];
extern u8 data_801ca270_slot04_16[];
extern s32 data_801ca278_slot04_16[];
extern ObjectFn data_801ca2a8_slot04_16[];
extern s32 data_801ca2d0_slot04_16[];
extern s32 data_801ca300_slot04_16[];
extern s32 data_801ca330_slot04_16[];
extern s16 data_801ca33c_slot04_16[];
void func_80138ae8(GameState *state, Object *object);
void func_801b4b08_slot04_16(Object *obj);
extern u8 data_801ca34c_slot04_16[];
extern s32 data_801ca354_slot04_16[];

void func_801b3ba4_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 0;
        obj->field_07++;
        obj->pos_y = obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x1f);
    }
}

void func_801b3c20_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        game_state.field_358 = obj->other;
        game_state.field_358->field_249 = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b3c94_slot04_16(Object *obj) {
    data_801ca218_slot04_16[obj->field_07](obj);
}

void func_801b3cd4_slot04_16(Object *obj) {
    obj->field_17b = 1;
    game_state.field_358 = obj->other;
    game_state.field_358->field_249 = 0;
    obj->field_07++;
    func_801307e0(obj, (obj->field_12a >> 1) + 0x39);
    func_80141f28(obj, 5);
    func_80138ae8(game_state.config, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    func_801b3d78_slot04_16(obj);
}

void func_801b3d78_slot04_16(Object *obj) {
    s16 t = *(u16 *)((u8 *)data_801ca23c_slot04_16 + (obj->field_12a & 0xfe));
    t -= 0x26;
    if (func_8013f8c4(obj, -0x26, t) == 0) {
        obj->field_15a = 9;
        func_801b447c_slot04_16(obj);
    } else {
        obj->field_07++;
        func_80141f28(obj, 0x14);
        func_80138ae8(game_state.config, obj);
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_801307e0(obj, 0x22);
    }
}

void func_801b3e34_slot04_16(Object *obj) {
    u16 tbl[8] = { 4, 5, 6, 0, 6, 8, 9, 0 };
    int d;
    int k;
    u8 i;
    int m;
    u16 *p;
    u8 st;

    func_80130efc(obj);
    st = obj->field_3a;
    p = tbl;
    if (st != 0) {
        if (st == 1) {
            m = -0x100;
            i = obj->field_12a >> 1;
            k = (obj->field_49 != 0) << 2;
            i += k;
            func_80140cd8(obj, (s16)(p[i] | m), 0);
            obj->field_3a &= m;
            game_state.config->field_63 = 0x3c;
            func_80120554(obj, obj->side, 0x319);
            func_80146960(obj);
        } else if (st == 2) {
            obj->field_07++;
            obj->field_3a &= 0xff00;
            d = 0x60;
            if (obj->field_0b != 0) {
                d = -0x60;
            }
            obj->pos_x -= d;
        }
    }
}

void func_801b3f8c_slot04_16(Object *obj) {
    s32 tbl[12] = { 0x24000, 0x90000, 0, -0x8000, 0x24000, 0xa0000, 0, -0x8400, 0x24000, 0xb0000, 0, -0x8800 };

    func_80130efc(obj);
    if ((u8)obj->field_3a == 3) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_3a &= 0xff00;
        obj->field_4c = tbl[obj->field_12a * 2];
        obj->field_50 = tbl[obj->field_12a * 2 + 1];
        obj->field_58 = tbl[obj->field_12a * 2 + 3];
        obj->field_54 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
}

void func_801b4080_slot04_16(Object *obj) {
    func_80130184(obj);
    if ((s32)obj->field_50 < 0) {
        s32 v = data_801ca244_slot04_16[obj->field_12a >> 1];
        obj->field_07 = obj->field_07 + 1;
        obj->field_58 = v;
        func_801307e0(obj, 0x23);
    }
}

void func_801b40ec_slot04_16(Object *obj) {
    int k;
    u32 i;

    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
        return;
    }
    obj->field_07 = obj->field_07 + 1;
    obj->field_45 = 0;
    obj->pos_y = (u16)obj->field_70;
    *(u32 *)&obj->field_14 &= 0xffff0000;
    game_state.config->field_63 = (obj->field_12a << 2) + 0x3c;
    func_80146960(obj);
    func_801307e0(obj, 0x24);
    i = obj->field_12a >> 1;
    k = obj->field_49 != 0;
    if (func_80140cd8(obj, data_801ca250_slot04_16[i + (k << 2)], 0xf) != 0) {
        obj->field_167 = 2;
        if (obj->field_49 == 0) {
            return;
        }
        obj->field_167 = 0x20;
        obj->field_255 = 6;
        game_state.config->field_6b = 0;
        func_80147000(obj);
    }
    func_80120554(obj, obj->side, 0x319);
}

void func_801b4208_slot04_16(Object *obj) {
    u8 t;
    s32 a;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t != 0) {
        if (t == 1) {
            obj->field_07 = obj->field_07 + 1;
            obj->field_45 = 1;
            obj->field_3a = obj->field_3a & 0xff00;
            ref_other.p = obj->other;
            ref_other.p->field_15b = 1;
            func_80140770(obj, data_801ca270_slot04_16[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->field_5c < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x20;
                    obj->field_255 = 6;
                }
            }
            obj->field_4c = data_801ca278_slot04_16[obj->field_12a * 2];
            obj->field_50 = data_801ca278_slot04_16[obj->field_12a * 2 + 1];
            obj->field_58 = data_801ca278_slot04_16[obj->field_12a * 2 + 3];
            obj->field_54 = 0;
            if (obj->field_0b == 0) {
                obj->field_4c = -obj->field_4c;
            }
        }
    }
}

void func_801b4380_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        *(u32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x25);
    }
}

void func_801b43fc_slot04_16(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_0b = obj->field_0b ^ 1;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b447c_slot04_16(Object *obj) {
    data_801ca2a8_slot04_16[obj->field_07](obj);
}

void func_801b44bc_slot04_16(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
}

void func_801b44d0_slot04_16(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = data_801ca2d0_slot04_16[obj->field_12a * 2];
        obj->field_54 = data_801ca2d0_slot04_16[obj->field_12a * 2 + 1];
        obj->field_46 = 0x10;
    }
}

void func_801b4560_slot04_16(Object *obj) {
    func_801b4b08_slot04_16(obj);
    if (func_8013f8c4(obj, -0x26, 0x1a) == 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 == 0) {
            obj->field_07 = 9;
            func_801307e0(obj, (obj->field_12a >> 1) + 0x3c);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_07 = 3;
        func_80141f28(obj, 0xc);
        func_80138ae8((GameState *)game_state.config, obj);
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_801307e0(obj, 0x26);
    }
}

void func_801b4628_slot04_16(Object *obj) {
    s32 a;
    s32 b;

    if (*(u8 *)&obj->field_3a != 1) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        a = data_801ca300_slot04_16[obj->field_12a * 2];
        obj->field_50 = data_801ca300_slot04_16[obj->field_12a * 2 + 1];
        b = data_801ca300_slot04_16[obj->field_12a * 2 + 2];
        obj->field_58 = data_801ca300_slot04_16[obj->field_12a * 2 + 3];
        if (obj->field_0b == 0) {
            a = -a;
            b = -b;
        }
        obj->field_4c = a;
        obj->field_54 = b;
    }
}

void func_801b46f0_slot04_16(Object *obj) {
    func_80130184(obj);
    if ((s32)obj->field_50 < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_58 = data_801ca330_slot04_16[obj->field_12a >> 1];
    }
    func_80130efc(obj);
}

void func_801b4760_slot04_16(Object *obj) {
    int k;
    u32 i;

    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
        return;
    }
    obj->field_07 = obj->field_07 + 1;
    obj->field_45 = 0;
    obj->pos_y = (u16)obj->field_70;
    *(u32 *)&obj->field_14 &= 0xffff0000;
    game_state.config->field_63 = (obj->field_12a << 2) + 0x3c;
    func_80146960(obj);
    func_801307e0(obj, 0x27);
    i = obj->field_12a >> 1;
    k = obj->field_49 != 0;
    if (func_80140cd8(obj, data_801ca33c_slot04_16[i + (k << 2)], 0xf) != 0) {
        obj->field_167 = 2;
        if (obj->field_49 != 0) {
            obj->field_167 = 0x20;
            obj->field_255 = 6;
            game_state.config->field_6b = 0;
            func_80147000(obj);
        }
    }
    func_80120554(obj, obj->side, 0x319);
}

void func_801b487c_slot04_16(Object *obj) {
    s32 a;

    if (*(u8 *)&obj->field_3a == 0) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_3a &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        func_80140770(obj, data_801ca34c_slot04_16[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if (((Slot04bObj *)ref_other.p)->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        a = data_801ca354_slot04_16[obj->field_12a * 2];
        obj->field_50 = data_801ca354_slot04_16[obj->field_12a * 2 + 1];
        obj->field_58 = data_801ca354_slot04_16[obj->field_12a * 2 + 3];
        obj->field_54 = 0;
        if (obj->field_0b != 0) {
            a = -a;
        }
        obj->field_4c = a;
    }
}

