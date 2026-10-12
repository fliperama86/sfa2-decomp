/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eb938_slot06_05[];
extern Slot06TileW data_801f30a4_slot06_05[2][0x68];

void init_draw_mode_prim_4(Tx *tx);

void set_layer_tile_map_param_6014_width_scale_slot06_05(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb938_slot06_05;
    layer->data_table = data_801eb938_slot06_05;
    layer->packed_param = 0x6014;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_104_gray_32x32_sprite_prims_slot06_05(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x68; i++) {
            p = data_801f30a4_slot06_05[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
