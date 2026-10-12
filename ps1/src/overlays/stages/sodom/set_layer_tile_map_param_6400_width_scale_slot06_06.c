/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ed398_slot06_06[];
extern Slot06TileW data_801f6d18_slot06_06[2][0x5b];

void init_draw_mode_prim_4(Tx *tx);

void set_layer_tile_map_param_6400_width_scale_slot06_06(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ed398_slot06_06;
    layer->data_table = data_801ed398_slot06_06;
    layer->packed_param = 0x6400;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_06(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5b; i++) {
            p = data_801f6d18_slot06_06[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
