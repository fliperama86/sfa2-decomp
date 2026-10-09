/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b321c_slot04_01(Object *obj) {
    obj->field_160 = 0xb4;
    obj->field_46 = 0;
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, ((Slot04aObj *)obj)->field_a6 ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
}
