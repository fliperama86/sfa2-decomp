/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
int func_80141b28(Object *object);
void func_801b128c_slot04_04(Object *obj);
void func_80146998(Object *object);

u8 func_801b11b0_slot04_04(Object *obj) {
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

int func_801b123c_slot04_04(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b128c_slot04_04(obj);
    }
    return r;
}

void func_801b128c_slot04_04(Object *obj) {
    ref_other.p = obj->other;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 8;
    obj->field_159 = 1;
    obj->field_157 = 0;
    obj->field_12a = 2;
    obj->field_6b = 0;
    obj->field_0b = obj->field_158;
    ref_other.p->field_6b = 0x14;
    ref_other.p->field_27b = 0x18;
    func_80146998(obj);
    func_801307e0(obj, 0x1f);
}
