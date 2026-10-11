/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1200_slot04_05(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->field_15a = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->state_2 = 7;
    obj->state_3 = 5;
    obj->field_12a = 4;
    obj->field_0b = t;
    obj->state_1 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x1b;
    obj->field_27b = 0x1f;
    func_801307e0(obj, 0x1a);
}
