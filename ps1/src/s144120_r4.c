/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144544(Object *object) {
    func_80144220(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        func_80144e90(object);
    }
}

void func_80144590(Object *object) {
    handlers_b61c[object->field_06](object);
    func_80131094(object);
    func_80120028(object);
}

void func_801445ec(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->field_46 = 0x1f;
    object->field_4c = -0x20000;
    object->field_54 = -0x8000;
    object->field_06 = object->field_06 + 1;
    func_80144ee4(object, 0xa);
}

void func_80144640(Object *object) {
    func_80144220(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        object->field_46 = 0xf;
        object->field_54 = 0x40000;
        object->field_06 = object->field_06 + 1;
    }
}

void func_8014469c(Object *object) {
    func_80144220(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        object->pos_x = 0xc0;
        object->field_06 = object->field_06 + 1;
    }
}
