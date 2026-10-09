/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
u8 func_801417cc(Object *object);
int func_80141e34(Object *object);
/* function of another unit of this module */
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b10ac_slot04_14(Object *obj);
u8 func_80141cec(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);

u8 func_801b0ce0_slot04_14(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            if (obj->field_45 == 0) {
                if (func_80141e34(obj) & 0xff) {
                    r = func_80141788(obj);
                    if (r) {
                        func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
                        obj->field_15a = 0xf;
                        obj->field_0b = obj->field_158;
                    }
                }
            } else if (obj->field_7e == 0) {
                r = func_801418bc(obj);
                if (r) {
                    func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
                    obj->field_15a = 0xf;
                }
            }
        }
    }
    return r;
}

u8 func_801b0ddc_slot04_14(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xe;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

u8 func_801b0e48_slot04_14(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xd;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

u8 func_801b0eb4_slot04_14(Object *obj) {
    u8 r;

    if (obj->field_7e == 0 && obj->field_177 == 0) return;
    r = func_801417cc(obj);
    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xc;
        obj->field_0b = obj->field_158;
    }
    return r;
}

u8 func_801b0f3c_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x90) {
        r = func_80141cec(obj);
        if (r) {
            func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 0xb;
            obj->field_12a = 4;
            obj->field_255 = 4;
            obj->field_0b = obj->field_158;
        }
    }
    return r;
}

u8 func_801b0fc8_slot04_14(Object *obj) {
    u8 r = func_801417cc(obj);
    u8 t;

    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        t = obj->field_158;
        obj->field_15a = 8;
        obj->field_0b = t;
        obj->field_48 = t ^ 1;
        func_801b10ac_slot04_14(obj);
    }
    return r;
}

u8 func_801b103c_slot04_14(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 8;
        obj->field_48 = obj->field_0b = obj->field_158;
        func_801b10ac_slot04_14(obj);
    }
    return r;
}

void func_801b10ac_slot04_14(Object *obj) {
    u16 t;

    t = obj->field_134 | obj->field_136;
    obj->field_129 = 0;
    obj->field_12a = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->field_129 = obj->field_129 + 2;
    }
}

u8 func_801b10f0_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        r = func_80141788(obj);
        if (r) {
            func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 7;
            obj->field_159 = 1;
            obj->field_0b = obj->field_158;
            func_80142718(obj);
        }
    }
    return r;
}

u8 func_801b1180_slot04_14(Object *obj) {
    int r = 1;

    if (obj->field_cd == 0) {
        r = func_80141b28(obj);
        if (!(u8)r) return 0;
    }
    func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_27b = 0x1e;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    func_801307e0(obj, 0x1a);
    return r;
}

void func_801b1238_slot04_14(Object *obj) {
    func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_27b = 0x1e;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    func_801307e0(obj, 0x1a);
}

u8 func_801b12b0_slot04_14(Object *obj) {
    int r = 1;

    if (obj->field_cd == 0) {
        r = func_80141b28(obj);
        if (!(u8)r) return 0;
    }
    func_801b62a8_slot04_14(obj, 1, 0, 7, 5);
    obj->field_159 = 1;
    obj->field_27b = 0x19;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    func_801307e0(obj, 0x31);
    return r;
}

void func_801b1368_slot04_14(Object *obj) {
    func_801b62a8_slot04_14(obj, 1, 0, 7, 5);
    obj->field_159 = 1;
    obj->field_27b = 0x19;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    func_801307e0(obj, 0x31);
}

u8 func_801b13e0_slot04_14(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}
