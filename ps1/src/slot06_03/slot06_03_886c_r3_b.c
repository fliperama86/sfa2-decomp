/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ee410_slot06_03[];
extern Slot06TileW data_801f62c0_slot06_03[2][0x75];

void func_80136d70(Tx *tx);

void func_801e92b4_slot06_03(Slot06Layer *layer) {
    layer->field_50 = data_801ee410_slot06_03;
    layer->field_54 = data_801ee410_slot06_03;
    layer->field_1e = 0x6404;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e92e0_slot06_03(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x75; i++) {
            p = data_801f62c0_slot06_03[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
