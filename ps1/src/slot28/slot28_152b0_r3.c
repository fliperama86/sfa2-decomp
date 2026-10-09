/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051c8c_slot28;
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern ObjectRef data_80051c9c_slot28;
extern ObjectRef data_80051ca0_slot28;
extern Object *data_80051c64_slot28[];
extern SequenceStep *data_8004dcd4_slot28[];
extern SequenceStep *data_8004dcd8_slot28[];
extern SequenceStep *data_8004dce0_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
extern SequenceStep *data_8004dd0c_slot28[];
extern SequenceStep *data_8004dd1c_slot28[];
Block172 *func_8011f1e0(void);
void func_801280f0(void);
void func_8012818c(void);
void func_80025e5c_slot28(Object *obj);
void func_80025e98_slot28(void);

void func_80025410_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->field_60 = 0x258;
    h->field_50++;
    func_80151020(0x207);
    func_80025e98_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        data_80051c8c_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 0;
        func_80130768(b, 0, data_8004dcd8_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        data_80051ca0_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 0;
        func_80130768(b, 0, data_8004dcd4_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        data_80051c90_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->field_7a = 0x10;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 4;
        func_80130768(b, 0, data_8004dce0_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        data_80051c94_slot28.p = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        data_80051c9c_slot28.p = b;
        b->field_7c = 0x1e0;
        b->pos_x = 0x28;
        b->field_7a = 0;
        b->field_0d = 0;
        b->field_03 = one;
        b->field_50 = 0xb8;
        b->field_09 = 0;
        func_80130768(b, 0, data_8004dd0c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        b->pos_x = 0xe0;
        b->pos_y = 0x60;
        b->field_7a = 0x60;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 2;
        func_80130768(b, 0, data_8004dd1c_slot28);
        data_80051c64_slot28[0] = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        b->pos_x = 0xdb;
        b->pos_y = 0x65;
        b->field_7a = 0x60;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 2;
        func_80130768(b, 0, data_8004dd1c_slot28);
        data_80051c64_slot28[1] = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        b->pos_x = 0xd0;
        b->pos_y = 0x80;
        b->field_7a = 0x60;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 2;
        func_80130768(b, 2, data_8004dd1c_slot28);
        data_80051c64_slot28[2] = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80025e5c_slot28(b);
        b->pos_x = 0xb8;
        b->pos_y = 0x90;
        b->field_7a = 0x60;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 3;
        func_80130768(b, 0, data_8004dce8_slot28);
        data_80051c64_slot28[3] = b;
    }
    func_8012818c();
}
