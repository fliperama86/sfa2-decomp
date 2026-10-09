/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80190542;

void func_801e9300_slot06_04(Slot06Layer *layer);
void func_801e93c8_slot06_04(Slot06Layer *layer);

void func_801e9300_slot06_04(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 += -0x18000;
        layer->field_30 -= 1;
        if (layer->field_30 == 0) {
            layer->field_88 = (layer->field_88 + 2) & 0xe;
            func_801e93c8_slot06_04(layer);
        }
    }
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
