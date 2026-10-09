/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8003b500_slot28[];
extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b530_slot28[];
extern ObjectRef data_80051a50_slot28;
extern ObjectRef data_80051a54_slot28;
extern ObjectRef data_80051a40_slot28;
void func_8001d578_slot28(Object *obj);
void func_8001d5b4_slot28(void);

void func_8001cb7c_slot28(Object *obj) {
    int one;
    Object *p;
    Object *b;
    data_8018f5a0->field_60 = 0xb4;
    data_8018f5a0->field_50 += 1;
    func_80151020(0x305);
    one = 1;
    func_8001d5b4_slot28();
    func_801280f0();
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001d578_slot28(p);
        data_80051a54_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_8003b500_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001d578_slot28(p);
        data_80051a40_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 1, data_8003b504_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    b = p;
    if (b != 0) {
        func_8001d578_slot28(b);
        data_80051a50_slot28.p = b;
        b->field_7c = 0x1e0;
        b->pos_x = 0x28;
        b->field_7a = 0;
        b->field_0d = 0;
        b->field_03 = one;
        b->field_09 = 0;
        b->field_50 = 0xb8;
        b->field_01 = one;
        func_80130768(b, 0, data_8003b530_slot28);
    }
    func_8012818c();
}
