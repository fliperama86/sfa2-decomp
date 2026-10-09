/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80024424_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_47--;
    if (obj->field_47 == 0) {
        o->field_05++;
    }
}
