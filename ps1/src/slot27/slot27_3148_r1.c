/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026a7c_slot27[])(Object *);

void func_80013148_slot27(Object *obj) {
    data_80026a7c_slot27[obj->field_06](obj);
    obj->field_01 = 0;
}
