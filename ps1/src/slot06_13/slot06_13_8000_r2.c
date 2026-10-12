/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e95b8_slot06_13[];
extern Slot06Tile data_801eec68_slot06_13[2][0xc3];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);

void update_layer_by_mode_then_cam_slot06_13(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
}

void init_layer_tile_map_fields_slot06_13(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e95b8_slot06_13;
    layer->data_table = data_801e95b8_slot06_13;
    layer->packed_param = 0x581c;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e85c8_slot06_13(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eec68_slot06_13[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xc3);
        i++;
    } while (i < 2);
}
