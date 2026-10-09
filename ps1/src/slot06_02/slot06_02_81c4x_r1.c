/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80190542;

void func_801e81c4_slot06_02(Slot06Layer *layer);

void func_801e81c4_slot06_02(Slot06Layer *layer) {
    Slot06Layer *l2 = (Slot06Layer *)data_801aa5d4;
    s16 d;

    layer->field_8b--;
    d = 0;
    if (layer->field_8b == 0) {
        layer->field_8b = 3;
        d = 0x400;
    }
    layer->field_4a = d;
    d = l2->field_22;
    d -= l2->field_0a;
    d += data_80190542;
    d -= d / 16;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d -= d / 8;
    d += layer->field_0e;
    d += layer->field_3a;
    layer->field_26 = d;
}
