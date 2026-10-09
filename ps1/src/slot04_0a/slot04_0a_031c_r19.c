/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b48a8_slot04_0a(Object *obj);
void func_801b48e0_slot04_0a(Object *obj);

void func_801b4868_slot04_0a(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b48a8_slot04_0a(obj);
    } else {
        func_801b48e0_slot04_0a(obj);
    }
}
