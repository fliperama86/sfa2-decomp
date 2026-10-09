/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec398_slot06_06[];
extern Poly28 data_801f3618_slot06_06[2][0xb0];

void func_8015c09c(void *prim);

void func_801e92b0_slot06_06(Slot06Layer *layer) {
    layer->field_50 = data_801ec398_slot06_06;
    layer->field_54 = data_801ec398_slot06_06;
    layer->field_1e = 0x6000;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e92dc_slot06_06(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f3618_slot06_06[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0xb0);
        i++;
    } while (i < 2);
}
