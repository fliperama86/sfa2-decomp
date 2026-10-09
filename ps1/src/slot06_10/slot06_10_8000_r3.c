/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801edc10_slot06_10[];
extern Slot06TileW data_801f50b0_slot06_10[2][0x5d];

void func_801e89c8_slot06_10(Slot06Layer *layer);
void func_801e8a1c_slot06_10(Slot06Layer *layer);
void func_80136d70(Tx *tx);

void func_801e8960_slot06_10(void) {
}

void func_801e8968_slot06_10(void) {
}

void func_801e8970_slot06_10(u8 *p) {
}

void func_801e8978_slot06_10(void) {
}

void func_801e8980_slot06_10(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e89c8_slot06_10(layer);
    } else if (layer->field_04 == 1) {
        func_801e8a1c_slot06_10(layer);
    }
}

void func_801e89c8_slot06_10(Slot06Layer *layer) {
    func_80136a2c((Cam *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_30 = 8;
        layer->field_04++;
    }
}

void func_801e8a1c_slot06_10(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (game_state.field_65 == 0) {
        layer->field_30--;
        if (layer->field_30 == 0) {
            layer->field_30 = 8;
            layer->field_4a = (layer->field_4a + 0x200) & 0x600;
        }
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d -= d / 8;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e += layer->field_0e;
    e += layer->field_3a;
    layer->field_26 = e;
}

void func_801e8ad0_slot06_10(Slot06Layer *layer) {
    layer->field_50 = data_801edc10_slot06_10;
    layer->field_54 = data_801edc10_slot06_10;
    layer->field_1e = 0x6408;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8afc_slot06_10(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5d; i++) {
            p = data_801f50b0_slot06_10[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
