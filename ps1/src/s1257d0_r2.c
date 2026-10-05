/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80125938(Select *a, Object *b, Object *c) {
    int i;
    s16 base;
    if (b->field_f1 == 0 && c->field_f1 == 0) {
        base = a->field_61 + (b->kind << 7);
        for (i = 0; i < 8; i++)
            a->field_120[i] = data_8016ea70[base + i];
    }
}
