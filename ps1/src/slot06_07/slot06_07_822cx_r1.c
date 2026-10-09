/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern Slot06Tile data_801ee22c_slot06_07[2][0x42];
extern Slot06Tile data_801f6384_slot06_07[2][0x98];

void func_801e822c_slot06_07(void);

void func_801e822c_slot06_07(void) {
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
            r = (Slot06Tile *)((u8 *)data_801ee22c_slot06_07 + i * 0x738 + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x42);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f6384_slot06_07[0];
            sp = &t->sprt;
            func_80136d1c((Tx *)(i * 0x10a0 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x10a0) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x98);
        i++;
    } while (i < 2);
}
