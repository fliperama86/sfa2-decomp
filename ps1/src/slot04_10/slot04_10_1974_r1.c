/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void func_80130efc(Object *object);
void func_80142b3c(Object *object);
int func_801418bc(Object *object);
u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013caf0(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
u8 func_8013cd5c(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_80141788(Object *object);
u8 func_801417cc(Object *object);
int func_80141e34(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_80142ba0(Object *object);
/* functions of other units of this module */
void func_801b1098_slot04_10(Object *obj);
int func_801b2324_slot04_10(Object *obj);
int func_801b2200_slot04_10(Object *obj);
u8 func_801b1c18_slot04_10(Object *obj);
u8 func_801b1d00_slot04_10(Object *obj);
u8 func_801b1d8c_slot04_10(Object *obj);
u8 func_801b1e2c_slot04_10(Object *obj);
u8 func_801b1e98_slot04_10(Object *obj);
u8 func_801b1f8c_slot04_10(Object *obj);
u8 func_801b1ffc_slot04_10(Object *obj);
u8 func_801b206c_slot04_10(Object *obj);
u8 func_801b20d4_slot04_10(Object *obj);

void func_801b1974_slot04_10(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    func_80130efc(obj);
    if (o->field_3a == 1) {
        if (o->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 0);
        } else {
            func_80140770(obj, 0, 5, 0xb, 0, 0, 0);
        }
        obj->field_07 = 4;
    }
}

void func_801b19f0_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801b1098_slot04_10(obj);
    }
}

void func_801b1a30_slot04_10(Object *obj) {
    if (func_8013d210(obj) && (u8)func_801b2324_slot04_10(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b1c18_slot04_10(obj)) return;
    if (func_8013cd5c(obj, 0, 0x22) && func_801b1d00_slot04_10(obj)) return;
    if ((u8)func_8013caf0(obj, 1, 0x15) && func_801b1d8c_slot04_10(obj)) return;
    if (func_8013ccb4(obj, 2, 0x21) && func_801b1e2c_slot04_10(obj)) return;
    if (func_8013ccb4(obj, 3, 0x23) && func_801b1e98_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 4) && func_801b1f8c_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0xd) && func_801b20d4_slot04_10(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 0xe) && (u8)func_801b2200_slot04_10(obj)) return;
    if ((u8)func_8013d0c8(obj) && func_801b1ffc_slot04_10(obj)) return;
    if ((u8)func_8013d0fc(obj)) func_801b206c_slot04_10(obj);
}

u8 func_801b1c18_slot04_10(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            if (obj->field_45 == 0) {
                if (func_80141e34(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->field_04 = 1;
                        obj->field_05 = 0;
                        obj->field_06 = 8;
                        obj->field_07 = 0;
                        obj->field_15a = 0;
                        obj->field_0b = obj->field_158;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->field_04 = 1;
                    obj->field_05 = 0;
                    obj->field_06 = 8;
                    obj->field_07 = 0;
                    obj->field_15a = 0;
                }
            }
        }
        return r;
    }
    return 0;
}

u8 func_801b1d00_slot04_10(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 0xb;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
        return 1;
    }
    return r;
}

u8 func_801b1d8c_slot04_10(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj) && func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 0xa;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
        return 1;
    }
    return r;
}

u8 func_801b1e2c_slot04_10(Object *obj) {
    if (!func_801417cc(obj)) {
        return 0;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 7;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}

u8 func_801b1e98_slot04_10(Object *obj) {
    u8 r = 0;

    if (*(u16 *)&obj->field_04 == 1 && obj->field_06 == 5) {
        if (obj->field_67 == 0) {
            if (func_801417cc(obj)) {
                r = 1;
                obj->field_15a = 8;
                obj->field_04 = 1;
                obj->field_05 = 0;
                obj->field_06 = 7;
                obj->field_07 = 0;
                obj->field_159 = 1;
                obj->field_0b = obj->field_158;
                func_80142ba0(obj);
                return 1;
            }
        }
    } else if (func_801417cc(obj)) {
        r++;
        obj->field_15a = 8;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b1f8c_slot04_10(Object *obj) {
    if (!func_801417cc(obj)) {
        return 0;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}

u8 func_801b1ffc_slot04_10(Object *obj) {
    if (!func_801417cc(obj)) {
        return 0;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 4;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}

u8 func_801b206c_slot04_10(Object *obj) {
    if (!func_801417cc(obj)) {
        return 0;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 5;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    return 1;
}
