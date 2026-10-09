/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8ba8_slot06_01(Slot06Layer *layer) {
    s32 d;
    int i;

    i = 0;
    d = layer->field_60;
    d -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->field_08;
    layer->field_68 = 0;
    layer->field_6c = 0;
    d >>= 7;
    layer->field_74 = d;
    do {
        layer->field_6c += d;
        i++;
    } while (i < 0x28);
    i = 0;
    do {
        layer->field_68 -= d;
        i++;
    } while (i < 0x10);
}
