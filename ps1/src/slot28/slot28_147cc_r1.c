/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern ObjectRef data_80051c5c_slot28;
extern ObjectRef data_80051c60_slot28;
extern SequenceStep *data_8004bd70_slot28[];
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd7c_slot28[];
extern SequenceStep *data_8004bda4_slot28[];
void func_80025114_slot28(Object *o);
void func_80025150_slot28(void);

void func_800247cc_slot28(Object *obj) {
    HudState *g = data_8018f5a0;
    Object *p;
    int one;
    g->field_60 = 0x258;
    g->field_50 += 1;
    func_80151020(0x404);
    func_80025150_slot28();
    one = 1;
    func_801280f0();
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_80025114_slot28(p);
        data_80051c4c_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 0;
        func_80130768(p, 1, data_8004bd74_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_80025114_slot28(p);
        data_80051c60_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 0;
        func_80130768(p, 0, data_8004bd70_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_80025114_slot28(p);
        data_80051c50_slot28[0] = p;
        p->pos_x = 0x58;
        p->pos_y = 0x20;
        p->field_7a = 0x10;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 5;
        func_80130768(p, 0, data_8004bd7c_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_80025114_slot28(p);
        data_80051c5c_slot28.p = p;
        p->field_7c = 0x1e0;
        p->pos_x = 0x28;
        p->field_7a = 0;
        p->field_0d = 0;
        p->field_03 = one;
        p->field_50 = 0xb8;
        p->field_09 = 0;
        func_80130768(p, 0, data_8004bda4_slot28);
    }
    func_8012818c();
}
