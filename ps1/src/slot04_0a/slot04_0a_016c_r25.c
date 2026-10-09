/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;

    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    obj->field_14 -= o->field_50;
    o->field_50 += o->field_58;
}
