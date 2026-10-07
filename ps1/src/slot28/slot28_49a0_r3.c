/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_800518c8_slot28;
extern Object *data_800518bc_slot28[];
extern SequenceStep *data_8002bddc_slot28[];

void func_8001581c_slot28(Object *obj, int arg);
void func_800150e8_slot28(Object *obj);
void func_801282d4(void);
void func_80128370(void);

void func_80014f80_slot28(Object *obj) {
    if (((Slot28Obj *)data_800518c8_slot28.p)->field_3a < 0) {
        data_8018f5a0->field_60 = 0xc0;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_8014f4d4(6, 1);
    }
}

void func_80014fd8_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t < 0) {
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}

void func_80015028_slot28(Object *obj) {
    Object *o;
    HudState *h;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0xf0;
        h->field_52 = h->field_52 + 1;
        func_800150e8_slot28(obj);
        o = data_800518bc_slot28[0];
        o->pos_x = 0x50;
        o->pos_y = -0x5f;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_09 = 2;
        o->field_0d = 0;
        o->field_01 = 1;
        func_80130768(o, 0, data_8002bddc_slot28);
        func_8001581c_slot28(obj, 4);
        func_8014f4d4(1, 0x402);
        func_80128370();
    }
}
