/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4a60_slot04_07(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 4) {
        func_80130dc0(obj);
    } else if (obj->field_219 == 0) {
        obj->field_07 = 2;
        func_80120554(obj, obj->side, 0x324);
        obj->field_4c = 0x90000;
        obj->field_54 = -0x8000;
        func_80130dc0(obj);
    } else {
        obj->field_07 = 3;
        obj->field_157 = 0;
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x2d);
    }
}
