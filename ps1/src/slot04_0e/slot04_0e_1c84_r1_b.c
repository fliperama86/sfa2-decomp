/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
u8 func_801417cc(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
void func_801b2658_slot04_0e(Object *obj);
int func_801b24a0_slot04_0e(Object *obj);
int func_801b25e8_slot04_0e(Object *obj);
int func_801b253c_slot04_0e(Object *obj);
int func_801b225c_slot04_0e(Object *obj);
int func_801b21d4_slot04_0e(Object *obj);
int func_801b22e4_slot04_0e(Object *obj);
int func_801b2348_slot04_0e(Object *obj);
int func_801b23b0_slot04_0e(Object *obj);
int func_801b2428_slot04_0e(Object *obj);

int func_801b213c_slot04_0e(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    if (((Slot04bObj *)obj)->field_1da != 0) return 0;
    obj->field_15a = 4;
    obj->field_04 = 1;
    obj->field_159 = 0;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    func_80142778(obj);
    return 1;
}

int func_801b21d4_slot04_0e(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_15a = 5;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    func_80142718(obj);
    return 1;
}

int func_801b225c_slot04_0e(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    func_80142778(obj);
    return 1;
}

int func_801b22e4_slot04_0e(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_15a = 7;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b2348_slot04_0e(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_15a = 0xc;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b23b0_slot04_0e(Object *obj) {
    if (!(u8)func_80141b28(obj)) return 0;
    obj->field_15a = 9;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b2428_slot04_0e(Object *obj) {
    if (!(u8)func_80141b28(obj)) return 0;
    obj->field_15a = 0xa;
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_159 = 0;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b24a0_slot04_0e(Object *obj) {
    if (obj->field_7e != 0 || obj->field_177 != 0) {
        if (func_801417cc(obj) != 0) {
            obj->field_15a = 8;
            obj->field_04 = 1;
            obj->field_159 = 0;
            obj->field_05 = 0;
            obj->field_06 = 7;
            obj->field_07 = 0;
            obj->field_12c = 0;
            obj->field_12d = 0;
            obj->field_12e = 0;
            obj->field_12f = 0;
            obj->field_0b = obj->field_158;
            return 1;
        }
    }
    return 0;
}

int func_801b253c_slot04_0e(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 != 0) {
        return (u8)func_801b25e8_slot04_0e(obj);
    }
    if (!func_80141788(obj)) return 0;
    obj->field_15a = 0xb;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    return 1;
}

int func_801b25e8_slot04_0e(Object *obj) {
    if (obj->field_7e != 0) return 0;
    if (!func_801418bc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_06 = 8;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 0xb;
    return 1;
}

void func_801b2658_slot04_0e(Object *object) {
    u8 side = object->field_158;
    u16 bits = object->field_134 | object->field_136;
    u8 value;
    object->field_12c = 0;
    object->field_12d = 0;
    object->field_12e = 0;
    object->field_12f = 0;
    object->field_0b = side;
    if (bits & 0xc0) {
        value = 0;
    } else if (bits & 0x30) {
        value = 2;
    } else {
        value = 4;
    }
    object->field_12a = value;
}
