/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ed9b4_slot06_01[];
extern Poly28 data_801f6b7c_slot06_01[2][0x84];

void func_8015c09c(void *prim);

void set_layer_tile_map_and_scale_a_slot06_01(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ed9b4_slot06_01;
    layer->data_table = data_801ed9b4_slot06_01;
    layer->packed_param = 0x5c0e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_poly_ft4_buffers_gray_slot06_01(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f6b7c_slot06_01[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}
