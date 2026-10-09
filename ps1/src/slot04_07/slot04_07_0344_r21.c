/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b492c_slot04_07(Object *obj);
void func_801b4a0c_slot04_07(Object *obj);

void func_801b48ec_slot04_07(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 == 0) {
        func_801b492c_slot04_07(obj);
    } else {
        func_801b4a0c_slot04_07(obj);
    }
}
