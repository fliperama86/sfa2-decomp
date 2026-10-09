/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80023fe8_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_48 = 0;
        obj->field_46 = 0x30;
        obj->field_05 = obj->field_05 + 1;
        func_80130768(obj, 2, ((Slot28Obj *)obj)->field_6c);
    }
}

void func_80024030_slot28(Object *obj) {
    int d;
    if (((Slot28Obj *)obj)->field_47 != 0) {
        d = -1;
    } else {
        d = 1;
    }
    ((Slot28Obj *)obj)->field_14 = ((Slot28Obj *)obj)->field_14 - d;
    ((Slot28Obj *)obj)->field_46--;
    ((Slot28Obj *)obj)->field_47 = ((Slot28Obj *)obj)->field_47 ^ 1;
    if (((Slot28Obj *)obj)->field_46 == 0) {
        obj->field_05 = obj->field_05 + 1;
    }
}
