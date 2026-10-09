/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c0c_slot28;
extern Object *data_80051c10_slot28[];
extern ObjectRef data_80051c1c_slot28;
extern ObjectRef data_80051c20_slot28;
extern SequenceStep *data_8004a084_slot28[];
extern SequenceStep *data_8004a088_slot28[];
extern SequenceStep *data_8004a094_slot28[];
extern SequenceStep *data_8004a09c_slot28[];
extern SequenceStep *data_8004a0d0_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
void func_80023c10_slot28(Object *obj);
void func_80023c4c_slot28(void);

void func_80023110_slot28(Object *obj) {
    Object *o;
    int one;
    data_8018f5a0->field_60 = 0x12c;
    data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
    func_80151020(0x405);
    func_80023c4c_slot28();
    one = 1;
    func_801280f0();
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        data_80051c0c_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_8004a088_slot28);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        data_80051c20_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_8004a084_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        data_80051c10_slot28[0] = o;
        o->pos_x = 8;
        o->pos_y = -0x60;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 5;
        func_80130768(o, 0, data_8004a094_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        data_80051c1c_slot28.p = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_50 = 0xb8;
        o->field_09 = 0;
        func_80130768(o, 0, data_8004a0d0_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        o->pos_x = 0xdb;
        o->pos_y = 0x5d;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 7, data_8004a09c_slot28);
        data_80051be4_slot28[0] = o;
        o->field_09 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        o->pos_x = 0xd8;
        o->pos_y = 0x69;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 8, data_8004a09c_slot28);
        data_80051be4_slot28[1] = o;
        o->field_09 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        o->pos_x = 0xce;
        o->pos_y = 0x96;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 9, data_8004a09c_slot28);
        data_80051be4_slot28[2] = o;
        o->field_09 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0x1d;
        o->field_09 = 2;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0x93;
        o->pos_y = 0x69;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_80048d70_slot28;
        o->field_9c = data_800490f4_slot28;
        o->field_00 = one;
        o->field_03 = one;
        o->field_01 = one;
        o->field_0d = 0;
        o->box_tables = (BoxTables *)data_8004a09c_slot28;
        data_80051be4_slot28[3] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80023c10_slot28(o);
        o->pos_x = 0xb0;
        o->pos_y = 0xa1;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 4;
        func_80130768(o, 0, data_8004a09c_slot28);
        data_80051be4_slot28[4] = o;
        o->field_09 = 3;
    }
    func_8012818c();
}
