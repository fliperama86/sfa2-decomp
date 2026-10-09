/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001ad5c_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_10 += 0x8000;
    if (o->pos_x >= 0x90) {
        o->field_05++;
        o->pos_x = 0x90;
    }
    func_80131094(o);
}
