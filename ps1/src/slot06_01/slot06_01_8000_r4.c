/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ed9b4_slot06_01[];
extern Poly28 data_801f6b7c_slot06_01[2][0x84];

void func_8015c09c(void *prim);

void func_801e91e0_slot06_01(Slot06Layer *layer) {
    layer->field_50 = data_801ed9b4_slot06_01;
    layer->field_54 = data_801ed9b4_slot06_01;
    layer->field_1e = 0x5c0e;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e920c_slot06_01(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f6b7c_slot06_01[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}
