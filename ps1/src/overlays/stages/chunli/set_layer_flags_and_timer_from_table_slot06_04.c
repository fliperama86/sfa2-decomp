/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ed348_slot06_04[];
extern u16 data_801e9ce0_slot06_04[];
extern Slot06TileW data_801f6660_slot06_04[2][26];

void init_draw_mode_prim_4(Tx *tx);
void set_layer_flags_and_timer_from_table_slot06_04(Slot06Layer *layer);

void set_layer_flags_and_timer_from_table_slot06_04(Slot06Layer *layer) {
    layer->flags_bits = data_801e9ce0_slot06_04[layer->table_index];
    layer->countdown_timer = (s16)data_801e9ce0_slot06_04[layer->table_index + 1];
}

void set_layer_tile_map_param_640c_width_scale_slot06_04(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ed348_slot06_04;
    layer->data_table = data_801ed348_slot06_04;
    layer->packed_param = 0x640c;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_two_banks_of_26_tile_prims_slot06_04(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 26; i++) {
            p = data_801f6660_slot06_04[k] + i;
            init_draw_mode_prim_4((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
