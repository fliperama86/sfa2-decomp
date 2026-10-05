/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_8012f898(Object *object);
void func_80130678(Object *object, int index);

void func_8012a804(Object *object) {
    int index;
    s32 speed;
    if (func_8012f898(object) == 0) {
        if (object->field_21a == object->field_48) {
            *(s32 *)&object->field_10 += object->field_4c;
            func_80130efc(object);
            return;
        }
        object->field_48 = object->field_21a;
    }
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
