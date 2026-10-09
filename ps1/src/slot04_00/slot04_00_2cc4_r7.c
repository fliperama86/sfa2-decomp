/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b33c8_slot04_00(Object *obj) {
    obj->field_05 = obj->field_05 + 1;
    ref_other.p = obj->field_3c;
    ref_other.p->field_14c = 0;
    func_80138070(obj, 6);
}
