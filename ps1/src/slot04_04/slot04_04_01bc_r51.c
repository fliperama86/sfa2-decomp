/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45e0_slot04_04(Object *obj);
void func_801b4610_slot04_04(Object *obj);

void func_801b45a0_slot04_04(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b45e0_slot04_04(obj);
    } else {
        func_801b4610_slot04_04(obj);
    }
}
