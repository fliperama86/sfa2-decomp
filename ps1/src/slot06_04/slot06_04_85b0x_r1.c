/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern Slot06Tile data_801f03c8_slot06_04[2][0x145];
extern Slot06Tile data_801f6ce0_slot06_04[2][0x130];

void func_801e85b0_slot06_04(void) {
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
            r = (Slot06Tile *)((u8 *)data_801f03c8_slot06_04 + i * 0x238c + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x145);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f6ce0_slot06_04[0];
            sp = &t->sprt;
            func_80136d1c((Tx *)(i * 0x2140 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x2140) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x130);
        i++;
    } while (i < 2);
}
