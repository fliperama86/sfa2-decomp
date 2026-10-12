/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ee410_slot06_03[];
extern Slot06TileW data_801f62c0_slot06_03[2][0x75];

void init_draw_mode_prim_4(Tx *tx);

void set_layer_tile_map_and_scale_f_slot06_03(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ee410_slot06_03;
    layer->data_table = data_801ee410_slot06_03;
    layer->packed_param = 0x6404;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_gray_32x32_sprite_prims_slot06_03(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x75; i++) {
            p = data_801f62c0_slot06_03[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
