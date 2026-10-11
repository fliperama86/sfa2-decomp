/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4170_slot04_03(Object *obj) {
    obj->field_159 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->field_128 = 0;
    } else if (obj->field_12a == 4 && obj->field_219 != 0 && obj->field_21a == 0) {
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x1b);
    } else {
        func_80130dc0(obj);
    }
}
