#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b49bc_slot04_08(Object *obj) {
    int s = 0xc;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        s = 0x12;
    }
    if (obj->field_129 != 0) {
        s += 3;
    }
    if (obj->field_219 != 0) {
        func_801204f4(obj, obj->side, 0xc);
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x34);
    } else {
        func_801307e0(obj, (s16)((obj->field_12a >> 1) + s));
    }
}
