/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06Tile data_801eca24_slot06_09[2][0xdf];
extern Slot06Tile data_801f6724_slot06_09[2][0x90];

void init_slot06_09_two_tile_groups_gray_slot06_09(void);

void init_slot06_09_two_tile_groups_gray_slot06_09(void) {
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
            r = (Slot06Tile *)((u8 *)data_801eca24_slot06_09 + i * 0x1864 + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0xdf);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f6724_slot06_09[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            init_draw_mode_prim_3((Tx *)(i * 0xfc0 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0xfc0) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x90);
        i++;
    } while (i < 2);
}
