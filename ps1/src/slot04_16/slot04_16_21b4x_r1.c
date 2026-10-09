/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21b4_slot04_16(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 2;
    obj->field_159 = 1;
    obj->field_17b = 1;
    obj->field_12a = 4;
    obj->field_27b = 0x11;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->field_0b = obj->field_158;
    ref_other.p = obj->other;
    ref_other.p->field_6b = 0xd;
    func_801307e0(obj, 0x20);
}
