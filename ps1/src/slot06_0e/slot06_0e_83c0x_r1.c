/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern Slot06Tile data_801ef554_slot06_0e[2][0xd0];
extern Slot06Tile data_801f8478_slot06_0e[2][0xd0];

void func_801e83c0_slot06_0e(void);

void func_801e83c0_slot06_0e(void) {
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
            r = (Slot06Tile *)((u8 *)data_801ef554_slot06_0e + i * 0x16c0 + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0xd0);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f8478_slot06_0e[0];
            sp = &t->sprt;
            func_80136d1c((Tx *)(i * 0x16c0 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x16c0) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0xd0);
        i++;
    } while (i < 2);
}
