/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6ab0_slot04_10[];
extern s16 data_801c6abc_slot04_10[];
extern u8 data_801c6adc_slot04_10[];
extern s32 data_801c6ae4_slot04_10[];
extern ObjectFn data_801c6b14_slot04_10[];
extern s32 data_801c6b3c_slot04_10[];
extern s32 data_801c6b6c_slot04_10[];
extern s32 data_801c6b9c_slot04_10[];
extern s16 data_801c6ba8_slot04_10[];

int func_80130184(Object *object);
u8 func_80140cd8(Object *object, int a, int b);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80142adc(Object *object);
void func_80146960(Object *object);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80138ae8(GameState *state, Object *object);
void func_801b1078_slot04_10(Object *obj);
void func_801b4ad0_slot04_10(Object *obj);

void func_801b4048_slot04_10(Object *obj) {
    func_80130184(obj);
    if ((s32)obj->field_50 < 0) {
        s32 v = data_801c6ab0_slot04_10[obj->field_12a >> 1];
        obj->field_07 = obj->field_07 + 1;
        obj->field_58 = v;
        func_801307e0(obj, 0x23);
    }
}

void func_801b40b4_slot04_10(Object *obj) {
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
    if (func_80140cd8(obj, data_801c6abc_slot04_10[i + (k << 2)], 0xf) != 0) {
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

void func_801b41d0_slot04_10(Object *obj) {
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
            func_80140770(obj, data_801c6adc_slot04_10[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->field_5c < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x20;
                    obj->field_255 = 6;
                }
            }
            obj->field_4c = data_801c6ae4_slot04_10[obj->field_12a * 2];
            obj->field_50 = data_801c6ae4_slot04_10[obj->field_12a * 2 + 1];
            obj->field_58 = data_801c6ae4_slot04_10[obj->field_12a * 2 + 3];
            obj->field_54 = 0;
            if (obj->field_0b == 0) {
                obj->field_4c = -obj->field_4c;
            }
        }
    }
}

void func_801b4348_slot04_10(Object *obj) {
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

void func_801b43c4_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_0b = obj->field_0b ^ 1;
        func_801b1078_slot04_10(obj);
    }
}

void func_801b4444_slot04_10(Object *obj) {
    data_801c6b14_slot04_10[obj->field_07](obj);
}

void func_801b4484_slot04_10(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
}

void func_801b4498_slot04_10(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = data_801c6b3c_slot04_10[obj->field_12a * 2];
        obj->field_54 = data_801c6b3c_slot04_10[obj->field_12a * 2 + 1];
        obj->field_46 = 0x10;
    }
}

void func_801b4528_slot04_10(Object *obj) {
    func_801b4ad0_slot04_10(obj);
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

void func_801b45f0_slot04_10(Object *obj) {
    s32 a;
    s32 b;

    if (*(u8 *)&obj->field_3a != 1) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        a = data_801c6b6c_slot04_10[obj->field_12a * 2];
        obj->field_50 = data_801c6b6c_slot04_10[obj->field_12a * 2 + 1];
        b = data_801c6b6c_slot04_10[obj->field_12a * 2 + 2];
        obj->field_58 = data_801c6b6c_slot04_10[obj->field_12a * 2 + 3];
        if (obj->field_0b == 0) {
            a = -a;
            b = -b;
        }
        obj->field_4c = a;
        obj->field_54 = b;
    }
}

void func_801b46b8_slot04_10(Object *obj) {
    func_80130184(obj);
    if ((s32)obj->field_50 < 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_58 = data_801c6b9c_slot04_10[obj->field_12a >> 1];
    }
    func_80130efc(obj);
}

void func_801b4728_slot04_10(Object *obj) {
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
    if (func_80140cd8(obj, data_801c6ba8_slot04_10[i + (k << 2)], 0xf) != 0) {
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
