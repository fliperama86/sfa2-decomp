/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec410_slot06_03[];
extern Slot06Tile data_801f8004_slot06_03[0x30][2];

void init_layer_map_and_sprite_colors_slot06_03(Slot06Layer *layer) {
    int i = 0;
    int j;
    Slot06Sprt16 *sp;
    u8 *base = (u8 *)data_801f8004_slot06_03;
    Slot06Sprt16 *sp0 = &data_801f8004_slot06_03[0][0].sprt;
    u8 c;

    layer->tile_map_ptr = data_801ec410_slot06_03;
    layer->data_table = data_801ec410_slot06_03;
    layer->packed_param = 0x581a;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xc5;
    layer->rect_right = 0xc5;
    layer->field_1c = 0x62;
    do {
        j = 0;
        c = 0x80;
        do {
            init_draw_mode_prim_3((Tx *)(j * 0x38 + (i * 0x1c + (u32)base)));
            sp = (Slot06Sprt16 *)((i * 0x1c + j * 0x38) + (u32)sp0);
            sp->color_r = c;
            sp->color_g = c;
            sp->color_b = c;
            j++;
        } while (j < 0x30);
        i++;
    } while (i < 2);
}
