/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ecdc0_slot06_11[];
extern Slot06TileW data_801f5600_slot06_11[2][0x30];

void func_80136d70(Tx *tx);

void func_801e8924_slot06_11(void) {
}

void func_801e892c_slot06_11(void) {
}

void func_801e8934_slot06_11(u8 *p) {
}

void func_801e893c_slot06_11(void) {
}

void func_801e8944_slot06_11(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_801369d4((Cam *)layer, 0x1c0, 0x10);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e += layer->field_0e;
        e += layer->field_3a;
        layer->field_26 = e;
    }
}

void func_801e89dc_slot06_11(Slot06Layer *layer) {
    layer->field_50 = data_801ecdc0_slot06_11;
    layer->field_54 = data_801ecdc0_slot06_11;
    layer->field_1e = 0x6418;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8a08_slot06_11(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x30; i++) {
            p = data_801f5600_slot06_11[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
