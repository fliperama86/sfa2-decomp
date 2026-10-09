/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8cb4_slot06_0e(Slot06Layer *layer) {
    int i;
    int n;

    n = layer->field_60;
    n -= *(s32 *)&((Slot06Layer *)data_801aa5d4)->field_08;
    layer->field_68 = 0;
    layer->field_6c = 0;
    n /= 79;
    layer->field_74 = n;
    i = 0;
    do {
        layer->field_6c += n;
        i++;
    } while (i < 0x28);
    i = 0;
    do {
        layer->field_68 -= n;
        i++;
    } while (i < 0x18);
}
