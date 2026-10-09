/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801eb5b8_slot06_13[2][0xfa];
extern u16 data_801e8db8_slot06_13[];

void func_801e81f8_slot06_13(Slot06Layer *layer);
void func_801e859c_slot06_13(Slot06Layer *layer);
void func_801e8a6c_slot06_13(Slot06Layer *layer);
void func_801e85c8_slot06_13(void);
void func_801e8a98_slot06_13(void);
void func_801e82c4_slot06_13(void);
void func_801e8668_slot06_13(void);
void func_801e8b38_slot06_13(void);
void func_801e88ec_slot06_13(void);
void func_801e88f4_slot06_13(u8 *p);
void func_801e88fc_slot06_13(void);
void func_801e8224_slot06_13(void);

void func_801e8000_slot06_13(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e82c4_slot06_13();
    }
    if (*flags & 4) {
        func_801e8668_slot06_13();
    }
    if (*flags & 8) {
        func_801e8b38_slot06_13();
    }
    if (*flags & 4) {
        func_801e88ec_slot06_13();
    }
}

void func_801e8098_slot06_13(void) {
    func_801e81f8_slot06_13((Slot06Layer *)data_801aa544);
    func_801e859c_slot06_13((Slot06Layer *)data_801aa5d4);
    func_801e8a6c_slot06_13((Slot06Layer *)cam_obj);
    func_801e88f4_slot06_13(data_801904d8);
}

void func_801e80f0_slot06_13(void) {
    func_801e8224_slot06_13();
    func_801e85c8_slot06_13();
    func_801e8a98_slot06_13();
    func_801e88fc_slot06_13();
}

void func_801e8128_slot06_13(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_801361a4((Sprite *)layer, 0x40, 0);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d /= 8;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e /= 8;
        e += layer->field_0e;
        e += layer->field_3a;
        layer->field_26 = e;
    }
}

void func_801e81f8_slot06_13(Slot06Layer *layer) {
    layer->field_50 = data_801e8db8_slot06_13;
    layer->field_54 = data_801e8db8_slot06_13;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8224_slot06_13(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eb5b8_slot06_13[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xfa);
        i++;
    } while (i < 2);
}
