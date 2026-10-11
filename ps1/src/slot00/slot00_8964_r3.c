/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078bf4_slot00(Object *obj) {
    obj->field_60 -= 1;
    if (obj->field_60 & 0x80) {
        obj->field_60 = 0x13;
        obj->accel_x = -obj->accel_x;
        obj->field_09 ^= 6;
    }
    obj->field_61 -= 1;
    if (obj->field_61 & 0x80) {
        obj->field_61 = 0x1f;
        obj->accel_y = -obj->accel_y;
    }
    obj->field_45 -= 1;
    if (obj->field_45 & 0x80) {
        obj->field_45 = 0x27;
        obj->field_09 ^= 6;
    }
}
