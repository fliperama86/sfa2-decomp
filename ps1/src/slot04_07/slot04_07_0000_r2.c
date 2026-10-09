/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0190_slot04_07(Object *obj) {
    obj->field_06 += 1;
    if (obj->field_0b == 0) {
        obj->field_4c = -0x40000;
    } else {
        obj->field_4c = 0x40000;
    }
    obj->field_50 = 0x10000;
    obj->field_54 = 0;
    obj->field_58 = -0x2a00;
    func_801204f4(obj, obj->side, 6);
}
