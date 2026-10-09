/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);

void func_801b2e64_slot04_03(Object *obj) {
    if (obj->field_3a != 0) {
        obj->field_07++;
        func_80146998(obj);
        obj->other->field_6b = 0x12;
    }
    func_80130efc(obj);
}

void func_801b2ec0_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07++;
        func_801307e0(obj, 0x42);
    }
    func_80130efc(obj);
}
