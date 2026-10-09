/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051878_slot28;
extern Object *data_8005187c_slot28[];
extern ObjectRef data_8005188c_slot28;
extern SequenceStep *data_80029ecc_slot28[];
extern SequenceStep *data_80029ed0_slot28[];
extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee0_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
extern SequenceStep *data_80029ef4_slot28[];
extern HudState *data_8018f5a0;
void func_8001420c_slot28(Object *obj);

void func_80013ea0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *o;
    int one;
    h->field_50 = h->field_50 + 1;
    h->field_60 = 0x258;
    func_801280f0();
    one = 1;
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_80051878_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 0;
        func_80130768(o, 1, data_80029ed0_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_8005188c_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 0;
        func_80130768(o, 0, data_80029ecc_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_8005187c_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 0, data_80029ed8_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_8005187c_slot28[1] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 3;
        func_80130768(o, 0, data_80029ee0_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_8005187c_slot28[2] = o;
        o->pos_x = 0xc0;
        o->pos_y = 0xa0;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_80029ee4_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001420c_slot28(o);
        data_8005187c_slot28[3] = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_09 = 0;
        o->field_50 = 0xb8;
        func_80130768(o, 0, data_80029ef4_slot28);
    }
    func_80151020(0x300);
    func_8012818c();
}
