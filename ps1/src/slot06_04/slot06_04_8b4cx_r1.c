/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void compute_layer_step_and_accumulate_slot06_04(Slot06Layer *layer) {
    int i;
    int d;

    d = layer->field_60;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->bound_x_a;
    d /= 64;
    layer->computed_value = d;
    layer->neg_accum = 0;
    layer->pos_accum = 0;
    for (i = 0; i < 0x28; i++) {
        layer->pos_accum += d;
    }
    for (i = 0; i < 8; i++) {
        layer->neg_accum -= d;
    }
}
