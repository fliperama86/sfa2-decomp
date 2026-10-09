/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b432c_slot04_04(Object *obj);
void func_801b445c_slot04_04(Object *obj);

void func_801b42ec_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b445c_slot04_04(obj);
    } else {
        func_801b432c_slot04_04(obj);
    }
}
