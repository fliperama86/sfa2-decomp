/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void load_select_entries(Select *a, Object *b, Object *c) {
    int i;
    s16 base;
    if (b->field_f1 == 0 && c->field_f1 == 0) {
        base = a->base_offset + (b->kind << 7);
        for (i = 0; i < 8; i++)
            a->entries[i] = data_8016ea70[base + i];
    }
}
