/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801428e4(Object *object) {
    if (object->field_cd != 0) {
        func_80142968(object);
    } else {
        func_80141f28(object, table_8017ad54[object->field_255 >> 1]);
    }
}

void func_80142940(Object *object) {
    object->field_12a = 4;
    object->field_255 = 4;
    func_80142968(object);
}
