/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea5b8_slot06_13[];
extern Slot06TileW data_801f1710_slot06_13[2][78];

void func_80136d70(Tx *tx);
void func_801e894c_slot06_13(Slot06Layer *layer);
void func_801e89a0_slot06_13(Slot06Layer *layer);

void func_801e88e4_slot06_13(void) {
}

void func_801e88ec_slot06_13(void) {
}

void func_801e88f4_slot06_13(u8 *p) {
}

void func_801e88fc_slot06_13(void) {
}

void func_801e8904_slot06_13(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e894c_slot06_13(layer);
    } else if (layer->field_04 == 1) {
        func_801e89a0_slot06_13(layer);
    }
}

void func_801e894c_slot06_13(Slot06Layer *layer) {
    func_80136a2c((Cam *)layer, 0x140, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_30 = 6;
        layer->field_04++;
    }
}

void func_801e89a0_slot06_13(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (game_state.field_65 == 0) {
        layer->field_30 -= 1;
        if (layer->field_30 == 0) {
            layer->field_30 = 6;
            layer->field_4a = (layer->field_4a + 0x200) & 0x600;
        }
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d /= 4;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 4;
    e += layer->field_0e;
    e += layer->field_3a;
    layer->field_26 = e;
}

void func_801e8a6c_slot06_13(Slot06Layer *layer) {
    layer->field_50 = data_801ea5b8_slot06_13;
    layer->field_54 = data_801ea5b8_slot06_13;
    layer->field_1e = 0x600c;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8a98_slot06_13(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 78; i++) {
            p = data_801f1710_slot06_13[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
