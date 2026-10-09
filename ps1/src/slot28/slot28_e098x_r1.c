/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051a80_slot28;
extern Object *data_80051a84_slot28[];
extern Object *data_80051a58_slot28[];
extern SequenceStep *data_8003d2d0_slot28[];
extern SequenceStep *data_8003d2d8_slot28[];
extern SequenceStep *data_8003d2e0_slot28[];
void func_8001e2a0_slot28(Object *o);
void func_8001e2ec_slot28(Object *obj, int arg);
void func_8001e31c_slot28(Object *o);
void func_8001e37c_slot28(void);

void func_8001e098_slot28(Object *obj) {
    Object *p;
    HudState *g;
    if (obj->field_f0 == 0) {
        g = data_8018f5a0;
        g->field_60 = 0x258;
        g->field_52 = g->field_52 + 1;
        func_8001e2a0_slot28(obj);
        func_8001e37c_slot28();
        func_80130768(data_80051a80_slot28.p, 1, data_8003d2d0_slot28);
        p = data_80051a84_slot28[0];
        p->pos_x = 0x48;
        p->field_01 = 0;
        p->pos_y = 0x20;
        func_80130768(p, 1, data_8003d2d8_slot28);
        p->field_01 = 1;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8001e31c_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 4, data_8003d2e0_slot28);
            data_80051a58_slot28[0] = p;
        }
        func_80128370();
        func_8001e2ec_slot28(obj, 2);
    }
}
