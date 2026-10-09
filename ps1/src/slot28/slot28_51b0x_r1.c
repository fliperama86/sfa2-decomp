/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern Object *data_800518bc_slot28[];
extern SequenceStep *data_8002bddc_slot28[];
extern SequenceStep *data_8002bde4_slot28[];
void func_80014d58_slot28(Object *obj);

void func_800151b0_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int x;
    int y;
    int one;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        x = 0xf0;
        h->field_60 = x;
        h->field_52 = h->field_52 + 1;
        p = data_800518bc_slot28[0];
        func_80130768(p, 1, data_8002bddc_slot28);
        one = 1;
        y = 0x50;
        p->field_01 = one;
        p->pos_x = y;
        p->pos_y = 0x20;
        p->field_03 = 0;
        p->field_09 = 4;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80014d58_slot28(p);
            p->field_7a = 0x60;
            p->field_09 = one;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->pos_x = x;
            p->pos_y = y;
            func_80130768(p, 5, data_8002bde4_slot28);
            data_80051890_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80014d58_slot28(p);
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->pos_x = 0x78;
            p->field_09 = one;
            p->field_0d = 0;
            p->pos_y = 0xa0;
            func_80130768(p, 0x10, data_8002bde4_slot28);
            data_80051894_slot28.p = p;
        }
        func_80128370();
    }
}
