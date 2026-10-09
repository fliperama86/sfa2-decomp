/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5b04_slot04_02(Object *object);
void func_801b5ca8_slot04_02(Object *object);

void func_801b5ab4_slot04_02(Object *obj) {
    if (obj->field_129 == 0 || obj->field_12a != 4) {
        func_801b5b04_slot04_02(obj);
    } else {
        func_801b5ca8_slot04_02(obj);
    }
}
