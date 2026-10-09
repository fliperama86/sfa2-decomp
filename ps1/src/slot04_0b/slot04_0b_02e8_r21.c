/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e88_slot04_0b(Object *object);

void func_801b3b3c_slot04_0b(Object *object) {
    object->field_06 = object->field_06 + 1;
    func_801b3e88_slot04_0b(object);
    object->field_46 = 2;
    func_80131094(object);
}
