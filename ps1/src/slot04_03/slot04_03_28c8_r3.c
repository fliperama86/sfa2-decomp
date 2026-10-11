/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b68_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3++;
        func_80146998(obj);
        obj->other->field_6b = 6;
    }
    func_80130efc(obj);
}

void func_801b2bc4_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        func_801307e0(obj, 0x20);
    }
    func_80130efc(obj);
}
