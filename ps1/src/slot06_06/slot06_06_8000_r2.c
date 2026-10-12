/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec398_slot06_06[];
extern Slot06Tile data_801f25b0_slot06_06[2][0x4b];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_by_mode_then_cam_and_scroll_slot06_06(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void init_layer_tile_map_6000_with_rect_slot06_06(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ec398_slot06_06;
    layer->data_table = data_801ec398_slot06_06;
    layer->packed_param = 0x6000;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xb0;
    layer->rect_right = 0xd8;
    layer->field_1c = 0x80;
}

void func_801e8710_slot06_06(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f25b0_slot06_06[i][j];
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x4b);
        i++;
    } while (i < 2);
}
