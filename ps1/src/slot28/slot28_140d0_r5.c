/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800246e8_slot28(Object *o) {
    Slot28Obj *obj;
    int t = *(u16 *)&((Slot28Obj *)o)->field_46 - 1;
    *(u16 *)&((Slot28Obj *)o)->field_46 = t;
    obj = (Slot28Obj *)o;
    if ((s16)t == 0) {
        obj->field_05 += 1;
    }
    obj->field_10 += obj->field_4c;
    obj->field_14 += obj->field_50;
}
