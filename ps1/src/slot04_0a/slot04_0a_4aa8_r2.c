/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c68_slot04_0a(Object *obj) {
    int t;
    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    t = 0xc;
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + t));
}
