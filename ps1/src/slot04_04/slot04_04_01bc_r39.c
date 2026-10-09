/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b373c_slot04_04(Object *obj);
void func_801b3594_slot04_04(Object *obj);

void func_801b3554_slot04_04(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b373c_slot04_04(obj);
    } else {
        func_801b3594_slot04_04(obj);
    }
}
