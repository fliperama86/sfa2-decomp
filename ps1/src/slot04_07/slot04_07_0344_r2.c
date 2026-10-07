/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b058c_slot04_07(Object *obj);
void func_801b0b98_slot04_07(Object *obj);
void func_801b05cc_slot04_07(Object *obj);
void func_801b0958_slot04_07(Object *obj);

void func_801b054c_slot04_07(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b058c_slot04_07(obj);
    } else {
        func_801b0b98_slot04_07(obj);
    }
}

void func_801b058c_slot04_07(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b05cc_slot04_07(obj);
    } else {
        func_801b0958_slot04_07(obj);
    }
}
