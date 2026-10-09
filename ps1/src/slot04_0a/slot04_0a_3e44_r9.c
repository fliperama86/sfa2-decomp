/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4988_slot04_0a(Object *obj);
void func_801b4a68_slot04_0a(Object *obj);

void func_801b4948_slot04_0a(Object *obj) {
    if (obj->field_12a != 4) {
        func_801b4988_slot04_0a(obj);
    } else {
        func_801b4a68_slot04_0a(obj);
    }
}

