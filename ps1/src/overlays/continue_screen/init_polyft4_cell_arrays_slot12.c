/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SlotCell data_80029350_slot12[];
extern SlotCell data_80029b70_slot12[];

void init_polyft4_cell_arrays_slot12(void) {
    int i;

    for (i = 0; i < 0x34; i++) {
        SlotCell *p = &data_80029350_slot12[i];
        SlotCell *q = &data_80029350_slot12[i + 0x34];

        func_8015c09c(p);
        func_8015c09c(q);
        p->color_r = 0x80;
        p->color_g = 0x80;
        p->color_b = 0x80;
        q->color_r = 0x80;
        q->color_g = 0x80;
        q->color_b = 0x80;
        data_80029350_slot12[i].tpage = func_8015bd0c(0, 0, 0x3c0, 0);
        data_80029b70_slot12[i].tpage = func_8015bd0c(0, 0, 0x3c0, 0);
        data_80029350_slot12[i].clut = func_8015bdd4(0, 0x1f0);
        data_80029b70_slot12[i].clut = func_8015bdd4(0, 0x1f0);
    }
}
