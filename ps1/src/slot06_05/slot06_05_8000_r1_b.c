/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e9938_slot06_05[];

void func_801e822c_slot06_05(Slot06Layer *layer);

void func_801e822c_slot06_05(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e9938_slot06_05;
    layer->data_table = data_801e9938_slot06_05;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
