/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801c8984_slot05_06(Object *obj) {
    u16 x;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    x = 0x12;
    if (obj->field_48 == 0) {
        x = 0xc;
    }
    if (obj->field_129 != 0) {
        x += 3;
    }
    x += obj->field_12a >> 1;
    func_801307e0(obj, x);
}
