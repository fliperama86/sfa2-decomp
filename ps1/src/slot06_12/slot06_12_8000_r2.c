/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void save_cam_prev_pos_and_set_table_ptr(Cam *cam);
void update_cam_and_store_scroll_y_diff(void);
void func_801e8644_slot06_12(Slot06Layer *layer);
void func_801e869c_slot06_12(Slot06Layer *layer);
void load_layer_timer_and_flags_from_tables_slot06_12(Slot06Layer *layer);

void update_layer_modes_then_cam_and_scroll_slot06_12(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_801e8644_slot06_12(layer);
    } else if (layer->mode == 1) {
        func_801e869c_slot06_12(layer);
    }
    save_cam_prev_pos_and_set_table_ptr((Cam *)layer);
    update_cam_and_store_scroll_y_diff();
}

void func_801e8644_slot06_12(Slot06Layer *layer) {
    init_sprite_position_wide((Sprite *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->table_index = 0;
        layer->mode++;
        load_layer_timer_and_flags_from_tables_slot06_12(layer);
    }
}
