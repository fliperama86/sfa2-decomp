/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);

int func_801b0d60_slot04_06(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 3;
    obj->field_0b = obj->field_158;
    func_80142718(obj);
    return 1;
}

int func_801b0de4_slot04_06(Object *o) {
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
    o->field_15a = 4;
    o->field_27b = 0x1d;
    o->other->field_6b = 0x19;
    func_801307e0(o, 0x39);
    return 1;
}

void func_801b0e78_slot04_06(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_0b = t;
    obj->field_15a = 4;
    obj->field_27b = 0x1d;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_12a = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    func_801307e0(obj, 0x39);
}
