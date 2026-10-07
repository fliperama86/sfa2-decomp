/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e74_slot04_08(Object *obj) {
    Slot04aObj *o = (Slot04aObj *)obj;

    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_160 = 0x60;
    obj->field_46 = 0;
    o->field_1c7 = 0;
    func_801307e0(obj, 0x1a);
}
