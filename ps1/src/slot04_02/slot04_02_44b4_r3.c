/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ad8_slot04_02(Object *obj) {
    Object *p;

    obj->field_07 = 6;
    obj->field_50 = 0x40000;
    obj->field_58 = -0x5800;
    p = obj->other;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x20000;
    } else {
        obj->field_4c = -0x20000;
    }
    obj->field_54 = 0;
    func_80141f28(obj, 8);
    func_80120554(p, p->side, 0x31a);
    func_801307e0(obj, 0x4a);
    func_80141e5c(obj);
}
