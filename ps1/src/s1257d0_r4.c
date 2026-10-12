/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80125b34(Select *a, Object *b) {
    if (a->mode != 3 && a->selected_index == a->index_limit)
        a->kind_value = data_8016f538[b->kind];
}
