/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
extern void (*data_80030664_slot28[])(Object *);
void func_80016e28_slot28(Object *obj, int arg);
extern Object *data_800518fc_slot28[];

void func_80016540_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8002e1d4_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_8002e758_slot28;
}

void func_8001657c_slot28(Object *obj) {
    data_80030664_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}

void func_800165d4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_80016e28_slot28(obj, 0);
}

void func_80016608_slot28(Object *obj) {
    if (*(s16 *)&data_800518fc_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
