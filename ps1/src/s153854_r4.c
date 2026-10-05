/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153ed8(Actor *a) {
    Object *o = a->field_1c;
    a->field_10 = a->field_14 = table_80181380[a->field_09];
    a->field_10 = o->field_170;
}

void func_80153f0c(Actor *a) {
    table_80180384[a->field_01]->buf[6] = table_80180348[a->field_18->field_bc >> 1];
    func_801519b4(table_80180384[a->field_01]);
}

void func_80153f88(void) {
    data_80180ed8();
}
