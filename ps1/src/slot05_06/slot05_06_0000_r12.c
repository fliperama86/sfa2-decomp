/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);
u8 func_801417cc(Object *object);
void func_80142ba0(Object *object);

int func_801c8ee4_slot05_06(Object *o) {
    if (!(u8)func_80141b28(o)) return 0;
    o->field_04 = 1;
    o->field_05 = 0;
    o->field_07 = 0;
    o->field_157 = 0;
    o->field_12a = 0;
    o->field_6b = 0;
    o->field_06 = 7;
    o->field_159 = 1;
    o->field_0b = o->field_158;
    o->field_15a = 8;
    o->field_27b = 0x1e;
    o->other->field_6b = 0x1a;
    func_801307e0(o, 0x39);
    return 1;
}

void func_801c8f78_slot05_06(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_0b = t;
    obj->field_15a = 8;
    obj->field_27b = 0x1e;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_12a = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x1a;
    func_801307e0(obj, 0x39);
}

int func_801c8fe4_slot05_06(Object *obj) {
    if (func_801417cc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return 1;
}
