/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebed0_slot06_07[];
extern u16 data_801eced0_slot06_07[];
extern Poly28 data_801f33c4_slot06_07[2][0x58];
extern Slot06TileW data_801f4f44_slot06_07[2][0x51];
extern u16 data_80190542;

void func_8015c09c(void *prim);
void func_80136d70(Tx *tx);

void func_801e91dc_slot06_07(Slot06Layer *layer) {
    layer->field_50 = data_801ebed0_slot06_07;
    layer->field_54 = data_801ebed0_slot06_07;
    layer->field_1e = 0x580e;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9208_slot06_07(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f33c4_slot06_07[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x58);
        i++;
    } while (i < 2);
}

void func_801e92a8_slot06_07(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_801369d4((Cam *)layer, 0x1a0, 0);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += data_80190542;
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
}

void func_801e9378_slot06_07(Slot06Layer *layer) {
    layer->field_50 = data_801eced0_slot06_07;
    layer->field_54 = data_801eced0_slot06_07;
    layer->field_1e = 0x6808;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e93a4_slot06_07(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x51; i++) {
            p = data_801f4f44_slot06_07[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
