/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130678(Object *object, int index);

void func_8012a5ac(Object *object) {
    object->field_16a = 0;
    handler_table_19a8[object->field_07](object);
}

void func_8012a5ec(Object *object) {
    int index;
    s32 speed;
    object->field_157 = 0;
    object->field_07 = object->field_07 + 1;
    object->field_0b = object->field_158;
    index = 2;
    if (object->field_48 != 0) {
        index = 3;
    }
    func_80130678(object, index);
    speed = motion_speed_table[object->kind * 2 + (object->field_48 & 1)];
    if (object->field_0b != 0) {
        speed = -speed;
    }
    object->field_4c = speed;
    func_8012a684(object);
}
