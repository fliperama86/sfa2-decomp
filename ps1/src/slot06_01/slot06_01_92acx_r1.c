/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80190542;

void func_801369b0(Cam *cam);

void func_801e92ac_slot06_01(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (layer->field_04 == 0) {
        func_801369b0((Cam *)layer);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += data_80190542;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        d = l2->field_26;
        d -= l2->field_0e;
        d += layer->field_0e;
        d += layer->field_3a;
        layer->field_26 = d;
    }
}
