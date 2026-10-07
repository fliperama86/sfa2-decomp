/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b089c_slot04_0a(Object *obj);
void func_801b092c_slot04_0a(Object *obj);

void func_801b085c_slot04_0a(Object *obj) {
    if (obj->field_129 == 0) {
        func_801b089c_slot04_0a(obj);
    } else {
        func_801b092c_slot04_0a(obj);
    }
}
