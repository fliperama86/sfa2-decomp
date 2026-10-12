/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_poly_cells_from_sequence_list_slot2a(Object *obj, SlotCell *cell, s16 value) {
    SlotList *list = (SlotList *)obj->sequence->data_ptr;
    int n = list->count;
    int c = 0x80;
    u8 *p = list->entries;

    do {
        func_8015c09c(cell);
        p++;
        cell->color_r = c;
        cell->color_g = c;
        cell->color_b = c;
        cell->tpage = value;
        cell->clut = (s16)func_8015bdd4((*p++ << 4) + 0x2c0, 0x60);
        p += 2;
        cell++;
        n--;
    } while (n > 0);
}
