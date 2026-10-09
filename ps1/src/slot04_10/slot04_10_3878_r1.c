/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_80142adc(Object *object);
void func_801b1078_slot04_10(Object *obj);
void func_801b4444_slot04_10(Object *obj);
void func_801b3d40_slot04_10(Object *obj);
extern ObjectFn data_801c6a84_slot04_10[];
extern u16 data_801c6aa8_slot04_10[];

void func_801b3878_slot04_10(Object *obj) {
    s16 a[12] = { 0x17, 0, 0x1b, 0, 0x1f, 0x12, 0x14, 0xf, 0x16, 0xf, 0x18, 0xf };
    s16 b[3] = { 0x20, 0x21, 0x22 };
    s32 c[12] = { 0x38000, 0x60000, 0, -0x8000, 0x30000, 0x70000, 0, -0x8400, 0x48000, 0x80000, 0, -0x8800 };
    u16 t;
    s16 *p;
    int k;
    u8 i;

    t = obj->field_3a;
    if ((u8)t == 0) {
        func_80130efc(obj);
        return;
    }
    if ((u8)t != 0xf) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_3a &= 0xff00;
        game_state.field_358 = obj->other;
        game_state.field_358->field_15b = 1;
        func_80140770(obj, ((u8 *)b)[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 1);
        game_state.field_358 = obj->other;
        if ((s16)game_state.field_358->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        obj->field_4c = c[(obj->field_12a >> 1) * 4 + 0];
        obj->field_50 = c[(obj->field_12a >> 1) * 4 + 1];
        obj->field_58 = c[(obj->field_12a >> 1) * 4 + 3];
        obj->field_54 = 0;
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
        }
    } else {
        obj->field_3a = t & 0xff00;
        game_state.config->field_63 = (obj->field_12a << 2) + 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        k = obj->field_49 != 0 ? 6 : 0; p = (s16 *)((obj->field_12a + k) * 2 + (u32)a);
        if ((u8)func_80140cd8(obj, p[0], p[1]) != 0) {
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
}

void func_801b3b6c_slot04_10(Object *obj) {
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

void func_801b3be8_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        game_state.field_358 = obj->other;
        game_state.field_358->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}

void func_801b3c5c_slot04_10(Object *obj) {
    data_801c6a84_slot04_10[obj->field_07](obj);
}

void func_801b3c9c_slot04_10(Object *obj) {
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
    func_801b3d40_slot04_10(obj);
}

void func_801b3d40_slot04_10(Object *obj) {
    s16 t = *(u16 *)((u8 *)data_801c6aa8_slot04_10 + (obj->field_12a & 0xfe));
    t -= 0x26;
    if ((u8)func_8013f8c4(obj, -0x26, t) == 0) {
        obj->field_15a = 9;
        func_801b4444_slot04_10(obj);
    } else {
        obj->field_07++;
        func_80141f28(obj, 0x14);
        func_80138ae8(game_state.config, obj);
        func_80120554(obj, obj->side ^ 1, 0x31a);
        func_801307e0(obj, 0x22);
    }
}

void func_801b3dfc_slot04_10(Object *obj) {
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

void func_801b3f54_slot04_10(Object *obj) {
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
