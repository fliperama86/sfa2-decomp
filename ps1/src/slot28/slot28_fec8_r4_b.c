/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80020608_slot28(Object *o);

void func_80020608_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;

    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    obj->field_14 -= o->field_50;
    o->field_50 += o->field_58;
}
