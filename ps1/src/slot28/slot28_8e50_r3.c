/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353a0_slot28[])(Object *);

void func_80019310_slot28(Object *obj) {
    obj->field_04++;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    if (obj->field_03 == 4) {
        obj->field_50 = 0xfffec000;
        obj->field_58 = 0x1000;
    }
    if (obj->field_03 == 5) {
        obj->field_50 = 0xfffee000;
        obj->field_58 = 0x1000;
    }
}

void func_80019374_slot28(Object *obj) {
    data_800353a0_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}
