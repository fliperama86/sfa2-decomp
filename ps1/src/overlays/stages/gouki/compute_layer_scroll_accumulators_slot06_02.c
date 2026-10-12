/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void compute_layer_scroll_accumulators_slot06_02(Slot06Layer *layer) {
    int i;
    int n;

    n = layer->field_60;
    n -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->bound_x_a;
    layer->neg_accum = 0;
    layer->pos_accum = 0;
    n /= 144;
    layer->computed_value = n;
    i = 0;
    do {
        layer->pos_accum += n;
        i++;
    } while (i < 0x28);
    i = 0;
    do {
        layer->neg_accum -= n;
        i++;
    } while (i < 0xe);
}
