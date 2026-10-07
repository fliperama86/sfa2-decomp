/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2a8c_slot04_08(Object *obj);
void func_801b2af4_slot04_08(Object *obj);

void func_801b2a4c_slot04_08(Object *obj) {
    if (obj->field_12c == 0) {
        func_801b2a8c_slot04_08(obj);
    } else {
        func_801b2af4_slot04_08(obj);
    }
}
