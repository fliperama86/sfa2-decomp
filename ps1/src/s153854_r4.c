/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153ed8(Actor *a) {
    Object *o = a->target;
    a->score_value = a->field_14 = table_80181380[a->number_value];
    a->score_value = o->field_170;
}

void func_80153f0c(Actor *a) {
    table_80180384[a->side]->buf[6] = table_80180348[a->owner->field_bc >> 1];
    func_801519b4(table_80180384[a->side]);
}

void func_80153f88(void) {
    data_80180ed8();
}
