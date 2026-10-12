/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ed9b4_slot06_01[];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_by_mode_then_cam_and_scroll_slot06_01(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void init_layer_tile_map_with_rect_slot06_01(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ed9b4_slot06_01;
    layer->data_table = data_801ed9b4_slot06_01;
    layer->packed_param = 0x5c0e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xc8;
    layer->rect_right = 0xd8;
    layer->field_1c = 0x58;
}
