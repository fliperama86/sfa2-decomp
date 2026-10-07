/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026940_slot27[])(Object *);
extern void (*data_80026950_slot27[])(Object *);

void func_80011cfc_slot27(Object *object) {
    data_80026940_slot27[object->field_05](object);
}

void func_80011d3c_slot27(Object *obj) {
    data_80026950_slot27[obj->field_06](obj);
    obj->field_01 = 1;
}
