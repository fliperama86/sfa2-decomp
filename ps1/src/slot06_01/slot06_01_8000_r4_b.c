/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ee9b4_slot06_01[];
extern Slot06TileW data_801f94bc_slot06_01[2][0x43];

void func_80136d70(Tx *tx);

void func_801e934c_slot06_01(Slot06Layer *layer) {
    layer->field_50 = data_801ee9b4_slot06_01;
    layer->field_54 = data_801ee9b4_slot06_01;
    layer->field_1e = 0x6818;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9378_slot06_01(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x43; i++) {
            p = data_801f94bc_slot06_01[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
