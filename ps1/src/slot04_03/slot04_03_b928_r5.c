/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1118_slot04_03(Object *obj) {
    u8 t;

    t = 1;
    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_15a = 7;
    obj->field_27b = 0x1c;
    obj->field_0b = t;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x18;
    func_801307e0(obj, 0x1f);
}

