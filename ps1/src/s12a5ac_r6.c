/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012b2c4(Object *object) {
    if ((s16)object->field_3a >= 0) {
        func_80130efc(object);
    } else if (object->field_157 != 0) {
        func_80131468(object);
    } else {
        func_801312b8(object);
    }
}
