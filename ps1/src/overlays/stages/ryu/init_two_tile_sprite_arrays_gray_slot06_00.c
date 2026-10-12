/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"
extern Slot06Tile data_801edc80_slot06_00[2][270];
extern Slot06Tile data_801f3050_slot06_00[2][85];

void init_two_tile_sprite_arrays_gray_slot06_00(void) {
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
            r = (Slot06Tile *)((u8 *)data_801edc80_slot06_00 + i * 0x1d88 + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x10e);
        i++;
    } while (i < 2);
    i = 0;
    do {
        j = 0;
        do {
            t = data_801f3050_slot06_00[0];
            sp = &t->sprt;
            init_draw_mode_prim_3((Tx *)(i * 0x94c + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x94c) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x55);
        i++;
    } while (i < 2);
}
