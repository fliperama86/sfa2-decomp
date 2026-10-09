/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c62b8_slot04_0f[];
extern ObjectFn data_801c62c4_slot04_0f[];
extern ObjectFn data_801c62d0_slot04_0f[];
extern ObjectFn data_801c62d8_slot04_0f[];
extern ObjectFn data_801c62e4_slot04_0f[];
extern ObjectFn data_801c62ec_slot04_0f[];
extern ObjectFn data_801c62f8_slot04_0f[];
extern ObjectFn data_801c6300_slot04_0f[];
extern ObjectFn data_801c630c_slot04_0f[];
extern s32 data_801c6324_slot04_0f[];

void func_80130dc0(Object *obj);
void func_80131468(Object *object);
void func_801b0e68_slot04_0f(Object *obj);
void func_801b1cc4_slot04_0f(Object *obj);
void func_801b67b0_slot04_0f(Object *obj);
void func_801b5e40_slot04_0f(Object *obj);
void func_801b5e84_slot04_0f(Object *obj);
void func_801b601c_slot04_0f(Object *obj);
void func_801b6060_slot04_0f(Object *obj);
void func_801b61f4_slot04_0f(Object *obj);
void func_801b6238_slot04_0f(Object *obj);
void func_801b638c_slot04_0f(Object *obj);
void func_801b63d0_slot04_0f(Object *obj);
void func_801b66ac_slot04_0f(Object *obj);

void func_801b5dbc_slot04_0f(Object *obj) {
    data_801c62b8_slot04_0f[obj->field_128 >> 1](obj);
}

void func_801b5e00_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b601c_slot04_0f(obj);
    } else {
        func_801b5e40_slot04_0f(obj);
    }
}

void func_801b5e40_slot04_0f(Object *obj) {
    data_801c62c4_slot04_0f[obj->field_12a >> 1](obj);
}

void func_801b5e84_slot04_0f(Object *obj) {
    data_801c62d0_slot04_0f[obj->field_07](obj);
}

void func_801b5ec4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5ee4_slot04_0f(Object *obj) {
    func_801b5e84_slot04_0f(obj);
}

void func_801b5f04_slot04_0f(Object *obj) {
    u8 t;

    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_218 == 0 || !func_8013f8c4(obj, -0x14, 0x14)) {
        obj->field_159 = 1;
        if (obj->field_219 != 0) {
            t = obj->field_12a >> 1;
            func_80130ec0(obj);
            func_801307e0(obj, t + 0x1a);
        } else {
            func_80130dc0(obj);
        }
    } else {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}

void func_801b5fbc_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj)) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b601c_slot04_0f(Object *obj) {
    data_801c62d8_slot04_0f[obj->field_12a >> 1](obj);
}

void func_801b6060_slot04_0f(Object *obj) {
    data_801c62e4_slot04_0f[obj->field_07](obj);
}

void func_801b60a0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60c0_slot04_0f(Object *obj) {
    func_801b6060_slot04_0f(obj);
}

void func_801b60e0_slot04_0f(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_219 != 0) {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x1d);
    } else {
        func_80130dc0(obj);
    }
}

void func_801b6154_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj)) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b61b4_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b638c_slot04_0f(obj);
    } else {
        func_801b61f4_slot04_0f(obj);
    }
}

void func_801b61f4_slot04_0f(Object *obj) {
    data_801c62ec_slot04_0f[obj->field_12a >> 1](obj);
}

void func_801b6238_slot04_0f(Object *obj) {
    data_801c62f8_slot04_0f[obj->field_07](obj);
}

void func_801b6278_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b6298_slot04_0f(Object *obj) {
    func_801b6238_slot04_0f(obj);
}

void func_801b62b8_slot04_0f(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_219 == 0) {
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x20);
    }
}

void func_801b632c_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj)) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b638c_slot04_0f(Object *obj) {
    data_801c6300_slot04_0f[obj->field_12a >> 1](obj);
}

void func_801b63d0_slot04_0f(Object *obj) {
    data_801c630c_slot04_0f[obj->field_07](obj);
}

void func_801b6410_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6430_slot04_0f(Object *obj) {
    func_801b63d0_slot04_0f(obj);
}

void func_801b6450_slot04_0f(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_219 == 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x23);
    }
}

void func_801b64d0_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj)) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b6530_slot04_0f(Object *obj) {
    int a;
    int b;

    obj->field_50 = 0;
    obj->field_58 = 0;
    obj->field_07 = obj->field_07 + 1;
    a = data_801c6324_slot04_0f[obj->field_12a >> 1];
    b = 0xffff0000;
    if (obj->field_0b == 0) {
        a = -a;
        b = 0x10000;
    }
    obj->field_4c = a;
    obj->field_54 = b;
    func_80130efc(obj);
}

void func_801b6598_slot04_0f(Object *obj) {
    if (((Slot04bObj *)obj)->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
    } else {
        func_80130efc(obj);
    }
}

void func_801b65dc_slot04_0f(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    if (obj->field_4c == 0) {
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x36);
    } else {
        func_80130efc(obj);
    }
}

void func_801b664c_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b668c_slot04_0f(Object *obj) {
    func_801b0e68_slot04_0f(obj);
}

void func_801b66ac_slot04_0f(Object *obj) {
    if (obj->field_45 != 0 && obj->field_218 != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_48 = 0;
        obj->field_0b = obj->field_158;
        if (obj->field_129 != 0) {
            obj->field_48 = 2;
        }
        if (obj->field_21a != 0) {
            obj->field_48 = obj->field_48 + 4;
        }
        func_801b1cc4_slot04_0f(obj);
    } else {
        obj->field_20e = obj->field_20e & 0xfe;
        obj->field_211 = obj->field_211 | 0x40;
        func_801b67b0_slot04_0f(obj);
    }
}

void func_801b676c_slot04_0f(Object *obj) {
    if (obj->field_211 & 1) {
        func_801b66ac_slot04_0f(obj);
    } else {
        func_801b67b0_slot04_0f(obj);
    }
}
