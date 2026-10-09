/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e95b8_slot06_13[];
extern Slot06Tile data_801eec68_slot06_13[2][0xc3];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);

void func_801e8540_slot06_13(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
}

void func_801e859c_slot06_13(Slot06Layer *layer) {
    layer->field_50 = data_801e95b8_slot06_13;
    layer->field_54 = data_801e95b8_slot06_13;
    layer->field_1e = 0x581c;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e85c8_slot06_13(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eec68_slot06_13[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xc3);
        i++;
    } while (i < 2);
}
