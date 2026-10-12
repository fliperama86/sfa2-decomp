/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801eaf90_slot06_00[];

void init_sprite_3ff_and_count_pending(Sprite *sprite);
void update_camera_if_active(Sprite *sprite);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);

void update_layer_by_mode_then_cam_slot06_00(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_3ff_and_count_pending((Sprite *)layer);
    } else if (layer->mode == 1) {
        update_camera_if_active((Sprite *)layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
}

void init_second_layer_tile_map_slot06_00(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eaf90_slot06_00;
    layer->data_table = data_801eaf90_slot06_00;
    layer->packed_param = 0x5812;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
