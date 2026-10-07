/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051c5c_slot28;
void func_801280f0(void);
extern SequenceStep *data_8004bda4_slot28[];
extern Object *data_80051c50_slot28[];
extern u8 data_8004ab90_slot28[];
extern u8 data_8004aed8_slot28[];
extern Object *data_80051c24_slot28[];
void func_8011f240(Slab172 *s);

void func_80025058_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051c5c_slot28.p)->field_3a < 0) {
        func_801280f0();
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}

void func_800250ac_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_8018f5a0->field_4e += 1;
    }
}

void func_800250e4_slot28(Object *obj, int arg) {
    func_80130768(data_80051c50_slot28[3], arg, data_8004bda4_slot28);
}

void func_80025114_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8004ab90_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_8004aed8_slot28;
}

void func_80025150_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051c24_slot28[i] = 0;
    }
}

void func_80025174_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051c24_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051c24_slot28[i] = 0;
        }
    }
}
