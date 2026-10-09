/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_80142ba0(Object *object);

u8 func_801b1d38_slot04_16(Object *obj) {
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

u8 func_801b1dc4_slot04_16(Object *obj) {
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

u8 func_801b1e64_slot04_16(Object *obj) {
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

u8 func_801b1ed0_slot04_16(Object *obj) {
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

u8 func_801b1fc4_slot04_16(Object *obj) {
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

u8 func_801b2034_slot04_16(Object *obj) {
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

u8 func_801b20a4_slot04_16(Object *obj) {
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
