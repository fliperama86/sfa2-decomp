/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80125b34(Select *a, Object *b) {
    if (a->field_1b != 3 && a->field_54 == a->field_a4)
        a->field_40 = data_8016f538[b->kind];
}
