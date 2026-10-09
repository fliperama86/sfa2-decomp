/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ec3b4_slot06_05[2][0x7d];
extern Slot06Tile data_801f4aa4_slot06_05[2][0x70];

void func_801e8258_slot06_05(void);

void func_801e8258_slot06_05(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;
    i = 0;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801ec3b4_slot06_05 + i * 0xdac + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x7d);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f4aa4_slot06_05[0];
            sp = &t->sprt;
            func_80136d1c((Tx *)(i * 0xc40 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0xc40) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x70);
        i++;
    } while (i < 2);
}
