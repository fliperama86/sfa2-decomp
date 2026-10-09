/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);

int func_801b2200_slot04_10(Object *obj) {
    if (func_80141b28(obj) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_17b = 1;
        obj->field_12a = 4;
        obj->field_27b = 0x1a;
        obj->field_157 = 0;
        obj->field_6b = 0;
        obj->field_0b = obj->field_158;
        ref_other.p = obj->other;
        ref_other.p->field_6b = 0x16;
        func_801307e0(obj, 0x46);
    }
    return 0;
}
