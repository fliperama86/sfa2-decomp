/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051bcc_slot28;
extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bd4_slot28;
extern ObjectRef data_80051bdc_slot28;
extern ObjectRef data_80051be0_slot28;
extern Object *data_80051ba4_slot28[];
extern SequenceStep *data_80047bd4_slot28[];
extern SequenceStep *data_80047bd8_slot28[];
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be0_slot28[];
extern SequenceStep *data_80047be4_slot28[];
extern SequenceStep *data_80047bf8_slot28[];
extern u8 data_800469ec_slot28[];
extern u8 data_80046d34_slot28[];
Block172 *func_8011f1e0(void);
void func_801280f0(void);
void func_8012818c(void);
void func_80022ca8_slot28(void);
void func_80022c6c_slot28(Object *obj);

void func_80022218_slot28(Object *obj) {
    Object *o;
    data_8018f5a0->field_60 = 0x12c;
    data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
    func_80151020(0x504);
    func_80022ca8_slot28();
    func_801280f0();
    data_80190568 = 1;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bcc_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 1;
        func_80130768(o, 0, data_80047bd8_slot28);
        o->field_01 = 1;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051be0_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 1;
        func_80130768(o, 0, data_80047bd4_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bd0_slot28[0] = o;
        o->pos_x = 0x58;
        o->pos_y = 0x20;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 4;
        o->field_01 = 0;
        func_80130768(o, 0, data_80047bdc_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bd4_slot28.p = o;
        o->pos_x = 0x58;
        o->pos_y = -0x60;
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
        o->field_01 = 1;
        o->field_0d = 0;
        o->field_09 = 5;
        func_80130768(o, 0, data_80047be0_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bdc_slot28.p = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = 1;
        o->field_50 = 0xb8;
        o->field_09 = 0;
        func_80130768(o, 0, data_80047bf8_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0x65;
        o->field_09 = 2;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_800469ec_slot28;
        o->field_9c = data_80046d34_slot28;
        o->field_00 = 1;
        o->field_03 = 0;
        o->field_01 = 1;
        o->field_0d = 0;
        o->box_tables = (BoxTables *)data_80047be4_slot28;
        data_80051ba4_slot28[0] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0xa6;
        o->field_00 = 1;
        o->field_03 = 10;
        data_80051ba4_slot28[1] = o;
    }
    func_8012818c();
}
