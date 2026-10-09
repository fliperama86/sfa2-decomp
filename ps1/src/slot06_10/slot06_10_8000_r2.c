/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801f0ed8_slot06_10[2][0x12d];
extern Slot06Tile data_801f6800_slot06_10[200][2];
extern u16 data_801ecc10_slot06_10[];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);

void func_801e8544_slot06_10(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
}

void func_801e85a0_slot06_10(Slot06Layer *layer) {
    layer->field_50 = data_801ecc10_slot06_10;
    layer->field_54 = data_801ecc10_slot06_10;
    layer->field_1e = 0x581c;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e85cc_slot06_10(void) {
    int i;
    int j;
    Slot06Tile *r;
    Slot06Tile *t;
    Slot06Sprt16 *sp;
    Slot06Sprt16 *q;

    i = 0;
    do {
        j = 0;
        do {
            r = (Slot06Tile *)((u8 *)data_801f0ed8_slot06_10 + i * 0x20ec + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x12d);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f6800_slot06_10[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            func_80136d1c((Tx *)(j * 0x38 + (i * 0x1c + (u32)t)));
            q = (Slot06Sprt16 *)((j * 0x38 + i * 0x1c) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0xc8);
        i++;
    } while (i < 2);
}
