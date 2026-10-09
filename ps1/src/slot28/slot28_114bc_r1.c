/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b94_slot28;
extern ObjectRef data_80051b9c_slot28;
extern ObjectRef data_80051ba0_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c3c_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c4c_slot28[];
extern SequenceStep *data_80045c6c_slot28[];
extern u8 data_80044620_slot28[];
extern u8 data_800449d0_slot28[];
extern SequenceStep *data_80045c58_slot28[];
void func_80021d94_slot28(Object *obj);
void func_80021dd0_slot28(void);

void func_800214bc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->field_60 = 0x258;
    h->field_50++;
    func_80151020(0x401);
    func_80021dd0_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b8c_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = one;
        func_80130768(b, 1, data_80045c40_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051ba0_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = one;
        func_80130768(b, 0, data_80045c3c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b90_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->field_7a = 0x10;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 5;
        func_80130768(b, 0, data_80045c4c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b94_slot28.p = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->field_7a = 0x20;
        b->field_7c = 0x1e0;
        b->field_01 = 0;
        b->field_0d = 0;
        b->field_09 = 4;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80021d94_slot28(b);
        data_80051b9c_slot28.p = b;
        b->field_7c = 0x1e0;
        b->pos_x = 0x28;
        b->field_7a = 0;
        b->field_0d = 0;
        b->field_03 = one;
        b->field_50 = 0xb8;
        b->field_09 = 0;
        func_80130768(b, 0, data_80045c6c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x7a;
        b->field_09 = 2;
        b->field_7a = 0x60;
        b->field_7c = 0x1e0;
        b->pos_x = 0xb8;
        b->pos_y = 0xa0;
        b->field_90 = (void *)0x80060000;
        b->field_98 = data_80044620_slot28;
        b->field_9c = data_800449d0_slot28;
        b->field_00 = one;
        b->field_03 = 0;
        b->field_01 = one;
        b->field_0d = 0;
        b->box_tables = (BoxTables *)data_80045c58_slot28;
        data_80051b64_slot28[0] = b;
    }
    func_8012818c();
}
