/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_layer_sprite_then_set_flags_from_table_slot06_02(Slot06Layer *layer);
void update_layer_table_index_and_camera_slot06_02(Slot06Layer *layer);
void set_layer_flags_from_table_slot06_02(Slot06Layer *layer);
void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);

void update_layer_modes_then_cam_and_scroll_slot06_02(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_sprite_then_set_flags_from_table_slot06_02(layer);
    } else if (layer->mode == 1) {
        update_layer_table_index_and_camera_slot06_02(layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void init_layer_sprite_then_set_flags_from_table_slot06_02(Slot06Layer *layer) {
    init_sprite_position_wide((Sprite *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->table_index = 0;
        layer->mode++;
        set_layer_flags_from_table_slot06_02(layer);
    }
}
