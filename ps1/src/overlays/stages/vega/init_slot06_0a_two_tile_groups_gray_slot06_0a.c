/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"
extern Slot06Tile data_801ed498_slot06_0a[2][0xc1];
extern Slot06Tile data_801f4df0_slot06_0a[2][0x68];

void init_slot06_0a_two_tile_groups_gray_slot06_0a(void);

void init_slot06_0a_two_tile_groups_gray_slot06_0a(void) {
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
            r = (Slot06Tile *)((u8 *)data_801ed498_slot06_0a + i * 0x151c + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0xc1);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f4df0_slot06_0a[0];
            sp = &t->sprt;
            init_draw_mode_prim_3((Tx *)(i * 0xb60 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0xb60) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x68);
        i++;
    } while (i < 2);
}
