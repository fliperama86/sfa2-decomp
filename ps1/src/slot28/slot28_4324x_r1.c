/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051878_slot28;
extern Object *data_8005187c_slot28[];
extern SequenceStep *data_80029ed0_slot28[];
extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
void func_80014854_slot28(Object *obj, int arg);

void func_80014324_slot28(Object *obj) {
    HudState *h;
    Object *o;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        h->field_60 = 0x258;
        func_80130768(data_80051878_slot28.p, 0, data_80029ed0_slot28);
        o = data_8005187c_slot28[0];
        o->pos_y = 0x20;
        func_80130768(o, 1, data_80029ed8_slot28);
        o = data_8005187c_slot28[2];
        o->pos_x = 0xc0;
        o->pos_y = 0x90;
        func_80130768(o, 1, data_80029ee4_slot28);
        func_80014854_slot28(obj, 1);
        func_80128370();
    }
}
