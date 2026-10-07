/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8007a06c_slot00[])(Object *obj);
extern void (*data_8007a07c_slot00[])(Object *obj);
void func_800780f4_slot00(Object *obj);

void func_8007803c_slot00(Object *obj) {
    data_8007a06c_slot00[obj->field_04](obj);
}

void func_8007807c_slot00(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
    obj->field_67 = 0;
    obj->field_09 = 0;
    data_8007a07c_slot00[obj->field_03 >> 1](obj);
}

void func_800780d0_slot00(Object *obj) {
    obj->field_44 = 1;
    func_800780f4_slot00(obj);
}
