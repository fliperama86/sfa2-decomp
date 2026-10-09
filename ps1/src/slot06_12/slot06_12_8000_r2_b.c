/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801e9ea8_slot06_12[];
extern s16 data_801e9eaa_slot06_12[];
extern u16 data_801eb52c_slot06_12[];
extern Slot06Tile data_801f11e0_slot06_12[2][0x10e];

void func_801e872c_slot06_12(Slot06Layer *layer);

void func_801e872c_slot06_12(Slot06Layer *layer) {
    s16 k;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    k = layer->field_88 * 2;
    layer->field_30 = data_801e9ea8_slot06_12[k];
    layer->field_4a = data_801e9eaa_slot06_12[k];
}

void func_801e8778_slot06_12(Slot06Layer *layer) {
    layer->field_50 = data_801eb52c_slot06_12;
    layer->field_54 = data_801eb52c_slot06_12;
    layer->field_1e = 0x580e;
    layer->field_58 = 0x200;
    layer->field_5c = 0x100;
    layer->field_4c = 0xc8;
    layer->field_4e = 0xd8;
    layer->field_1c = 0x58;
}

void func_801e87bc_slot06_12(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f11e0_slot06_12[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x10e);
        i++;
    } while (i < 2);
}
