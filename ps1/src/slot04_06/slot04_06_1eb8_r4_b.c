/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2350_slot04_06(Object *obj) {
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_54 = 0;
    obj->field_07++;
    obj->field_4c = 0x20000;
    obj->field_50 = 0x78000;
    obj->field_58 = -0x6000;
    func_801307e0(obj, 0x1f);
}

void func_801b23ac_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_1c3 != 0) {
        obj->field_1c3--;
    } else {
        if (obj->field_3a != 0) {
            o->field_45 = 1;
            obj->field_1c8 = 0;
            o->field_07++;
        }
        func_80130efc(o);
    }
}
