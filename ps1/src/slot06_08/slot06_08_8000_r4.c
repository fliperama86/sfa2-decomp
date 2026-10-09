/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eb6d8_slot06_08[];
extern u16 data_801ec6d8_slot06_08[];
extern Poly28 data_801f2504_slot06_08[2][0x108];
extern Slot06TileW data_801f7784_slot06_08[2][0x2f];

extern u16 data_80190542;

void func_8015c09c(void *prim);
void func_801369b0(Cam *cam);
void func_80136d70(Tx *tx);

void func_801e9424_slot06_08(Slot06Layer *layer) {
    layer->field_50 = data_801eb6d8_slot06_08;
    layer->field_54 = data_801eb6d8_slot06_08;
    layer->field_1e = 0x5c10;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9450_slot06_08(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f2504_slot06_08[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x108);
        i++;
    } while (i < 2);
}

void func_801e94f0_slot06_08(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_801369b0((Cam *)layer);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += data_80190542;
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

void func_801e9590_slot06_08(Slot06Layer *layer) {
    layer->field_50 = data_801ec6d8_slot06_08;
    layer->field_54 = data_801ec6d8_slot06_08;
    layer->field_1e = 0x6404;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e95bc_slot06_08(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x2f; i++) {
            p = data_801f7784_slot06_08[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
