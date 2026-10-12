/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebf90_slot06_00[];
extern Slot06TileW data_801f1790_slot06_00[2][98];

void init_draw_mode_prim_4(Tx *tx);

void func_801e8a3c_slot06_00(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebf90_slot06_00;
    layer->data_table = data_801ebf90_slot06_00;
    layer->packed_param = 0x5c18;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e8a68_slot06_00(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 98; i++) {
            p = data_801f1790_slot06_00[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
