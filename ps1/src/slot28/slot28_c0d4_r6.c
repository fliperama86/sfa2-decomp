/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001c998_slot28(Object *o) {
    if (((Slot28Obj *)o)->field_3a < 0) {
        o->field_04++;
    }
    o->field_01 = 0;
    func_80131094(o);
    o->field_48 = o->field_48 ^ 1;
    if (o->field_48 != 0) {
        o->field_01 = 1;
    }
}
