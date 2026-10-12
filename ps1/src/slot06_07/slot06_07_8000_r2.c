/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebed0_slot06_07[];
extern Slot06Tile data_801ef09c_slot06_07[2][0x133];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_by_mode_then_cam_and_scroll_slot06_07(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void func_801e8610_slot06_07(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebed0_slot06_07;
    layer->data_table = data_801ebed0_slot06_07;
    layer->packed_param = 0x580e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xd0;
    layer->rect_right = 0xd8;
    layer->field_1c = 0xa8;
}

void init_slot06_07_tiles_gray_slot06_07(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ef09c_slot06_07[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x133);
        i++;
    } while (i < 2);
}
