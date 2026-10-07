/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eadc0_slot06_11[];
extern Slot06Tile data_801efe80_slot06_11[2][0x190];
extern Slot06Tile data_801f620c_slot06_11[2][0x100];

void func_801362f4(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e8504_slot06_11(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801362f4((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e8568_slot06_11(Slot06Layer *layer) {
    layer->field_50 = data_801eadc0_slot06_11;
    layer->field_54 = data_801eadc0_slot06_11;
    layer->field_1e = 0x580e;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8594_slot06_11(void) {
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
            r = (Slot06Tile *)((u8 *)data_801efe80_slot06_11 + i * 0x2bc0 + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x190);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f620c_slot06_11[0];
    sp = &t->sprt;
    do {
        j = 0;
        do {
            func_80136d1c((Tx *)(i * 0x1c00 + (u32)(t + j)));
            q = (Slot06Sprt16 *)((j * 0x1c + i * 0x1c00) + (u32)sp);
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            j++;
        } while (j < 0x100);
        i++;
    } while (i < 2);
}
