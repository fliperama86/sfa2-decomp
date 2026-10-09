/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7cc0_slot04_09[];

void func_801b2858_slot04_09(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->field_14 -= o->field_50;
    o->field_50 += o->field_58;
    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
}

void func_801b289c_slot04_09(Object *obj) {
    data_801c7cc0_slot04_09[obj->field_07](obj);
}
