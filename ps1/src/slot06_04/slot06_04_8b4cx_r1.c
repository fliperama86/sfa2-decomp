/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8b4c_slot06_04(Slot06Layer *layer) {
    int i;
    int d;

    d = layer->field_60;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->field_08;
    d /= 64;
    layer->field_74 = d;
    layer->field_68 = 0;
    layer->field_6c = 0;
    for (i = 0; i < 0x28; i++) {
        layer->field_6c += d;
    }
    for (i = 0; i < 8; i++) {
        layer->field_68 -= d;
    }
}
