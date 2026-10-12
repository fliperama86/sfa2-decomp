/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec410_slot06_03[];
extern Poly28 data_801f4c40_slot06_03[2][0x48];

void func_8015c09c(void *prim);

void set_layer_tile_map_and_scale_e_slot06_03(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec410_slot06_03;
    layer->data_table = data_801ec410_slot06_03;
    layer->packed_param = 0x581a;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_poly_ft4_banks_gray_slot06_03(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f4c40_slot06_03[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x48);
        i++;
    } while (i < 2);
}
