/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b02f8_slot04_03(Object *obj);
void func_801b0894_slot04_03(Object *obj);
void func_801b0338_slot04_03(Object *obj);
void func_801b0494_slot04_03(Object *obj);

void func_801b02b8_slot04_03(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b02f8_slot04_03(obj);
    } else {
        func_801b0894_slot04_03(obj);
    }
}

void func_801b02f8_slot04_03(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b0338_slot04_03(obj);
    } else {
        func_801b0494_slot04_03(obj);
    }
}
