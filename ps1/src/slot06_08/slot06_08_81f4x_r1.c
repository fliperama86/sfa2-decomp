/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ed4f4_slot06_08[2][0xeb];
extern Slot06Tile data_801f8344_slot06_08[2][0x90];

void init_slot06_08_two_tile_groups_gray_slot06_08(void);

void init_slot06_08_two_tile_groups_gray_slot06_08(void) {
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
            r = (Slot06Tile *)((u8 *)data_801ed4f4_slot06_08 + i * 0x19b4 + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0xeb);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f8344_slot06_08[0];
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
