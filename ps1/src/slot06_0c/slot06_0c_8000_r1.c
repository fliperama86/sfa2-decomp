/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebc70_slot06_0c[];
extern Slot06Tile data_801ee070_slot06_0c[2][0x45];
extern u16 data_80190542;

void func_801e8204_slot06_0c(Slot06Layer *layer);
void func_801e85b0_slot06_0c(Slot06Layer *layer);
void func_801e9374_slot06_0c(Slot06Layer *layer);
void func_801e91ec_slot06_0c(Slot06Layer *layer);
void func_801e8230_slot06_0c(void);
void func_801e85f4_slot06_0c(void);
void func_801e93a0_slot06_0c(void);
void func_801e9218_slot06_0c(void);
void func_801e82d0_slot06_0c(void);
void func_801e870c_slot06_0c(void);
void func_801e9448_slot06_0c(void);
void func_801e8c1c_slot06_0c(void);
void func_80136180(Sprite *sprite);

void func_801e8000_slot06_0c(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e82d0_slot06_0c();
    }
    if (*flags & 4) {
        func_801e870c_slot06_0c();
    }
    if (*flags & 8) {
        func_801e9448_slot06_0c();
    }
    if (*flags & 4) {
        func_801e8c1c_slot06_0c();
    }
}

void func_801e8098_slot06_0c(void) {
    func_801e8204_slot06_0c((Slot06Layer *)data_801aa544);
    func_801e85b0_slot06_0c((Slot06Layer *)data_801aa5d4);
    func_801e9374_slot06_0c((Slot06Layer *)cam_obj);
    func_801e91ec_slot06_0c((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_0c(void) {
    func_801e8230_slot06_0c();
    func_801e85f4_slot06_0c();
    func_801e93a0_slot06_0c();
    func_801e9218_slot06_0c();
}

void func_801e8128_slot06_0c(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_80136180((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += data_80190542;
        d -= d / 8;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e -= e / 8;
        e += layer->field_0e;
        e += layer->field_3a;
        layer->field_26 = e;
    }
}

void func_801e8204_slot06_0c(Slot06Layer *layer) {
    layer->field_50 = data_801ebc70_slot06_0c;
    layer->field_54 = data_801ebc70_slot06_0c;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8230_slot06_0c(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ee070_slot06_0c[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x45);
        i++;
    } while (i < 2);
}
