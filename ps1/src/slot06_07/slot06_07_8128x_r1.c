/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_80190544;

void func_80136180(Sprite *sprite);

void func_801e8128_slot06_07(Slot06Layer *layer) {
    Slot06Layer *l2;
    int d;
    s16 e;

    if (layer->mode == 0) {
        func_80136180((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = *(s32 *)&l2->bound_x_c;
        d -= *(s32 *)&l2->bound_x_a;
        d += data_80190544;
        d += d / 4;
        d += *(s32 *)&layer->bound_x_a;
        d >>= 16;
        e = layer->pos_x_int + d;
        layer->field_22 = e;
        e = l2->field_26;
        e -= l2->field_0e;
        e += e / 4;
        e += layer->field_0e;
        e = layer->pos_y_int + e;
        layer->field_26 = e;
    }
}
