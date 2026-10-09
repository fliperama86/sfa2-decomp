/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec8ec_slot06_0e[];
extern Slot06Tile data_801f22d4_slot06_0e[2][0x7c];

void func_801e8748_slot06_0e(Slot06Layer *layer) {
    layer->field_50 = data_801ec8ec_slot06_0e;
    layer->field_54 = data_801ec8ec_slot06_0e;
    layer->field_1e = 0x5810;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
    layer->field_4c = 0xc0;
    layer->field_4e = 0xd8;
    layer->field_1c = 0x89;
}

void func_801e878c_slot06_0e(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f22d4_slot06_0e[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x7c);
        i++;
    } while (i < 2);
}
