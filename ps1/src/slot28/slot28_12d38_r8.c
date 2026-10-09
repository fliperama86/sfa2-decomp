/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8004a0d0_slot28[];
extern Object *data_80051c10_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
extern Object *data_80051be4_slot28[];
void func_8011f240(Slab172 *s);
extern u16 data_8004a610_slot28[];
extern u16 data_8004a210_slot28[];

void func_80023be0_slot28(Object *obj, int arg) {
    func_80130768(data_80051c10_slot28[3], arg, data_8004a0d0_slot28);
}

void func_80023c10_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_80048d70_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_800490f4_slot28;
}

void func_80023c4c_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051be4_slot28[i] = 0;
    }
}

void func_80023c70_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051be4_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051be4_slot28[i] = 0;
        }
    }
}

void func_80023cdc_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8004a610_slot28[i];
        data_801a27e4_rows[5][i] = data_8004a610_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8004a210_slot28[i];
        data_801a27e4_rows[7][i] = data_8004a210_slot28[i];
    }
}
