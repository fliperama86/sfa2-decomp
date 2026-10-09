/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);

void func_801b3e84_slot04_06(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        func_80146998(obj);
    }
    func_80130efc(obj);
}
