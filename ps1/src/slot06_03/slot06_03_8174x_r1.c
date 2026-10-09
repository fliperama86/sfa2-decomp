/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8174_slot06_03(Slot06Layer *layer);
void func_801e8230_slot06_03(Slot06Layer *layer);
void func_801e8268_slot06_03(Slot06Layer *layer);
void func_801e830c_slot06_03(Slot06Layer *layer);

void func_801e8174_slot06_03(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (layer->field_05 == 0) {
        func_801e8230_slot06_03(layer);
    } else if (layer->field_05 == 1) {
        func_801e8268_slot06_03(layer);
    } else if (layer->field_05 == 2) {
        func_801e830c_slot06_03(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d += layer->field_0e;
    d += layer->field_3a;
    layer->field_26 = d;
}
