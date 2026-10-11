/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2e64_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        func_80146998(obj);
        obj->other->field_6b = 0x12;
    }
    func_80130efc(obj);
}

void func_801b2ec0_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        func_801307e0(obj, 0x42);
    }
    func_80130efc(obj);
}
