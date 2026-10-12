/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea398_slot06_06[];

void func_801e827c_slot06_06(Slot06Layer *layer);
void func_801e822c_slot06_06(Slot06Layer *layer);

void func_801e822c_slot06_06(Slot06Layer *layer) {
    func_801361fc((Sprite *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->mode++;
    }
}

void func_801e827c_slot06_06(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea398_slot06_06;
    if (game_state.field_0a == 6 || game_state.skip_flag != 0) {
        layer->tile_map_ptr = data_801ea398_slot06_06 + 0x800;
    }
    layer->data_table = layer->tile_map_ptr;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
