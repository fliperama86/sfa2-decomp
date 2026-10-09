/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj) {
    int t = obj->field_4c;

    if (obj->field_0b == 0) {
        t = -t;
    }
    *(s32 *)&obj->field_10 += t;
    obj->field_4c += obj->field_54;
}

void func_801b4850_slot04_06(Object *obj) {
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}

void func_801b4874_slot04_06(Object *obj, u16 *p) {
    if (obj->field_0b == 0) {
        *p = -*p;
    }
}
