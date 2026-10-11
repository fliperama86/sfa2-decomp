/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0fd4_slot04_03(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->state_2 = 7;
    obj->field_0b = t;
    obj->field_15a = 5;
    obj->field_27b = 0x1b;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x17;
    func_801307e0(obj, 0x1f);
}
