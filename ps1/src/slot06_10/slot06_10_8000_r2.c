/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f0ed8_slot06_10[2][0x12d];
extern Slot06Tile data_801f6800_slot06_10[200][2];
extern u16 data_801ecc10_slot06_10[];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);

void update_layer_by_mode_then_cam_slot06_10(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
}

void init_layer_tile_map_fields_slot06_10(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ecc10_slot06_10;
    layer->data_table = data_801ecc10_slot06_10;
    layer->packed_param = 0x581c;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void init_gray_prims_in_two_tables_slot06_10(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;

    i = 0;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801f0ed8_slot06_10 + i * 0x20ec + j * 0x1c);
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x12d);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f6800_slot06_10[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            init_draw_mode_prim_3((Tx *)(j * 0x38 + (i * 0x1c + (u32)t)));
            q = (Slot06Sprt16 *)((j * 0x38 + i * 0x1c) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0xc8);
        i++;
    } while (i < 2);
}
