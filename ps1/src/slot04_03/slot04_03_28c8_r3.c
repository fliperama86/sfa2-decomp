/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b68_slot04_03(Object *obj) {
    if (obj->field_3a != 0) {
        obj->field_07++;
        func_80146998(obj);
        obj->other->field_6b = 6;
    }
    func_80130efc(obj);
}

void func_801b2bc4_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07++;
        func_801307e0(obj, 0x20);
    }
    func_80130efc(obj);
}
