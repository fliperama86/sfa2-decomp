/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ed348_slot06_04[];
extern u16 data_801e9ce0_slot06_04[];
extern Slot06TileW data_801f6660_slot06_04[2][26];

void func_80136d70(Tx *tx);
void func_801e93c8_slot06_04(Slot06Layer *layer);

void func_801e93c8_slot06_04(Slot06Layer *layer) {
    layer->field_4a = data_801e9ce0_slot06_04[layer->field_88];
    layer->field_30 = (s16)data_801e9ce0_slot06_04[layer->field_88 + 1];
}

void func_801e9418_slot06_04(Slot06Layer *layer) {
    layer->field_50 = data_801ed348_slot06_04;
    layer->field_54 = data_801ed348_slot06_04;
    layer->field_1e = 0x640c;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9444_slot06_04(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 26; i++) {
            p = data_801f6660_slot06_04[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
