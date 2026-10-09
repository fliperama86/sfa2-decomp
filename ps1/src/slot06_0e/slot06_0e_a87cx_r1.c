/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *func_8011f32c(void);
void func_801eaa28_slot06_0e(Object *obj);

void func_801ea87c_slot06_0e(Object *obj) {
    Object *c = func_8011f32c();

    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x16;
        c->field_03 = 0;
        c->field_81 = 4;
        c->field_3c = obj;
        c->field_09 = obj->field_09;
        obj->field_34 = (u32)c;
        obj->field_46 = 0;
        obj->field_0d = 0;
        obj->field_0c = 0;
        obj->field_04 = 1;
        obj->field_0a = 1;
        obj->field_81 = 4;
        obj->field_0f = 1;
        func_801eaa28_slot06_0e(obj);
    }
}
