/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b34c4_slot04_00(Object *object);

void func_801b3294_slot04_00(Object *object) {
    object->field_06 = object->field_06 + 1;
    func_801b34c4_slot04_00(object);
    object->field_46 = 2;
    func_80131094(object);
}
