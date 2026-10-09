/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5478_slot04_09(Object *obj);

void func_801b43f0_slot04_09(Object *obj) {
    if (obj->field_67 != 0) {
        obj->field_67 = 0;
        obj->other->field_6b = 4;
    }
    if (obj->field_3a & 0xf) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 4);
    }
    func_80130efc(obj);
}

void func_801b4470_slot04_09(Object *obj) {
    if (obj->field_3a & 0xf) {
        obj->field_07++;
        func_801b5478_slot04_09(obj);
    }
    func_80130efc(obj);
}
