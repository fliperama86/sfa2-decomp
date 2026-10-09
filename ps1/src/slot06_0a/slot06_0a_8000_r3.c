/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec11c_slot06_0a[];
extern Slot06TileW data_801f3eb0_slot06_0a[2][0x3d];

void func_801369b0(Cam *cam);
void func_80136d70(Tx *tx);
void func_801e8a0c_slot06_0a(Slot06Layer *layer);

void func_801e89a4_slot06_0a(void) {
}

void func_801e89ac_slot06_0a(void) {
}

void func_801e89b4_slot06_0a(u8 *p) {
}

void func_801e89bc_slot06_0a(void) {
}

void func_801e89c4_slot06_0a(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801369b0((Cam *)layer);
    } else if (layer->field_04 == 1) {
        func_801e8a0c_slot06_0a(layer);
    }
}

void func_801e8a0c_slot06_0a(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    layer->field_4a ^= 0x200;
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d /= 2;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 2;
    e += layer->field_0e;
    e += layer->field_3a;
    layer->field_26 = e;
}

void func_801e8aa0_slot06_0a(Slot06Layer *layer) {
    layer->field_50 = data_801ec11c_slot06_0a;
    layer->field_54 = data_801ec11c_slot06_0a;
    layer->field_1e = 0x6018;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8acc_slot06_0a(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x3d; i++) {
            p = data_801f3eb0_slot06_0a[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
