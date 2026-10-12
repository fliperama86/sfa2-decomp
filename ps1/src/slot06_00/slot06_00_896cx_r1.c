/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e896c_slot06_00(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (layer->mode == 0) {
        func_801369d4((Cam *)layer, 0x140, 0);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d /= 4;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        d = l2->field_26;
        d -= l2->field_0e;
        d /= 4;
        d += layer->field_0e;
        d += layer->pos_y_int;
        layer->field_26 = d;
    }
}
