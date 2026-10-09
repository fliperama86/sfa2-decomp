/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern int data_80055f3c_slot01;
extern ObjectRef data_80055f44_slot01;
void func_80011740_slot01(void);

void func_80011224_slot01(void) {
    Object *o;
    data_8018f5a0->field_4e++;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x2b;
        o->field_03 = 1;
    }
}

void func_80011274_slot01(void) {
    Object *o;
    data_8018f5a0->field_4e++;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x30;
        o->field_03 = *(u8 *)&data_80055f3c_slot01;
        o->field_09 = 8;
        o->field_7a = 0x10;
        o->field_7c = 0x1f0;
    }
}

void func_800112e4_slot01(void) {
    Object *o;
    data_8018f5a0->field_4e++;
    func_80011740_slot01();
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x13;
        if (data_80055f3c_slot01 == 0x12) {
            o->field_03 = 0x14;
        } else {
            o->field_03 = *(u8 *)&data_80055f3c_slot01;
        }
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
    }
    data_80055f44_slot01.p = o;
}
