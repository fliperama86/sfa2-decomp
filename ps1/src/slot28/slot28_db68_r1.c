/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051a80_slot28;
extern Object *data_80051a84_slot28[];
extern ObjectRef data_80051a90_slot28;
extern ObjectRef data_80051a94_slot28;
extern Object *data_80051a58_slot28[];
extern SequenceStep *data_8003d2cc_slot28[];
extern SequenceStep *data_8003d2d0_slot28[];
extern SequenceStep *data_8003d2d8_slot28[];
extern SequenceStep *data_8003d2e0_slot28[];
extern SequenceStep *data_8003d2f4_slot28[];
Block172 *func_8011f1e0(void);
void func_801280f0(void);
void func_8012818c(void);
void func_8001e31c_slot28(Object *o);
void func_8001e358_slot28(void);

void func_8001db68_slot28(Object *obj) {
    HudState *g = data_8018f5a0;
    Object *p;
    int one;
    g->field_60 = 0x258;
    g->field_50 += 1;
    func_8001e358_slot28();
    func_801280f0();
    one = 1;
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        data_80051a80_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_8003d2d0_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        data_80051a94_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_8003d2cc_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        data_80051a84_slot28[0] = p;
        p->pos_x = 0x58;
        p->pos_y = 0x20;
        p->field_7a = 0x10;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 5;
        func_80130768(p, 0, data_8003d2d8_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        data_80051a90_slot28.p = p;
        p->field_7c = 0x1e0;
        p->pos_x = 0x28;
        p->field_7a = 0;
        p->field_0d = 0;
        p->field_03 = one;
        p->field_50 = 0xb8;
        p->field_09 = 0;
        func_80130768(p, 0, data_8003d2f4_slot28);
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        p->pos_x = 0xb8;
        p->pos_y = 0x50;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 2;
        func_80130768(p, 0, data_8003d2e0_slot28);
        data_80051a58_slot28[0] = p;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        p->pos_x = 0xc8;
        p->pos_y = 0x58;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 2;
        func_80130768(p, 1, data_8003d2e0_slot28);
        data_80051a58_slot28[1] = p;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        p->pos_x = 0xd8;
        p->pos_y = 0x68;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 2;
        func_80130768(p, 2, data_8003d2e0_slot28);
        data_80051a58_slot28[2] = p;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8001e31c_slot28(p);
        p->pos_x = 0xee;
        p->pos_y = 0x78;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 2;
        func_80130768(p, 3, data_8003d2e0_slot28);
        data_80051a58_slot28[3] = p;
    }
    func_80151020(0x302);
    func_8012818c();
}
