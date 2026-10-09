/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ae0_slot04_07(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_160 = 0xb4;
    obj->field_46 = 0;
    ((Slot04aObj *)obj)->field_1c8 = 0;
    ((Slot04aObj *)obj)->field_1c9 = 0;
    ((Slot04aObj *)obj)->field_1ce = 0;
    func_801307e0(obj, 0x19);
}
