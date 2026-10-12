/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f0d40_slot06_03[2][0x120];

void init_two_banks_288_tile_prims_gray_slot06_03(void) {
    int i;
    int j;
    Slot06Tile *r;

    i = 0;
    do {
        j = 0;
        do {
            r = &data_801f0d40_slot06_03[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x120);
        i++;
    } while (i < 2);
}
