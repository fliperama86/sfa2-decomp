/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8013d1a8(Object *object);
u8 func_8013d210(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013cc48(Object *object, u8 a, u8 b);
int func_8013cdc8(Object *object, u8 a, u8 b);
int func_8013cdf0(Object *object, u8 a, u8 b);
int func_8013ce5c(Object *object, u8 a, u8 b);
u8 func_801417cc(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_80142b3c(Object *object);
void func_80142ba0(Object *object);
/* functions of other units of this module */
int func_801b1414_slot04_12(Object *obj);
void func_801b1510_slot04_12(Object *obj);
u8 func_801b0de0_slot04_12(Object *obj);
u8 func_801b0e54_slot04_12(Object *obj);
u8 func_801b0ecc_slot04_12(Object *obj);
u8 func_801b0f64_slot04_12(Object *obj);
u8 func_801b0fe8_slot04_12(Object *obj);
u8 func_801b107c_slot04_12(Object *obj);
u8 func_801b111c_slot04_12(Object *obj);
u8 func_801b11b0_slot04_12(Object *obj);
u8 func_801b123c_slot04_12(Object *obj);
void func_801b128c_slot04_12(Object *obj);
u8 func_801b1328_slot04_12(Object *obj);
void func_801b0bf0_slot04_12(Object *obj);

void func_801b0bf0_slot04_12(Object *obj) {
    if ((u8)func_8013d1a8(obj) && (u8)func_801b1414_slot04_12(obj)) return;
    if ((u8)func_8013cc48(obj, 0, 0x18) && func_801b111c_slot04_12(obj)) return;
    if ((u8)func_8013ce5c(obj, 1, 6) && func_801b107c_slot04_12(obj)) return;
    if ((u8)func_8013cdf0(obj, 2, 5) && func_801b0fe8_slot04_12(obj)) return;
    if ((u8)func_8013cdf0(obj, 3, 2) && func_801b0ecc_slot04_12(obj)) return;
    if ((u8)func_8013cdc8(obj, 4, 1) && func_801b0f64_slot04_12(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0x13) && func_801b0e54_slot04_12(obj)) return;
    if ((u8)func_8013cfdc(obj, 6, 1) && func_801b0de0_slot04_12(obj)) return;
    if (func_8013d210(obj) && func_801b11b0_slot04_12(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xd) && func_801b123c_slot04_12(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0xe)) func_801b1328_slot04_12(obj);
}

u8 func_801b0de0_slot04_12(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0e54_slot04_12(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_67 = 0;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0ecc_slot04_12(Object *obj) {
    u8 r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_240 != 0) {
            return 0;
        }
    }
    if (func_801417cc(obj)) {
        r++;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 2;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

u8 func_801b0f64_slot04_12(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        func_801b1510_slot04_12(obj);
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0fe8_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
    return r;
}

u8 func_801b107c_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        func_801b1510_slot04_12(obj);
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
    return r;
}

u8 func_801b111c_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
    }
    return r;
}

u8 func_801b11b0_slot04_12(Object *obj) {
    u8 r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_177 == 0) {
            return 0;
        }
    }
    if (func_801417cc(obj)) {
        r++;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 7;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
    }
    return r;
}

u8 func_801b123c_slot04_12(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b128c_slot04_12(obj);
    }
    return r;
}
