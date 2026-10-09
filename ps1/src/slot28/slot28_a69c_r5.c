/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001add0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_14 = obj->field_14 - o->field_50;
    o->field_50 = o->field_50 + o->field_58;
    if (obj->field_47 != 0) {
        if (o->field_50 >= 0) {
            o->field_50 = 0x8000;
            obj->field_47 = 0;
            o->field_58 = -0x400;
        }
    } else if (o->field_50 < 0) {
        o->field_50 = -0x8000;
        o->field_58 = 0x400;
        obj->field_47 = 0xff;
    }
    func_80131094(o);
    o->field_01 = 1;
}
