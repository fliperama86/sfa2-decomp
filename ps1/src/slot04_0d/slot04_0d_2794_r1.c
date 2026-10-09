/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2824_slot04_0d[];
extern ObjectFn data_801c2a90_slot04_0d[];
extern ObjectFn data_801c2aa4_slot04_0d[];
extern ObjectFn data_801c2aac_slot04_0d[];

void func_801483a4(Object *object, int a, int b);
void func_80141e5c(Object *object);
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801b2d40_slot04_0d(Object *object);
void func_801b2e9c_slot04_0d(Object *object);
void func_801b33c0_slot04_0d(Object *object);

void func_801b28f8_slot04_0d(Object *obj) {
    int v;

    obj->field_249 = 2;
    if (obj->field_4c >= 0) {
        if ((u8)obj->field_3a == 0) {
            obj->field_45 = 1;
            *(s32 *)&obj->field_14 += obj->field_50;
            obj->field_50 += obj->field_58;
        }
        v = obj->field_4c;
        if (obj->field_0b == 0) {
            v = -v;
        }
        *(s32 *)&obj->field_10 = v + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
    } else {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b29a4_slot04_0d(Object *obj) {
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    if (obj->field_50 > 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b29fc_slot04_0d(Object *obj) {
    s16 y;

    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    y = obj->field_70;
    if (obj->pos_y >= y) {
        obj->field_45 = 0;
        obj->pos_y = y;
        obj->field_14 = 0;
        obj->field_159 = 0;
        obj->field_07++;
        func_801209c4(obj);
    } else if ((u8)obj->field_3a != 0) {
        return;
    }
    func_80130efc(obj);
}

void func_801b2a9c_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2ae0_slot04_0d(Object *obj) {
    data_801c2a90_slot04_0d[obj->field_07](obj);
}

void func_801b2b20_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3b);
}

void func_801b2b88_slot04_0d(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        int a = -1;

        obj->field_07++;
        if (obj->field_4b != 0) {
            a = 1;
        }
        obj->field_165 = a;
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1a, 0x44);
    }
}

void func_801b2bfc_slot04_0d(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        u8 i = 0;

        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        ((Slot04bObj *)obj)->field_27b = data_801c2824_slot04_0d[i];
        obj->field_4c = 0x30000;
        obj->field_54 = 0;
    }
}

void func_801b2c8c_slot04_0d(Object *obj) {
    u16 a;

    func_80130efc(obj);
    a = obj->field_3a;
    if ((a & 0xff) != 0) {
        obj->field_3a = a & 0xff00;
        obj->field_07++;
        obj->field_54 = -0x4000;
    }
    func_801b2d40_slot04_0d(obj);
}

void func_801b2ce8_slot04_0d(Object *obj) {
    func_80130efc(obj);
    if ((s16)obj->field_3a >= 0) {
        func_801b2d40_slot04_0d(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801b2d40_slot04_0d(Object *obj) {
    int v = obj->field_4c;

    if (v >= 0) {
        if (obj->field_0b == 0) {
            v = -v;
        }
        *(s32 *)&obj->field_10 = v + *(s32 *)&obj->field_10;
        obj->field_4c = obj->field_4c + obj->field_54;
    }
}

void func_801b2d8c_slot04_0d(Object *obj) {
    data_801c2aa4_slot04_0d[obj->field_07](obj);
}

void func_801b2dcc_slot04_0d(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_157 = 0;
    func_801307e0(obj, 0x1c);
}

void func_801b2e1c_slot04_0d(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2e5c_slot04_0d(Object *obj) {
    if (obj->field_129 != 0) {
        func_801b33c0_slot04_0d(obj);
    } else {
        func_801b2e9c_slot04_0d(obj);
    }
}

void func_801b2e9c_slot04_0d(Object *obj) {
    data_801c2aac_slot04_0d[obj->field_07](obj);
}

void func_801b2edc_slot04_0d(Object *obj) {
    obj->field_160 = 0xb4;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1cd = 0;
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
    func_80141e5c(obj);
}
