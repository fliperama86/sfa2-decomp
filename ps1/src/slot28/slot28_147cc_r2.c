/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern Object *data_80051c24_slot28[];
extern ObjectRef data_80051c28_slot28;
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd84_slot28[];
void func_80025114_slot28(Object *o);
void func_800250e4_slot28(Object *o, int arg);

void func_80024a8c_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        func_80130768(data_80051c4c_slot28.p, 0, data_8004bd74_slot28);
        p = data_80051c50_slot28[0];
        p->pos_y = -0x60;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xa6;
            p->field_03 = 0xb;
            data_80051c28_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0x90;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 0, data_8004bd84_slot28);
            data_80051c24_slot28[0] = p;
        }
        func_800250e4_slot28(obj, 1);
        func_80128370();
    }
}
