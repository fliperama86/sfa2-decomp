/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_layer_accumulators_from_bound_delta_slot06_01(Slot06Layer *layer) {
    s32 d;
    int i;

    i = 0;
    d = layer->field_60;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->bound_x_a;
    layer->neg_accum = 0;
    layer->pos_accum = 0;
    d >>= 7;
    layer->computed_value = d;
    do {
        layer->pos_accum += d;
        i++;
    } while (i < 0x28);
    i = 0;
    do {
        layer->neg_accum -= d;
        i++;
    } while (i < 0x10);
}
