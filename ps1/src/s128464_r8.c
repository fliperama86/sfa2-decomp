/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a108(Object *object) {
    if (object->kind != 6 || *(u8 *)&object->field_3a == 0) {
        func_80130efc(object);
    } else {
        *(s16 *)&object->field_3a = -0x100;
    }
}

void func_8012a154(Object *object) {
    object->field_16a = 0;
    handlers_07[object->field_07](object);
}
