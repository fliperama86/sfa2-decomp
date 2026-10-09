/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec348_slot06_04[];
extern Poly28 data_801f4ae0_slot06_04[2][0x58];

void func_8015c09c(void *prim);
void func_801e92a8_slot06_04(Slot06Layer *layer);
void func_801e9300_slot06_04(Slot06Layer *layer);
void func_801e93c8_slot06_04(Slot06Layer *layer);

void func_801e9194_slot06_04(Slot06Layer *layer) {
    layer->field_50 = data_801ec348_slot06_04;
    layer->field_54 = data_801ec348_slot06_04;
    layer->field_1e = 0x580e;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e91c0_slot06_04(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f4ae0_slot06_04[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x58);
        i++;
    } while (i < 2);
}

void func_801e9260_slot06_04(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e92a8_slot06_04(layer);
    } else if (layer->field_04 == 1) {
        func_801e9300_slot06_04(layer);
    }
}

void func_801e92a8_slot06_04(Slot06Layer *layer) {
    func_80136a2c((Cam *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_88 = 0;
        layer->field_04++;
        func_801e93c8_slot06_04(layer);
    }
}
