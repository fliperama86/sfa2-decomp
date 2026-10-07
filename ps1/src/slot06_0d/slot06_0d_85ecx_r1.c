/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801eef44_slot06_0d[2][0x145];
extern Slot06Tile data_801f685c_slot06_0d[];

void func_801e85ec_slot06_0d(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Sprt16 *sp;
    u8 *base;
    Slot06Sprt16 *sp0;
    u8 c;

    i = 0;
    do {
        j = 0;
        do {
            r = &data_801eef44_slot06_0d[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x145);
        i++;
    } while (i < 2);
    i = 0;
    base = (u8 *)data_801f685c_slot06_0d;
    sp0 = &data_801f685c_slot06_0d->sprt;
    c = 0x80;
    do {
        j = 0;
        do {
            func_80136d1c((Tx *)(j * 0x38 + (i * 0x1c + (u32)base)));
            sp = (Slot06Sprt16 *)((i * 0x1c + j * 0x38) + (u32)sp0);
            sp->color_r = c;
            sp->color_g = c;
            sp->color_b = c;
            j++;
        } while (j < 0x78);
        i++;
    } while (i < 2);
}
