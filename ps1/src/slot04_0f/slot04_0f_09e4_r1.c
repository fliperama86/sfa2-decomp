/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c59f8_slot04_0f[];
extern s32 data_801c5a00_slot04_0f[];
extern ObjectFn data_801c5a0c_slot04_0f[];
extern s32 data_801c5a18_slot04_0f[];

void func_80130dc0(Object *object);
void func_80131468(Object *object);

/* functions of other units of this module */
void func_801b4a3c_slot04_0f(Object *obj);

void func_801b0cac_slot04_0f(Object *obj);
void func_801b0d90_slot04_0f(Object *obj);

void func_801b09e4_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int i;

    o->field_159 = 1;
    obj->field_337 = 0;
    o->field_07++;
    if (o->field_130 & 0x2000) {
        func_80130ec0(o);
        func_80141f28(o, o->field_12a >> 1);
        i = o->field_12a >> 2;
        o->field_248 = data_801c59f8_slot04_0f[i];
        o->field_29b = data_801c59f8_slot04_0f[i + 4];
        func_801307e0(o, (o->field_12a >> 1) + 0x23);
    } else {
        o->field_07 = 2;
        obj->field_337 = 0xff;
        func_80130dc0(o);
    }
}

void func_801b0aa8_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0ac8_slot04_0f(Object *obj) {
    s32 x;
    s32 y = -0x10000;

    obj->field_07++;
    obj->field_58 = 0;
    obj->field_50 = 0;
    x = data_801c5a00_slot04_0f[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        x = -x;
        y = 0x10000;
    }
    obj->field_4c = x;
    obj->field_54 = y;
    func_80130efc(obj);
}

void func_801b0b30_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_07++;
        func_80120554(obj, obj->side, 0x324);
    }
    func_80130efc(obj);
}

void func_801b0b80_slot04_0f(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c == 0) {
        obj->field_07++;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x36);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0bf0_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0c34_slot04_0f(Object *obj) {
    obj->field_07 = 3;
    obj->field_159 = 1;
    obj->field_128 = 4;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_129 == 0) {
        func_801b0cac_slot04_0f(obj);
    } else {
        func_801b0d90_slot04_0f(obj);
    }
}

void func_801b0cac_slot04_0f(Object *obj) {
    int a = 0xc;

    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (!(obj->field_130 & 0x1000)) {
        if (obj->field_130 & 0x4000) {
            if (obj->field_12a == 4) {
                if (obj->pos_y < obj->field_70 - 0x38) {
                    obj->field_04 = 1;
                    obj->field_05 = 0;
                    obj->field_06 = 5;
                    obj->field_07 = 0;
                    func_80141f28(obj, 2);
                    func_801307e0(obj, 0x32);
                    return;
                }
            }
        } else if (obj->field_130 & 0x2000) {
            a += 0x1a;
        }
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
    func_80141f28(obj, obj->field_12a >> 1);
}

void func_801b0d90_slot04_0f(Object *obj) {
    int a = 0xf;

    if (obj->field_48 != 0) {
        a = 0x15;
    }
    if (!(obj->field_130 & 0x1000)) {
        if (obj->field_130 & 0x4000) {
            if (obj->field_70 - 0x38 >= obj->pos_y) {
                obj->field_04 = 1;
                obj->field_05 = 0;
                obj->field_06 = 5;
                obj->field_07 = 0;
                func_80141f28(obj, 2);
                func_801307e0(obj, (obj->field_12a >> 1) + 0x33);
                return;
            }
        } else if (obj->field_130 & 0x2000) {
            a += 0x1a;
        }
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
    func_80141f28(obj, obj->field_12a >> 1);
}

void func_801b0e68_slot04_0f(Object *obj) {
    data_801c5a0c_slot04_0f[obj->field_07](obj);
}

void func_801b0ea8_slot04_0f(Object *obj) {
    int i = 0;
    s32 x;

    obj->field_07++;
    if (obj->field_129 != 0) {
        i = obj->field_12a;
    }
    x = data_801c5a18_slot04_0f[i];
    obj->field_50 = data_801c5a18_slot04_0f[i + 1];
    if (obj->field_0b == 0) {
        x = -x;
    }
    obj->field_4c = x;
}
