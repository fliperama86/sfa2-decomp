/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80043870_slot28[];
extern Object *data_80051b44_slot28[];
extern u8 data_8004231c_slot28[];
extern u8 data_800426b8_slot28[];
extern Object *data_80051b18_slot28[];
void func_8011f240(Slab172 *s);

void func_80021060_slot28(Object *obj, int arg) {
    func_80130768(data_80051b44_slot28[3], arg, data_80043870_slot28);
}

void func_80021090_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8004231c_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_800426b8_slot28;
}

void func_800210cc_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051b18_slot28[i] = 0;
    }
}

void func_800210f0_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051b18_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051b18_slot28[i] = 0;
        }
    }
}
