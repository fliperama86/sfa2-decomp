/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0658_slot04_08(Object *obj) {
    int k;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    k = 0xc;
    if (obj->field_48 != 0) {
        k = 0x12;
    }
    if (obj->field_129 != 0) {
        k += 3;
    } else if (obj->field_12a == 4 && (obj->field_130 & 0x4000) != 0) {
        func_801204f4(obj, obj->side, 0xc);
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x34);
        return;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + k));
}
