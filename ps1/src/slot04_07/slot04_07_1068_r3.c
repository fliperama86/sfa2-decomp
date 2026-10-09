/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1548_slot04_07(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->other->field_6b = 0x12;
    obj->field_6b = 0;
    obj->field_27b = 0x16;
    func_801307e0(obj, 0x50);
}
