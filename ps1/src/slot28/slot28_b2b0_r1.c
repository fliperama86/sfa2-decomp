/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051a00_slot28;
extern Object *data_80051a04_slot28[];
extern ObjectRef data_80051a08_slot28;
extern ObjectRef data_80051a10_slot28;
extern ObjectRef data_80051a14_slot28;
extern Object *data_800519d8_slot28[];
extern SequenceStep *data_80039280_slot28[];
extern SequenceStep *data_8003927c_slot28[];
extern SequenceStep *data_80039298_slot28[];
extern SequenceStep *data_800392d0_slot28[];
extern SequenceStep *data_8003929c_slot28[];

void func_8001c460_slot28(Object *obj);
void func_8001c49c_slot28(void);

void func_8001b2b0_slot28(Object *obj) {
    Object *p;
    int one;
    int two;
    data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
    func_8001c49c_slot28();
    func_801280f0();
    one = 1;
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        data_80051a00_slot28.p = p;
        p->pos_y = -8;
        p->pos_x = 0;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 2, data_80039280_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        data_80051a14_slot28.p = p;
        p->pos_y = -8;
        p->pos_x = 0;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_8003927c_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        data_80051a04_slot28[0] = p;
        p->pos_x = 0x50;
        p->pos_y = 0x20;
        p->field_7a = 0x10;
        p->field_01 = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        data_80051a08_slot28.p = p;
        p->pos_x = 0x50;
        p->pos_y = 0x20;
        p->field_7a = 0x20;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 5;
        func_80130768(p, 0, data_80039298_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        data_80051a10_slot28.p = p;
        p->field_7c = 0x1e0;
        p->pos_x = 0x28;
        p->field_7a = 0;
        p->field_0d = 0;
        p->field_03 = one;
        p->field_50 = 0xb8;
        p->field_09 = 0;
        func_80130768(p, 0, data_800392d0_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_02 = 0xa6;
        p->field_00 = one;
        p->field_03 = 8;
        data_800519d8_slot28[2] = p;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        p->pos_x = 0xe0;
        p->pos_y = 0x80;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        two = 2;
        p->field_0d = 0;
        p->field_09 = two;
        func_80130768(p, 0, data_8003929c_slot28);
        data_800519d8_slot28[0] = p;
        p->field_09 = two;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001c460_slot28(p);
        p->pos_x = 0xc0;
        p->pos_y = 0x90;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 2;
        func_80130768(p, 0xc, data_8003929c_slot28);
        data_800519d8_slot28[1] = p;
        p->field_09 = 3;
    }
    func_80151020(0x403);
    func_8012818c();
}
