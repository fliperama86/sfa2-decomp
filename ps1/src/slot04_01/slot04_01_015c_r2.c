/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0360_slot04_01(Object *obj);
void func_801b0690_slot04_01(Object *obj);
void func_801b03a0_slot04_01(Object *obj);
void func_801b0458_slot04_01(Object *obj);

void func_801b0320_slot04_01(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b0360_slot04_01(obj);
    } else {
        func_801b0690_slot04_01(obj);
    }
}

void func_801b0360_slot04_01(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b03a0_slot04_01(obj);
    } else {
        func_801b0458_slot04_01(obj);
    }
}
