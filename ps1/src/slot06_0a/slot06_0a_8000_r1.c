/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e991c_slot06_0a[];

void func_801e81f8_slot06_0a(Slot06Layer *layer);
void func_801e866c_slot06_0a(Slot06Layer *layer);
void func_801e8aa0_slot06_0a(Slot06Layer *layer);
void func_801e89b4_slot06_0a(u8 *p);
void func_801e8224_slot06_0a(void);
void func_801e8698_slot06_0a(void);
void func_801e8acc_slot06_0a(void);
void func_801e89bc_slot06_0a(void);
void func_801e8330_slot06_0a(void);
void func_801e8738_slot06_0a(void);
void func_801e8b6c_slot06_0a(void);
void func_801e89ac_slot06_0a(void);

void func_801e8000_slot06_0a(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8330_slot06_0a();
    }
    if (*flags & 4) {
        func_801e8738_slot06_0a();
    }
    if (*flags & 8) {
        func_801e8b6c_slot06_0a();
    }
    if (*flags & 4) {
        func_801e89ac_slot06_0a();
    }
}

void func_801e8098_slot06_0a(void) {
    func_801e81f8_slot06_0a((Slot06Layer *)data_801aa544);
    func_801e866c_slot06_0a((Slot06Layer *)data_801aa5d4);
    func_801e8aa0_slot06_0a((Slot06Layer *)cam_obj);
    func_801e89b4_slot06_0a(data_801904d8);
}

void func_801e80f0_slot06_0a(void) {
    func_801e8224_slot06_0a();
    func_801e8698_slot06_0a();
    func_801e8acc_slot06_0a();
    func_801e89bc_slot06_0a();
}

void func_801e8128_slot06_0a(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_801361a4((Sprite *)layer, 0x40, 0);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d /= 4;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e /= 4;
        e += layer->field_0e;
        e += layer->field_3a;
        layer->field_26 = e;
    }
}

void func_801e81f8_slot06_0a(Slot06Layer *layer) {
    layer->field_50 = data_801e991c_slot06_0a;
    layer->field_54 = data_801e991c_slot06_0a;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
