/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cbdc(Object *object, u8 index, u8 arg);
int func_8013cc48(Object *object, u8 index, u8 arg);
int func_8013cac8(Object *object, u8 index, u8 arg);
u8 func_801417cc(Object *object);
int func_80141e34(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_80142ba0(Object *object);

/* functions of other units of this module */
int func_801b10cc_slot04_0d(Object *obj);
int func_801b1048_slot04_0d(Object *obj);

int func_801b0c1c_slot04_0d(Object *obj);
int func_801b0f38_slot04_0d(Object *obj);
int func_801b0fc0_slot04_0d(Object *obj);
int func_801b0e58_slot04_0d(Object *obj);
int func_801b0b9c_slot04_0d(Object *obj);
int func_801b0ec8_slot04_0d(Object *obj);
int func_801b0d08_slot04_0d(Object *obj);
int func_801b0db0_slot04_0d(Object *obj);
void func_801b0d48_slot04_0d(Object *obj);
void func_801b0df0_slot04_0d(Object *obj);

void func_801b09d8_slot04_0d(Object *obj) {
    if (func_8013d210(obj) && (u8)func_801b10cc_slot04_0d(obj)) return;
    if ((u8)func_8013d1a8(obj) && (u8)func_801b0c1c_slot04_0d(obj)) return;
    if ((u8)func_8013cb70(obj, 0, 0x18) && (u8)func_801b0f38_slot04_0d(obj)) return;
    if ((u8)func_8013cbdc(obj, 1, 0x15) && (u8)func_801b0fc0_slot04_0d(obj)) return;
    if ((u8)func_8013cc48(obj, 2, 0x17) && (u8)func_801b1048_slot04_0d(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 4) && (u8)func_801b0e58_slot04_0d(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0) && (u8)func_801b0b9c_slot04_0d(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 3) && (u8)func_801b0ec8_slot04_0d(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 0xd) && (u8)func_801b0d08_slot04_0d(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xe)) func_801b0db0_slot04_0d(obj);
}

int func_801b0b9c_slot04_0d(Object *obj) {
    if (obj->field_14c != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 0;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}

int func_801b0c1c_slot04_0d(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 != 0) {
        if (obj->field_7e != 0) return 0;
        if (!(u8)func_801418bc(obj)) return 0;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 1;
    } else {
        if (!(u8)func_80141e34(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_0b = obj->field_158;
    }
    return 1;
}

int func_801b0d08_slot04_0d(Object *obj) {
    if ((u8)func_80141b28(obj)) {
        func_801b0d48_slot04_0d(obj);
        return 1;
    }
    return 0;
}

void func_801b0d48_slot04_0d(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_0b = t;
    obj->field_15a = 2;
    obj->field_27b = 0x1c;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x18;
    func_801307e0(obj, 0x2e);
}

int func_801b0db0_slot04_0d(Object *obj) {
    if ((u8)func_80141b28(obj)) {
        func_801b0df0_slot04_0d(obj);
        return 1;
    }
    return 0;
}

void func_801b0df0_slot04_0d(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_0b = t;
    obj->field_15a = 3;
    obj->field_27b = 0x20;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x1c;
    func_801307e0(obj, 0x2e);
}

int func_801b0e58_slot04_0d(Object *obj) {
    u8 one;
    int r;

    if (func_801417cc(obj)) {
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = one;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int func_801b0ec8_slot04_0d(Object *obj) {
    u8 one;
    int r;

    if (func_801417cc(obj)) {
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = one;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int func_801b0f38_slot04_0d(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
    return 1;
}

int func_801b0fc0_slot04_0d(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 7;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142778(obj);
    return 1;
}
