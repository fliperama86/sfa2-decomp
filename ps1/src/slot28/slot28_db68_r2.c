/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001e2ec_slot28(Object *obj, int arg);

void func_8001df30_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    data_8018f5a0->field_52 += 1;
    func_801204f4(0, obj->field_227, 0x14);
    func_8001e2ec_slot28(o, 0);
}
