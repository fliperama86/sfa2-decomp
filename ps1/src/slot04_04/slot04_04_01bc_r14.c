/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1378_slot04_04(Object *obj);

int func_801b1328_slot04_04(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b1378_slot04_04(obj);
    }
    return r;
}

void func_801b1378_slot04_04(Object *obj) {
    ref_other.p = obj->other;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 9;
    obj->field_159 = 1;
    obj->field_157 = 0;
    obj->field_12a = 4;
    obj->field_6b = 0;
    obj->field_0b = obj->field_158;
    ref_other.p->field_6b = 0x14;
    ref_other.p->field_27b = 0x18;
    func_80146998(obj);
    func_801307e0(obj, 0x1e);
}
