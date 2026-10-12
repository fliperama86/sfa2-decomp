/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec398_slot06_06[];
extern Poly28 data_801f3618_slot06_06[2][0xb0];

void func_8015c09c(void *prim);

void set_layer_tile_map_param_6000_width_scale_slot06_06(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec398_slot06_06;
    layer->data_table = data_801ec398_slot06_06;
    layer->packed_param = 0x6000;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_layer_poly_ft4_prims_slot06_06(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f3618_slot06_06[i][j];
            func_8015c09c(r);
            r->r0 = c;
            r->g0 = c;
            r->b0 = c;
            j++;
        } while (j < 0xb0);
        i++;
    } while (i < 2);
}
