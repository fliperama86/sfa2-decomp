/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b496c_slot04_07(Object *obj);
void func_801b49a4_slot04_07(Object *obj);

void func_801b492c_slot04_07(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b496c_slot04_07(obj);
    } else {
        func_801b49a4_slot04_07(obj);
    }
}
