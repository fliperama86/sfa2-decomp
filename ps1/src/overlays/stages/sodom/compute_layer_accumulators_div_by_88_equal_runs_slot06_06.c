/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void compute_layer_accumulators_div_by_88_equal_runs_slot06_06(Slot06Layer *layer) {
    int i;
    int n;

    n = layer->field_60;
    n -= *(s32 *)&((Slot06Layer *)main_layer)->bound_x_a;
    layer->neg_accum = 0;
    layer->pos_accum = 0;
    n /= 88;
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
    } while (i < 0x28);
}
