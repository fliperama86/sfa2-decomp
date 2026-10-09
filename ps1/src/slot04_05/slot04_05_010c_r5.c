/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0b40_slot04_05(Object *obj) {
    int x = 0xc;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        x = 0x12;
    }
    if (obj->field_129 != 0) {
        x += 3;
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + x));
}
