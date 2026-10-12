/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e9f18_slot06_02[];

void init_first_layer_tile_map_slot06_02(Slot06Layer *layer);

void init_first_layer_tile_map_slot06_02(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e9f18_slot06_02;
    layer->data_table = data_801e9f18_slot06_02;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
