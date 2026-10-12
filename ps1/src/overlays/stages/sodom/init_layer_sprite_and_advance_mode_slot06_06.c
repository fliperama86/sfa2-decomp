/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801ea398_slot06_06[];

void init_layer_tile_map_5800_conditional_slot06_06(Slot06Layer *layer);
void init_layer_sprite_and_advance_mode_slot06_06(Slot06Layer *layer);

void init_layer_sprite_and_advance_mode_slot06_06(Slot06Layer *layer) {
    init_sprite_position((Sprite *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->mode++;
    }
}

void init_layer_tile_map_5800_conditional_slot06_06(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea398_slot06_06;
    if (game_state.field_0a == 6 || game_state.skip_flag != 0) {
        layer->tile_map_ptr = data_801ea398_slot06_06 + 0x800;
    }
    layer->data_table = layer->tile_map_ptr;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
