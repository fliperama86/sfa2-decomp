/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80149888(Object *object) {
    int delta;
    ref_other.p = object->field_3c;
    delta = data_8017cf30[ref_other.p->kind];
    if (ref_other.p->field_0b != 0) {
        delta = -delta;
    }
    object->pos_x = delta + object->pos_x;
}

void func_801498e0(Object *object) {
    data_8017cf6c[object->field_04](object);
}
