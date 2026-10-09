/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800244dc_slot28(Object *o) {
    Slot28Obj *obj;
    ((Slot28Obj *)o)->field_47--;
    obj = (Slot28Obj *)o;
    if (obj->field_47 == 0) {
        obj->field_05 += 1;
    }
    obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}
