/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801eef88_slot06_0c[2][0x109];
extern Slot06Tile data_801f5bc0_slot06_0c[200][2];

void func_801e85f4_slot06_0c(void) {
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
            r = &data_801eef88_slot06_0c[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x109);
        i++;
    } while (i < 2);
    i = 0;
    base = (u8 *)data_801f5bc0_slot06_0c;
    sp0 = &data_801f5bc0_slot06_0c[0][0].sprt;
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
        } while (j < 200);
        i++;
    } while (i < 2);
}
