/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012c6a4(Object *object) {
    u16 base;
    int step;
    object->field_60 = 0;
    object->countdown = 0;
    object->facing_side = object->saved_facing;
    base = 7;
    if (object->field_157 == 0) base = 4;
    step = object->field_62;
    if (step >= 2) step = 2;
    base += step;
    func_801308c4(object, base);
    func_80131e88(object);
    if (object->field_6b == 0) func_8012cca8(object);
}

void func_8012c72c(Object *object) {
    u16 base;
    int step;
    object->field_60 = 0;
    object->countdown = 0;
    object->field_157 = 0;
    object->facing_side = object->saved_facing;
    base = 1;
    if (object->hit_type == 0x18) base = 4;
    step = object->field_62;
    if (step >= 2) step = 2;
    base += step;
    func_801308c4(object, base);
    if (object->field_6b == 0) func_8012cca8(object);
}
