/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_800518c8_slot28;
extern Object *data_800518bc_slot28[];
extern ObjectRef data_80051890_slot28;

void func_80014ec8_slot28(void);
void func_8001581c_slot28(Object *obj, int arg);

void func_80014e20_slot28(Object *obj) {
    if (((Slot28Obj *)data_800518c8_slot28.p)->field_3a < 0) {
        data_8018f5a0->field_60 = 0x78;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}

void func_80014e60_slot28(Object *obj) {
    HudState *h;
    int t;
    int n;
    func_80014ec8_slot28();
    h = data_8018f5a0;
    t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        n = h->field_52;
        h->field_60 = 8;
        h->field_52 = n + 1;
        data_800518bc_slot28[0]->field_01 = 0;
    }
}

void func_80014ec8_slot28(void) {
    HudState *h = data_8018f5a0;
    s16 x = h->field_60;
    Object *o = data_800518bc_slot28[0];
    int m = (x < 100) ? 3 : 0;
    if (x < 60) {
        m = 1;
    }
    o->field_01 = 1;
    if (m != 0) {
        if ((x & m) == 0) {
            o->field_01 = 0;
        }
    }
}

void func_80014f1c_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        h->field_60 = 0xf0;
        h->field_52 = h->field_52 + 1;
        data_80051890_slot28.p->field_48 = 0xff;
        func_8001581c_slot28(obj, 1);
    }
}
