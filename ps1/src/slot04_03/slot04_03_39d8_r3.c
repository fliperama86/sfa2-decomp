/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80137b64(Object *object);

void func_801b3c88_slot04_03(Object *obj) {
    ref_other.p = obj->field_3c;
    ref_other.p->field_225 = 1;
    func_80137b64(obj);
}
