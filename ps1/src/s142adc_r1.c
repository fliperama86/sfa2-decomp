/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

unsigned short func_80130470(Object *object);

void func_80142adc(Object *object) {
    if (object->field_49 != 0 && object->field_17b == 0 && func_80130470(object)) {
        func_8013047c(object);
    }
}

void func_80142b3c(Object *object) {
    func_80142c04(object);
    if (((object->field_134 | object->field_136) & 0x80) || ((object->field_134 | object->field_136) & 1)) {
        object->field_12a = 0;
    } else if ((object->field_134 | object->field_136) & 0x10) {
        object->field_12a = 2;
    } else {
        object->field_12a = 4;
    }
}

void func_80142ba0(Object *object) {
    func_80142c04(object);
    if (((object->field_134 | object->field_136) & 0x40) || ((object->field_134 | object->field_136) & 2)) {
        object->field_12a = 0;
    } else if ((object->field_134 | object->field_136) & 0x20) {
        object->field_12a = 2;
    } else {
        object->field_12a = 4;
    }
}

void func_80142c04(Object *object) {
    object->field_49 = 0;
    object->field_29c = 0;
    object->field_29a = 0;
    if (object->field_7e != 0) object->field_49 = object->field_49 + 1;
    ref_other.p = object->other;
    if (object->field_66 != 0) ref_other.p->field_289 = 0;
    else ref_other.p->field_288 = 0;
}
