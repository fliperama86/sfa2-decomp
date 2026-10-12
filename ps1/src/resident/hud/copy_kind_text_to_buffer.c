/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void copy_kind_text_to_buffer(Object *o) {
    TextObj *t = table_80180f9c[o->side];
    int i;
    for (i = 0; i < 7; i++) {
        t->buf[i] = table_80180fa4[o->kind * 7 + i];
    }
    add_item_to_table(t);
}
