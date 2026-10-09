/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);
/* functions of other units of this module */
void func_801b128c_slot04_12(Object *obj);

void func_801b128c_slot04_12(Object *obj) {
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
