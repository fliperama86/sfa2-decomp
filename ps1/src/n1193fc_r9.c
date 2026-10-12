/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_state_0_if_x_outside_margin_wide(Object *object) {
    int m = box_margin[0];
    if ((u16)(object->pos_x - m + 0x80) > 0x280) {
        object->state_0 = 3;
    }
}
