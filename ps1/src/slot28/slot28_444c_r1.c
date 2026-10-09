/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;

extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
extern Object *data_8005187c_slot28[];
void func_80014854_slot28(Object *obj, int arg);

void func_8001444c_slot28(Object *obj) {
    HudState *h;
    Object *o;
    Object *q;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52 += 1;
        o = data_8005187c_slot28[0];
        func_80130768(o, 0, data_80029ed8_slot28);
        o->pos_y = -0x60;
        o = data_8005187c_slot28[1];
        o->pos_y = -0x60;
        o = data_8005187c_slot28[2];
        q = o;
        q->pos_x = 0xc0;
        q->pos_y = 0x90;
        func_80130768(q, 2, data_80029ee4_slot28);
        func_80014854_slot28(obj, 2);
        func_80128370();
    }
}

void func_80014514_slot28(Object *obj) {
    if (obj->field_f0 == 0 && (s16)data_8005187c_slot28[3]->field_3a < 0) {
        data_8018f5a0->field_52 += 1;
        func_80130768(data_8005187c_slot28[2], 3, data_80029ee4_slot28);
        func_80014854_slot28(obj, 3);
    }
}
