/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec398_slot06_06[];
extern Slot06Tile data_801f25b0_slot06_06[2][0x4b];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e8668_slot06_06(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e86cc_slot06_06(Slot06Layer *layer) {
    layer->field_50 = data_801ec398_slot06_06;
    layer->field_54 = data_801ec398_slot06_06;
    layer->field_1e = 0x6000;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
    layer->field_4c = 0xb0;
    layer->field_4e = 0xd8;
    layer->field_1c = 0x80;
}

void func_801e8710_slot06_06(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f25b0_slot06_06[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x4b);
        i++;
    } while (i < 2);
}
