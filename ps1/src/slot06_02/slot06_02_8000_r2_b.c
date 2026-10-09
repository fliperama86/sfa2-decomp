/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f1f48_slot06_02[2][0x11a];
extern u16 data_801e9e74_slot06_02[];
extern u16 data_801eaf18_slot06_02[];

void func_801e87dc_slot06_02(Slot06Layer *layer);

void func_801e87dc_slot06_02(Slot06Layer *layer) {
    layer->field_4a = data_801e9e74_slot06_02[layer->field_88];
}

void func_801e8800_slot06_02(Slot06Layer *layer) {
    layer->field_50 = data_801eaf18_slot06_02;
    layer->field_54 = data_801eaf18_slot06_02;
    layer->field_1e = 0x5816;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
    layer->field_4c = 0xca;
    layer->field_4e = 0xd8;
    layer->field_1c = 0x48;
}

void func_801e8844_slot06_02(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f1f48_slot06_02[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x11a);
        i++;
    } while (i < 2);
}
