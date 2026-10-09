/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1058_slot04_0b(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_15a = 7;
    obj->field_159 = 1;
    obj->field_17b = 1;
    obj->field_12a = 4;
    obj->field_27b = 0x17;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->field_0b = obj->field_158;
    obj->other->field_6b = 0x13;
    func_80146998(obj);
    func_801307e0(obj, 0x2b);
}
