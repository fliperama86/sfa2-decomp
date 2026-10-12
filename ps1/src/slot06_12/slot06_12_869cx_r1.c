/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_layer_timer_and_flags_from_tables_slot06_12(Slot06Layer *layer);

void func_801e869c_slot06_12(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        layer->countdown_timer -= 1;
        if (layer->countdown_timer == 0) {
            layer->table_index = (layer->table_index + 1) & 7;
            load_layer_timer_and_flags_from_tables_slot06_12(layer);
        }
    }
    if (layer->field_00 != 0 && game_state.side_bits_2 == 0) {
        update_camera_x_from_player_span((Sprite *)layer);
        update_cam_y_toward_players((Sprite *)layer);
    }
}
