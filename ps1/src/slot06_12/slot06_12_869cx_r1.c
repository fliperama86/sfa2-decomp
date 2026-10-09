/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e872c_slot06_12(Slot06Layer *layer);

void func_801e869c_slot06_12(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        layer->field_30 -= 1;
        if (layer->field_30 == 0) {
            layer->field_88 = (layer->field_88 + 1) & 7;
            func_801e872c_slot06_12(layer);
        }
    }
    if (layer->field_00 != 0 && game_state.field_4b == 0) {
        func_801364a0((Sprite *)layer);
        func_80136744((Sprite *)layer);
    }
}
