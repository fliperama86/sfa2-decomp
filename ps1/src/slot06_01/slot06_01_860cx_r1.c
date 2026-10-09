/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f29dc_slot06_01[2][300];
extern Slot06Tile data_801fa57c_slot06_01[2][108];

void func_801e860c_slot06_01(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;
    u8 c;

    i = 0;
    c = 0x80;
    t = data_801fa57c_slot06_01[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801f29dc_slot06_01 + i * 0x20d0 + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 300);
        j = 0;
        do {
            func_80136d1c((Tx *)(i * 0xbd0 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0xbd0) + (u32)sp);
            q->color_r = c;
            q->color_g = c;
            q->color_b = c;
            j++;
        } while (j < 108);
        i++;
    } while (i < 2);
}
