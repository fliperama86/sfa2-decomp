/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f78_slot04_07(Object *obj) {
    Object *o;

    if (((Slot04aObj *)obj)->field_3a != 0) {
        o = obj->other;
        obj->field_07 = obj->field_07 + 1;
        o->field_15b = 1;
        func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
        if (((Slot04aObj *)o)->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x17;
            }
        }
        obj->field_17b = 0;
    }
    func_80130efc(obj);
}
