/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8002a87c_slot28[];
extern u16 data_8002a07c_slot28[];
extern u16 data_8002a47c_slot28[];
extern u8 data_80028aa0_slot28[];
extern u8 data_80028e0c_slot28[];
extern void (*data_8002ac84_slot28[])(Object *);
void func_80014854_slot28(Object *obj, int arg);

void func_80014138_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8002a87c_slot28[i];
        data_801a27e4_rows[5][i] = data_8002a87c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8002a07c_slot28[i];
        data_801a27e4_rows[7][i] = data_8002a07c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_8002a47c_slot28[i];
        data_801a27e4_rows[8][i] = data_8002a47c_slot28[i];
    }
}

void func_8001420c_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_80028aa0_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_80028e0c_slot28;
}

void func_80014248_slot28(Object *obj) {
    data_8002ac84_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}

void func_800142a0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_80014854_slot28(obj, 0);
}
