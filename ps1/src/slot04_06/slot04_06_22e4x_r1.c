/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22e4_slot04_06(Object *obj) {
    obj->field_4c = 0x40000;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_50 = 0x28000;
    obj->field_07++;
    obj->field_54 = 0;
    obj->field_58 = 0xffff6000;
    obj->field_45 = 1;
    if (obj->field_4b == 0) {
        obj->field_165 = 0xff;
    } else {
        obj->field_165 = 1;
    }
}
