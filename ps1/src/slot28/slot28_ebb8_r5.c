/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001f258_slot28(Object *obj) {
    int d = -0x8000;
    int t;
    if (obj->field_48 != 0) {
        obj->field_05++;
    }
    t = obj->field_46 + 1;
    obj->field_46 = t & 7;
    if (t & 4) {
        d = 0x10000;
    }
    ((Slot28Obj *)obj)->field_14 = ((Slot28Obj *)obj)->field_14 - d;
}

void func_8001f2ac_slot28(Object *obj) {
    ((Slot28Obj *)obj)->field_14 += 0x4000;
    if (obj->pos_y >= 0xc0) {
        obj->pos_y = 0xc0;
        obj->field_05++;
    }
}
