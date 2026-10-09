/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001f128_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05++;
        func_80130768(obj, 1, ((Slot28Obj *)obj)->field_6c);
    }
}
