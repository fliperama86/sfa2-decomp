/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebf90_slot06_00[];
extern Slot06TileW data_801f1790_slot06_00[2][98];

void func_80136d70(Tx *tx);

void func_801e8a3c_slot06_00(Slot06Layer *layer) {
    layer->field_50 = data_801ebf90_slot06_00;
    layer->field_54 = data_801ebf90_slot06_00;
    layer->field_1e = 0x5c18;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8a68_slot06_00(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 98; i++) {
            p = data_801f1790_slot06_00[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
