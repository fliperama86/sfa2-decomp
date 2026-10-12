/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_layer_flags_from_table_slot06_02(Slot06Layer *layer);

void update_layer_table_index_and_camera_slot06_02(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        if (layer->table_index == 2) {
            layer->table_index = 0;
        } else {
            layer->table_index++;
        }
        set_layer_flags_from_table_slot06_02(layer);
    }
    if (layer->field_00 != 0 && game_state.side_bits_2 == 0) {
        update_camera_x_from_player_span((Sprite *)layer);
        update_cam_y_toward_players((Sprite *)layer);
    }
}
