/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea52c_slot06_12[];
extern Slot06Tile data_801ef340_slot06_12[2][0x8c];
extern Slot06Tile data_801f6070_slot06_12[2][0x100];

void func_801e8224_slot06_12(Slot06Layer *layer);
void func_801e8778_slot06_12(Slot06Layer *layer);
void func_801e8dcc_slot06_12(Slot06Layer *layer);
void func_801e8cc4_slot06_12(Slot06Layer *layer);
void func_801e8174_slot06_12(Slot06Layer *layer);
void func_801e8250_slot06_12(void);
void func_801e87bc_slot06_12(void);
void func_801e8df8_slot06_12(void);
void func_801e8ccc_slot06_12(void);
void func_801e8364_slot06_12(void);
void func_801e885c_slot06_12(void);
void func_801e8e98_slot06_12(void);
void func_801e8cbc_slot06_12(void);

void func_801e8000_slot06_12(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8364_slot06_12();
    }
    if (*flags & 4) {
        func_801e885c_slot06_12();
    }
    if (*flags & 8) {
        func_801e8e98_slot06_12();
    }
    if (*flags & 4) {
        func_801e8cbc_slot06_12();
    }
}

void func_801e8098_slot06_12(void) {
    func_801e8224_slot06_12((Slot06Layer *)data_801aa544);
    func_801e8778_slot06_12((Slot06Layer *)data_801aa5d4);
    func_801e8dcc_slot06_12((Slot06Layer *)cam_obj);
    func_801e8cc4_slot06_12((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_12(void) {
    func_801e8250_slot06_12();
    func_801e87bc_slot06_12();
    func_801e8df8_slot06_12();
    func_801e8ccc_slot06_12();
}

void func_801e8128_slot06_12(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801361a4((Sprite *)layer, 0x40, -0x10);
    } else if (layer->field_04 == 1) {
        func_801e8174_slot06_12(layer);
    }
}

void func_801e8174_slot06_12(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 -= 0x7000;
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d /= 2;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    d = l2->field_26;
    d -= l2->field_0e;
    d += d / 4;
    d += layer->field_0e;
    d += layer->field_3a;
    layer->field_26 = d;
}

void func_801e8224_slot06_12(Slot06Layer *layer) {
    layer->field_50 = data_801ea52c_slot06_12;
    layer->field_54 = data_801ea52c_slot06_12;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8250_slot06_12(void) {
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
            r = (Slot06Tile *)((u8 *)data_801ef340_slot06_12 + i * 0xf50 + j * 0x1c);
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x8c);
        i++;
    } while (i < 2);
    i = 0;
    t = data_801f6070_slot06_12[0];
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
