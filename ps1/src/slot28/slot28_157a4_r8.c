/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800262d8_slot28(Object *obj) {
}

void func_800262e0_slot28(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
}
