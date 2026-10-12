/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e87dc_slot06_02(Slot06Layer *layer);

void func_801e8750_slot06_02(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        if (layer->table_index == 2) {
            layer->table_index = 0;
        } else {
            layer->table_index++;
        }
        func_801e87dc_slot06_02(layer);
    }
    if (layer->field_00 != 0 && game_state.side_bits_2 == 0) {
        func_801364a0((Sprite *)layer);
        func_80136744((Sprite *)layer);
    }
}
