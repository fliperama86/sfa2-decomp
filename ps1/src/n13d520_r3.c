/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80141248(Object *object) {
    if ((ref_other.p->field_134 & 0xfc) == 0) {
        object->field_160 = object->field_160 + 1;
    } else {
        int t = object->field_160 - 1;
        object->field_160 = t;
        if ((s16)t < 0) object->field_160 = 0;
    }
}
