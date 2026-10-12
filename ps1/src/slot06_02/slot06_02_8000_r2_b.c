/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f1f48_slot06_02[2][0x11a];
extern u16 data_801e9e74_slot06_02[];
extern u16 data_801eaf18_slot06_02[];

void func_801e87dc_slot06_02(Slot06Layer *layer);

void func_801e87dc_slot06_02(Slot06Layer *layer) {
    layer->flags_bits = data_801e9e74_slot06_02[layer->table_index];
}

void func_801e8800_slot06_02(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eaf18_slot06_02;
    layer->data_table = data_801eaf18_slot06_02;
    layer->packed_param = 0x5816;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xca;
    layer->rect_right = 0xd8;
    layer->field_1c = 0x48;
}

void func_801e8844_slot06_02(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f1f48_slot06_02[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x11a);
        i++;
    } while (i < 2);
}
