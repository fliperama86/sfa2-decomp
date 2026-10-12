/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 data_801e9ea8_slot06_12[];
extern s16 data_801e9eaa_slot06_12[];
extern u16 data_801eb52c_slot06_12[];
extern Slot06Tile data_801f11e0_slot06_12[2][0x10e];

void load_layer_timer_and_flags_from_tables_slot06_12(Slot06Layer *layer);

void load_layer_timer_and_flags_from_tables_slot06_12(Slot06Layer *layer) {
    s16 k;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    k = layer->table_index * 2;
    layer->countdown_timer = data_801e9ea8_slot06_12[k];
    layer->flags_bits = data_801e9eaa_slot06_12[k];
}

void set_layer_tile_map_and_rect_params_slot06_12(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb52c_slot06_12;
    layer->data_table = data_801eb52c_slot06_12;
    layer->packed_param = 0x580e;
    layer->map_width = 0x200;
    layer->scale = 0x100;
    layer->field_4c = 0xc8;
    layer->rect_right = 0xd8;
    layer->field_1c = 0x58;
}

void init_tile_prims_gray_bank_slot06_12(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f11e0_slot06_12[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x10e);
        i++;
    } while (i < 2);
}
