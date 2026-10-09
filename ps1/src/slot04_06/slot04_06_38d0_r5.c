/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4620_slot04_06(Object *object);

void func_801b3db0_slot04_06(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_157 = 0;
    func_801307e0(obj, 0x3c);
}

void func_801b3e00_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->field_3b & 0x80) {
        func_801b4620_slot04_06(o);
    } else {
        func_80130efc(o);
    }
}
