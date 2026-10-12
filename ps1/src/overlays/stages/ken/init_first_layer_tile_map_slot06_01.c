/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot06Tile data_801eedb4_slot06_01[2][0x113];
extern u16 data_801eb9b4_slot06_01[];

void init_first_layer_tile_map_slot06_01(Slot06Layer *layer);
void init_tile_sprites_gray_2x275_slot06_01(void);

void init_first_layer_tile_map_slot06_01(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb9b4_slot06_01;
    layer->data_table = data_801eb9b4_slot06_01;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_tile_sprites_gray_2x275_slot06_01(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eedb4_slot06_01[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x113);
        i++;
    } while (i < 2);
}
