/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801edd2c_slot06_12[];
extern Slot06TileW data_801f4cf0_slot06_12[2][0x4e];

void func_80136d70(Tx *tx);

void func_801e8d20_slot06_12(Slot06Layer *layer);

void func_801e8c44_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    int pos;
    int t;

    l2 = (Slot06Layer *)data_801aa5d4;
    if (game_state.field_65 == 0) {
        layer->field_70 -= 0x4000;
    }
    layer->field_70 &= 0x3ffffff;
    d = ((u16 *)&layer->field_60)[1];
    d -= l2->field_0a;
    t = d >> 1;
    t += d >> 3;
    t += layer->field_70 >> 16;
    t += 0x80;
    t &= 0xff;
    t -= 0x80;
    layer->field_74 = t << 16;
}

void func_801e8cbc_slot06_12(void) {
}

void func_801e8cc4_slot06_12(Slot06Layer *layer) {
}

void func_801e8ccc_slot06_12(void) {
}

void func_801e8cd4_slot06_12(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801369d4((Cam *)layer, 0x1c0, 0x10);
    } else if (layer->field_04 == 1) {
        func_801e8d20_slot06_12(layer);
    }
}

void func_801e8d20_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 -= 0x2000;
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d /= 4;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d /= 2;
    d += layer->field_0e;
    d += layer->field_3a;
    layer->field_26 = d;
}

void func_801e8dcc_slot06_12(Slot06Layer *layer) {
    layer->field_50 = data_801edd2c_slot06_12;
    layer->field_54 = data_801edd2c_slot06_12;
    layer->field_1e = 0x5c08;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8df8_slot06_12(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x4e; i++) {
            p = data_801f4cf0_slot06_12[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
