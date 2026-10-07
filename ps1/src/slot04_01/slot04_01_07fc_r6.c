/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f48_slot04_01(Object *obj) {
    u8 t = 1;

    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_07 = 5;
    obj->field_12a = 4;
    obj->field_0b = t;
    obj->field_05 = 0;
    obj->field_15a = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x15;
    obj->field_27b = 0x19;
    func_801307e0(obj, 0x1b);
}
