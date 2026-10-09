/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ed398_slot06_06[];
extern Slot06TileW data_801f6d18_slot06_06[2][0x5b];

void func_80136d70(Tx *tx);

void func_801e9440_slot06_06(Slot06Layer *layer) {
    layer->field_50 = data_801ed398_slot06_06;
    layer->field_54 = data_801ed398_slot06_06;
    layer->field_1e = 0x6400;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e946c_slot06_06(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x5b; i++) {
            p = data_801f6d18_slot06_06[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
