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
        layer->field_04++;
    }
}

void func_801e827c_slot06_06(Slot06Layer *layer) {
    layer->field_50 = data_801ea398_slot06_06;
    if (game_state.field_0a == 6 || game_state.field_8a != 0) {
        layer->field_50 = data_801ea398_slot06_06 + 0x800;
    }
    layer->field_54 = layer->field_50;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
