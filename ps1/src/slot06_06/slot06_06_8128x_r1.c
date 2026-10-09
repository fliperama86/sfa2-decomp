/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e822c_slot06_06(Slot06Layer *layer);

void func_801e8128_slot06_06(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    int h;

    if (layer->field_04 == 0) {
        func_801e822c_slot06_06(layer);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        if (d != 0) {
            h = (s16)l2->field_4e;
            h -= (s16)l2->field_1c;
            d = d * (h - 8) / h;
        }
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
