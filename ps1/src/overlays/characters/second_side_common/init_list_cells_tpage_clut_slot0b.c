/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

SlotCell *init_list_cells_tpage_clut_slot0b(Object *obj, SlotCell *cell, int arg, int base) {
    Slot0bList *list = (Slot0bList *)obj->sequence->data_ptr;
    u8 *p;
    s16 a2 = arg;
    int b2 = base;
    int n;
    int i;
    for (i = 0; i < 2; i++) {
        n = list->count;
        p = list->data;
        do {
            func_8015c09c(cell);
            p++;
            cell->color_r = 0x80;
            cell->color_g = 0x80;
            cell->color_b = 0x80;
            cell->tpage = a2;
            cell->clut = b2 + (*p++ << 6);
            p += 2;
            cell++;
            n--;
        } while (n > 0);
    }
    return cell;
}
