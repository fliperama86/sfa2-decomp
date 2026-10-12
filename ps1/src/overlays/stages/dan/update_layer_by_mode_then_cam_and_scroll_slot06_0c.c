/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ecc70_slot06_0c[];

void init_sprite_wide_and_consume_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_by_mode_then_cam_and_scroll_slot06_0c(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_wide_and_consume_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void init_layer_tile_map_0c_with_bounds_slot06_0c(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ecc70_slot06_0c;
    layer->data_table = data_801ecc70_slot06_0c;
    layer->packed_param = 0x5810;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xd0;
    layer->rect_right = 0xd8;
    layer->field_1c = 0xaf;
}
