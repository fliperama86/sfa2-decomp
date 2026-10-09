/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];
extern void (*data_8002c730_slot28[])(Object *);
void func_8001581c_slot28(Object *obj, int arg);

void func_80014d58_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8002acc4_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_8002b024_slot28;
}

void func_80014d94_slot28(Object *obj) {
    data_8002c730_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}

void func_80014dec_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_8001581c_slot28(obj, 0);
}
