/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *obj);

void func_801b3700_slot04_08(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80146998(obj);
}
